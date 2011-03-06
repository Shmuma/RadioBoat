#include <18F2550.h>
// 48MHz operation from external clock
#FUSES NOWDT, WDT128, PLL3, CPUDIV4, USBDIV, HSPLL, FCMEN, IESO, NOPUT, NOBROWNOUT, BORV20, VREGEN, NOPBADEN, LPT1OSC, NOMCLR, STVREN, NOLVP, NOXINST, NODEBUG, NOPROTECT, NOCPB, NOCPD, NOWRT, NOWRTC, NOWRTB, NOWRTD, NOEBTR, NOEBTRB, CCP2C1

/* 8MHz from internal clock, but for USB it uses external clock which should be 48MHz */
// #FUSES NOWDT, WDT128, PLL3, CPUDIV2, USBDIV, INTHS, FCMEN, IESO, NOPUT, NOBROWNOUT, BORV20, VREGEN, NOPBADEN, LPT1OSC, NOMCLR, STVREN, NOLVP, NOXINST, NODEBUG, NOPROTECT, NOCPB, NOCPD, NOWRT, NOWRTC, NOWRTB, NOWRTD, NOEBTR, NOEBTRB, CCP2C1

#use delay(clock=16000000)
/* 8KHz - 61Hz */
/* 4KHz - 30Hz */
/* 16MHz - 1Hz */

/* USB */
//#define USB_HID_DEVICE FALSE

#include <pic18_usb.h>
#include "usb_desc.h"
#include <usb.c>

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

/* usb buffers */
struct pwm_msg {
    union {
        unsigned int8 buf[64];
        struct {
            int8 enabled;
            int8 t2_divisor;
            int8 t2_period;
            int16 pwm1_duty;
            int16 pwm2_duty;
        };
    } u;
};

struct pwm_msg rx_msg;
unsigned int8 rx_msg_len;

/* pwm status */
static int1 pwm_enabled;

/*
  Two below values are define pwm cycle time:

  Divisor values 1, 4 and 16:
  1. (1/48000000)*4*1*period - 127 - 30 KHz 
  4. (1/48000000)*4*4*period - 127 - 7.5 KHz
  16. (1/48000000)*4*16*period - 127 - 2Khz, 200 - 1.2KHz, 255 - 972 Hz
 */
static int8 t2_divisor;         /* 1, 4, 16 */
static int8 t2_period;          /* 1 to 255 */
static int16 pwm1_duty = 200;
static int16 pwm2_duty = 800;


void process_usb_data ();
void lcd_refresh ();
void pwm_init (int1 enabled, int8 divisor, int8 period);
void pwm_duty (int16 c1, int16 c2);


void main ()
{
    setup_timer_3 (T3_DISABLED|T3_DIV_BY_1);
//    pwm_init (0, 1, 1);
    pwm_init (1, 16, 255);
    pwm_duty (200, 800);
    usb_init ();
    lcd_init ();
    lcd_putc ('\f');
    delay_ms (1);
    lcd_refresh ();

    while (TRUE) {
        usb_task ();
        if (usb_enumerated ()) {
            if (usb_kbhit (1)) {
                rx_msg_len = usb_get_packet (1, &rx_msg, sizeof (rx_msg));
                process_usb_data ();
            }
        }
        lcd_refresh ();
        delay_ms (100);
    }
}


void process_usb_data ()
{
    pwm_init (rx_msg.u.enabled, rx_msg.u.t2_divisor,
              rx_msg.u.t2_period);
    pwm_duty (rx_msg.u.pwm1_duty, rx_msg.u.pwm2_duty);
    lcd_refresh ();
}


void lcd_refresh ()
{
    lcd_gotoxy (1, 1);
    printf (lcd_putc, "U=%d P=%d D=%d I=%d",
            usb_enumerated (), pwm_enabled, t2_divisor, t2_period);

    lcd_gotoxy (1, 2);
    printf (lcd_putc, "P1=%Ld%% P2=%Ld%%", pwm1_duty / 10, pwm2_duty / 10);
}


void pwm_init (int1 enabled, int8 divisor, int8 period)
{
    set_tris_c (0);             /* CCP1 is RC2, CCP2 is RC1 */
    pwm_enabled = enabled;
    if (enabled) {
        setup_ccp1 (CCP_PWM);
        setup_ccp2 (CCP_PWM);
        t2_divisor = divisor;
        t2_period = period;
        switch (divisor) {
        case 1:
            setup_timer_2 (T2_DIV_BY_1, period, 1);
            break;
        case 4:
            setup_timer_2 (T2_DIV_BY_4, period, 1);
            break;
        case 16:
            setup_timer_2 (T2_DIV_BY_16, period, 1);
            break;
        }
        
        pwm1_duty = pwm2_duty = 0;
    }
    else {
        setup_ccp1 (CCP_OFF);
        setup_ccp2 (CCP_OFF);
    }
}


void pwm_duty (int16 c1, int16 c2)
{
    pwm1_duty = c1;
    set_pwm1_duty (c1);
    pwm2_duty = c2;
    set_pwm2_duty (c2);
}
