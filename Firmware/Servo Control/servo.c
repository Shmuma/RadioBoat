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

#define CH1_PIN PIN_B7
#define CH2_PIN PIN_C2
// PIN_C2
// PIN_B7

#use fast_io (A)

#define PWM_LOW_INT    50         /* 0.5 ms */
#define PWM_HIGH_INT   500        /* 5 ms */
#define PWM_TOTAL_INT  2500       /* 25 ms */

/* We want to get interrupt at 10KHz to be able to handle 100HZ PWM with 1% */
#define INT_TO_COUNT(i) ((int16)(i)*10/(4*CLOCK_HZ/1000000))
#define DUTY_TO_INT(p) (PWM_LOW_INT + ((PWM_HIGH_INT-PWM_LOW_INT)*(int16)p/100))

/* pwm parameters */
int8 pwm_duty1;                  /* in percent */
int8 pwm_duty2;

/* pwm state */
int16 pwm_int1, pwm_int2;
int16 pwm_duty1_cnt, pwm_duty2_cnt, pwm_total_cnt;


void update_lcd ();
void set_pwm_duty (int8 duty1, int8 duty2);
void reset_pwm ();


inline int16 update_pwm_cnt (int16 cnt, int8 channel)
{
    if (cnt > 1)
        return cnt-1;
    else {
        if (cnt)
            output_low (channel);
        return 0;
    }
}

#int_timer0
void isr_timer0 (void)
{
    pwm_duty1_cnt = update_pwm_cnt (pwm_duty1_cnt, CH1_PIN);
    pwm_duty2_cnt = update_pwm_cnt (pwm_duty2_cnt, CH2_PIN);

    if (pwm_total_cnt > 0)
        pwm_total_cnt--;
    else
        reset_pwm ();
}


void main()
{
    int16 c = 0;
    lcd_init ();
    lcd_putc ('\f');

    set_tris_b (0);             /* output on B */
    set_tris_c (0);             /* output on C */
    set_pwm_duty (0, 0);

    setup_timer_0 (RTCC_INTERNAL | RTCC_DIV_1 | RTCC_8_BIT);
    enable_interrupts (INT_TIMER0);
    enable_interrupts (GLOBAL);

    for (;;) {
        update_lcd ();
        delay_ms (100);
        c++;
        if (!(c % 5))
            set_pwm_duty (pwm_duty1+1, pwm_duty2+1);
    }
}


void update_lcd ()
{
    lcd_gotoxy (1, 1);
    printf (lcd_putc, "C1=%d%%, C2=%d%%\n", pwm_duty1, pwm_duty2);
    printf (lcd_putc, "I1=%.2fms, I2=%.2fms\n", (float)pwm_int1/100.0, (float)pwm_int2/100.0);
}


void set_pwm_duty (int8 c1, int8 c2)
{
    pwm_duty1 = c1;
    pwm_duty2 = c2;
    pwm_int1 = DUTY_TO_INT(c1);
    pwm_int2 = DUTY_TO_INT(c2);
    reset_pwm ();
}



void reset_pwm ()
{
    output_high (CH1_PIN);
    output_high (CH2_PIN);
    pwm_duty1_cnt = INT_TO_COUNT (pwm_int1);
    pwm_duty2_cnt = INT_TO_COUNT (pwm_int2);
    pwm_total_cnt = INT_TO_COUNT (PWM_TOTAL_INT);
}
