#include <servo.h>

/* LCD control */
#define LCD_EXTENDED_NEWLINE TRUE

#define LCD_ENABLE_PIN PIN_B2
#define LCD_RS_PIN PIN_B0
#define LCD_RW_PIN PIN_B1
#define LCD_DATA4 PIN_A4
#define LCD_DATA5 PIN_A5
#define LCD_DATA6 PIN_B3
#define LCD_DATA7 PIN_B4
#include <lcd.c>

#include <pic18_usb.h>
#include "usb_desc.h"
#include <usb.c>

/* usb buffers */
struct pwm_msg {
    union {
        unsigned int8 buf[64];
        struct {
            int8 enabled;
            int8 pwm1_duty_percent;
            int8 pwm2_duty_percent;
        };
    } u;
};

struct pwm_msg rx_msg;
unsigned int8 rx_msg_len;

//#define CH1_PIN PIN_B7
//#define CH1_PIN2 PIN_C7
//#define CH2_PIN PIN_B6
//#define CH2_PIN2 PIN_C6

#define CH1_PIN PIN_C1
#define CH1_PIN2 PIN_C1
#define CH2_PIN PIN_C2
#define CH2_PIN2 PIN_C2


//#use fast_io (C)

#define PWM_LOW_INT_C1    70         /* 0.5 ms */
#define PWM_HIGH_INT_C1   230        /* 1.5 ms */

#define PWM_LOW_INT_C2    100         /* 1 ms */
#define PWM_HIGH_INT_C2   200        /* 2 ms */

#define PWM_TOTAL_INT  2000       /* 20 ms */

/* We want to get interrupt at 10KHz to be able to handle 100HZ PWM with 1% */
#define INT_TO_COUNT(i) ((int16)(i)*10/(4*CLOCK_HZ/1000000))
#define DUTY_TO_INT_C1(p) (PWM_LOW_INT_C1 + ((PWM_HIGH_INT_C1-PWM_LOW_INT_C1)*(int16)p/100))
#define DUTY_TO_INT_C2(p) (PWM_LOW_INT_C2 + ((PWM_HIGH_INT_C2-PWM_LOW_INT_C2)*(int16)p/100))

/* pwm parameters */
int1 pwm_enabled;
int8 pwm_duty1;                  /* in percent */
int8 pwm_duty2;

/* pwm state */
int16 pwm_int1, pwm_int2;
int16 pwm_duty1_cnt, pwm_duty2_cnt, pwm_total_cnt;


void process_usb_data ();
void update_lcd ();
void set_pwm_enabled (int1 val);
void set_pwm_duty (int8 duty1, int8 duty2);
void reset_pwm ();


inline void set_channel (int1 channel, int1 value)
{
   if (!channel) {
      if (!value) {
         output_low (CH1_PIN);
         output_low (CH1_PIN2);
      }
      else {
         output_high (CH1_PIN);
         output_high (CH1_PIN2);
      }
   }
   else {
      if (!value) {
         output_low (CH2_PIN);
         output_low (CH2_PIN2);
      }
      else {
         output_high (CH2_PIN);
         output_high (CH2_PIN2);
      }
   }
}

inline int16 update_pwm_cnt (int16 cnt, int1 channel)
{
    if (cnt > 1)
        return cnt-1;
    else {
        if (cnt)
            set_channel (channel, 0);
        return 0;
    }
}

#int_timer0
void isr_timer0 (void)
{
    pwm_duty1_cnt = update_pwm_cnt (pwm_duty1_cnt, 0);
    pwm_duty2_cnt = update_pwm_cnt (pwm_duty2_cnt, 1);

    if (pwm_total_cnt > 0)
        pwm_total_cnt--;
    else {
        static int1 c1_dir = 1;
        if (c1_dir) {
            pwm_int1++;
            if (pwm_int1 == PWM_HIGH_INT_C1)
                c1_dir = 0;
        }
        else {
            pwm_int1--;
            if (pwm_int1 == PWM_LOW_INT_C1)
                c1_dir = 1;
        }
        reset_pwm ();
    }
}


void main()
{
    int16 c = 0;
    usb_init ();
    lcd_init ();
    lcd_putc ('\f');
    delay_ms (1);
    update_lcd ();

    set_tris_b (0);             /* output on B */
    set_tris_c (0);             /* output on C */
    set_pwm_enabled (0);
    set_pwm_duty (0, 50);
    pwm_int1 = DUTY_TO_INT_C1(0);
    reset_pwm ();

    setup_timer_0 (RTCC_INTERNAL | RTCC_DIV_1 | RTCC_8_BIT);
    enable_interrupts (INT_TIMER0);
    enable_interrupts (GLOBAL);

    for (;;) {
        usb_task ();
        if (usb_enumerated ()) {
            if (usb_kbhit (1)) {
                rx_msg_len = usb_get_packet (1, &rx_msg, sizeof (rx_msg));
                process_usb_data ();
            }
        }
        update_lcd ();
        delay_ms (100);
    }
}


void process_usb_data ()
{
    set_pwm_enabled (rx_msg.u.enabled);
    set_pwm_duty (rx_msg.u.pwm1_duty_percent, rx_msg.u.pwm2_duty_percent);
    update_lcd ();
}


void update_lcd ()
{
    lcd_gotoxy (1, 1);
    printf (lcd_putc, "%d%d, C1=%d%%, C2=%d%%\n", usb_enumerated (), pwm_enabled, pwm_duty1, pwm_duty2);
    printf (lcd_putc, "I1=%.2fms, I2=%.2fms\n", (float)pwm_int1/100.0, (float)pwm_int2/100.0);
}


void set_pwm_enabled (int1 val)
{
    if (pwm_enabled != val) {
        pwm_enabled = val;
        if (pwm_enabled)
            enable_interrupts (INT_TIMER0);
        else
            disable_interrupts (INT_TIMER0);
    }
}


void set_pwm_duty (int8 c1, int8 c2)
{
    pwm_duty1 = c1;
    pwm_duty2 = c2;
//    pwm_int1 = DUTY_TO_INT_C1(c1);
    pwm_int2 = DUTY_TO_INT_C2(c2);
}



void reset_pwm ()
{
    if (pwm_enabled) {
        set_channel (0, 1);
        set_channel (1, 1);
    }
    else {
        set_channel (0, 0);
        set_channel (1, 0);
    }

    pwm_duty1_cnt = INT_TO_COUNT (pwm_int1);
    pwm_duty2_cnt = INT_TO_COUNT (pwm_int2);
    pwm_total_cnt = INT_TO_COUNT (PWM_TOTAL_INT);
}
