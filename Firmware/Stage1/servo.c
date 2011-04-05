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

#include "protocol.h"

struct control_msg rx_msg;
struct reply_msg   tx_msg;
unsigned int8 rx_msg_len;

#define CH1_PIN PIN_C7
#define CH1_PIN2 PIN_B7
#define CH1_PIN3 PIN_C1
#define CH2_PIN PIN_C6
#define CH2_PIN2 PIN_B6
#define CH2_PIN3 PIN_C2



#use fast_io (C)

#define INT_MUL		  16         /* interval multiplication */

/* channel1: 0.7 - 1.5, channel2: 1 - 2 */
#define PWM_TOTAL_INT  INT_MUL*2000       /* 20 ms */

/* displayed pwm parameters */
int1 ch1_enabled, ch2_enabled;

/* timer counters for channels and total */
int16 pwm_delay1, pwm_delay2;

/* pwm state */
int1 c0_state, c1_state;

void process_usb_data ();
void update_lcd ();
void set_pwm_enabled (int1 ch1, int1 ch2);
void set_pwm_delay (int8 channel, int16 val);

void reset_ch1 ();
void reset_ch2 ();


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
        reset_ch1 ();
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
        reset_ch2 ();
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
    set_pwm_delay (0, 0);
    set_pwm_delay (1, 0);

    setup_timer_0 (RTCC_INTERNAL | RTCC_DIV_8);
    setup_timer_1 (T1_INTERNAL | T1_DIV_BY_8);
    enable_interrupts (GLOBAL);
    set_pwm_enabled (0, 0);

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
    switch (rx_msg.kind) {
    case MSG_GET_VER:
        tx_msg.kind = MSG_GET_VER;
        tx_msg.u.get_ver.major = VERSION_MAJOR;
        tx_msg.u.get_ver.minor = VERSION_MINOR;
        usb_put_packet (1, &tx_msg, sizeof (tx_msg), USB_DTS_TOGGLE);
        return;

    case MSG_ENABLE_SERVO:
        set_pwm_enabled (rx_msg.u.enable_servo.ch1,
                         rx_msg.u.enable_servo.ch2);
        break;

    case MSG_SET_SERVO:
        set_pwm_delay (rx_msg.u.set_servo.channel,
                       rx_msg.u.set_servo.value);
        break;
    }

    tx_msg.kind = MSG_REPLY;
    tx_msg.u.reply = REPLY_OK;
    usb_put_packet (1, &tx_msg, sizeof (tx_msg), USB_DTS_TOGGLE);

    update_lcd ();
}


void update_lcd ()
{
    lcd_gotoxy (1, 1);
    printf (lcd_putc, "%d%d%d\n", usb_enumerated (), ch1_enabled, ch2_enabled);
    printf (lcd_putc, "I1=%.2fms, I2=%.2fms\n", (float)pwm_delay1/100.0/INT_MUL, (float)pwm_delay2/100.0/INT_MUL);
}


void set_pwm_enabled (int1 ch1, int1 ch2)
{
    if (ch1_enabled != ch1) {
        ch1_enabled = ch1;
        if (ch1)
            enable_interrupts (INT_TIMER0);
        else
            disable_interrupts (INT_TIMER0);
    }

    if (ch2_enabled != ch2) {
        ch2_enabled = ch2;
        if (ch2)
            enable_interrupts (INT_TIMER1);
        else
            disable_interrupts (INT_TIMER1);
    }
}


void set_pwm_delay (int8 channel, int16 value)
{
    switch (channel) {
    case 0:
        pwm_delay1 = INT_MUL*value;
        break;
    case 1:
        pwm_delay2 = INT_MUL*value;
        break;
    }
}


/* start new pwm iteration */
void reset_ch1 ()
{
    if (!ch1_enabled)
        return;

    output_high (CH1_PIN);
    output_high (CH1_PIN2);
    output_high (CH1_PIN3);
    set_timer0 (-pwm_delay1);
    c0_state = 1;
}


void reset_ch2 ()
{
    if (!ch2_enabled)
        return;

    output_high (CH2_PIN);
    output_high (CH2_PIN2);
    output_high (CH2_PIN3);
    set_timer1 (-pwm_delay2);
    c1_state = 1;
}
