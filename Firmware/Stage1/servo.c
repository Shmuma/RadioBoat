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

#define CH1_PIN PIN_C7
#define CH1_PIN2 PIN_B7
#define CH1_PIN3 PIN_C1
#define CH2_PIN PIN_C6
#define CH2_PIN2 PIN_B6
#define CH2_PIN3 PIN_C2



#use fast_io (C)

#define INT_MUL		  16         /* interval multiplication */
#define PWM_LOW_INT_C1    70         /* 0.7 ms */
#define PWM_HIGH_INT_C1   150        /* 1.5 ms */

#define PWM_LOW_INT_C2    100         /* 1 ms */
#define PWM_HIGH_INT_C2   200        /* 2 ms */

#define PWM_TOTAL_INT  INT_MUL*2000       /* 20 ms */

/* We want to get interrupt at 10KHz to be able to handle 100HZ PWM with 1% */
#define DUTY_TO_INT_C1(p) (PWM_LOW_INT_C1 + ((PWM_HIGH_INT_C1-PWM_LOW_INT_C1)*(int16)p/100))
#define DUTY_TO_INT_C2(p) (PWM_LOW_INT_C2 + ((PWM_HIGH_INT_C2-PWM_LOW_INT_C2)*(int16)p/100))

/* displayed pwm parameters */
int1 pwm_enabled;
int8 pwm_duty1;                  /* in percent */
int8 pwm_duty2;

/* timer counters for channels and total */
int16 pwm_delay1, pwm_delay2;

/* pwm state */
int1 c0_state, c1_state;

void process_usb_data ();
void update_lcd ();
void set_pwm_enabled (int1 val);
void set_pwm_duty (int8 duty1, int8 duty2);

/* initialize new pwm cycle (setup delays and starts timer) */
void reset_pwm (int1 channel);


#int_timer0
void isr_timer0 (void)
{
    if (c0_state) {
        output_low (CH1_PIN);
        output_low (CH1_PIN2);
        output_low (CH1_PIN3);
        set_timer0 (pwm_delay1 - PWM_TOTAL_INT);
        c0_state = 0;
    }
    else
        reset_pwm (0);
}


#int_timer1
void isr_timer1 (void)
{
    if (c1_state) {
        output_low (CH2_PIN);
        output_low (CH2_PIN2);
        output_low (CH2_PIN3);
        set_timer1 (pwm_delay2 - PWM_TOTAL_INT);
        c1_state = 0;
    }
    else
        reset_pwm (1);
}


void main()
{
    usb_init ();
    lcd_init ();
    lcd_putc ('\f');
    delay_ms (1);
    update_lcd ();

    set_tris_b (0);             /* output on B */
    set_tris_c (0);             /* output on C */
    set_pwm_duty (0, 50);

    setup_timer_0 (RTCC_INTERNAL | RTCC_DIV_8);
    setup_timer_1 (T1_INTERNAL | T1_DIV_BY_8);
    enable_interrupts (GLOBAL);
    set_pwm_enabled (0);

    for (;;) {
        usb_task ();
        if (usb_enumerated ()) {
            if (usb_kbhit (1)) {
                rx_msg_len = usb_get_packet (1, &rx_msg, sizeof (rx_msg));
                process_usb_data ();
            }
        }
        delay_ms (1);
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
    printf (lcd_putc, "I1=%.2fms, I2=%.2fms\n", (float)pwm_delay1/100.0/INT_MUL, (float)pwm_delay2/100.0/INT_MUL);
}


void set_pwm_enabled (int1 val)
{
    if (pwm_enabled != val) {
        pwm_enabled = val;
        if (pwm_enabled) {
            enable_interrupts (INT_TIMER0);
            enable_interrupts (INT_TIMER1);
        }
        else {
            disable_interrupts (INT_TIMER0);
            disable_interrupts (INT_TIMER1);
        }
    }
}


void set_pwm_duty (int8 c1, int8 c2)
{
    pwm_duty1 = c1;
    pwm_duty2 = c2;
    pwm_delay1 = INT_MUL*DUTY_TO_INT_C1(c1);
    pwm_delay2 = INT_MUL*DUTY_TO_INT_C2(c2);
}


/* start new pwm iteration */
void reset_pwm (int1 channel)
{
    if (!pwm_enabled)
        return;

    if (!channel) {
        output_high (CH1_PIN);
        output_high (CH1_PIN2);
        output_high (CH1_PIN3);
        set_timer0 (-pwm_delay1);
        c0_state = 1;
    }
    else {
        output_high (CH2_PIN);
        output_high (CH2_PIN2);
        output_high (CH2_PIN3);
        set_timer1 (-pwm_delay2);
        c1_state = 1;
    }
}