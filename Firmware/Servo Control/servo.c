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

#define SIGNAL_PIN PIN_B7

#use fast_io (A)

/* We want to get interrupt at 10KHz to be able to handle 100HZ PWM with 1% */
#define T0_VAL (65535 - (CLOCK_HZ/40000) + 50)

/* pwm parameters */
int8 pwm_freq;                  /* in Hz */
int8 pwm_duty;                  /* in percent */

int16 t0_value;                 /* precalculated 16-bit counter value to obtain pwm_freq*100 t0 overflow rate */

/* pwm state */
int8 pwm_duty_cnt, pwm_cycle_cnt;

void update_lcd ();
void set_pwm_freq (int8 freq);
void set_pwm_duty (int8 duty);

#int_timer0
void isr_timer0 (void)
{
    set_timer0 (t0_value);

    if (pwm_duty_cnt > 1)
        pwm_duty_cnt--;
    else {
        if (pwm_duty_cnt) {
            output_low (SIGNAL_PIN);
            pwm_duty_cnt = 0;
        }
    }

    if (pwm_cycle_cnt > 0)
        pwm_cycle_cnt--;
    else {
        output_high (SIGNAL_PIN);
        pwm_duty_cnt = pwm_duty;
        pwm_cycle_cnt = 100;
    }
}


void main()
{
    lcd_init ();
    lcd_putc ('\f');

    set_tris_b (0);             /* output on B */
    set_pwm_freq (70);
    set_pwm_duty (30);

    setup_timer_0 (RTCC_INTERNAL | RTCC_DIV_1);
    enable_interrupts (INT_TIMER0);
    enable_interrupts (GLOBAL);
    set_timer0 (t0_value);

    output_high (SIGNAL_PIN);
    pwm_duty_cnt = pwm_duty;
    pwm_cycle_cnt = 100;

    for (;;) {
        update_lcd ();
        delay_ms (100);
    }
}


void update_lcd ()
{
    lcd_gotoxy (1, 1);
    printf (lcd_putc, "PWM: %d Hz, %d%%\n", pwm_freq, pwm_duty);
    printf (lcd_putc, "Cycle: %d\n", pwm_cycle_cnt);
}


void set_pwm_freq (int8 freq)
{
    /* update t0_value */
    t0_value = 65535 - (CLOCK_HZ/400) / freq + 50;
    pwm_freq = freq;
}


void set_pwm_duty (int8 duty)
{
    pwm_duty = duty;
}
