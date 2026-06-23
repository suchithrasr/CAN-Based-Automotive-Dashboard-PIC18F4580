#include <xc.h>
#include "timer0.h"
#include "message_handler.h"

/* Counts Timer0 overflows to generate the indicator LED blink period */
static uint32_t timer_count = 0;

void __interrupt() isr(void)
{
    /* Check if Timer0 overflow interrupt has fired */
    if(TMR0IF == 1)
    {
        /* Clear Timer0 interrupt flag to allow the next interrupt */
        TMR0IF = 0;

        /* Reload Timer0 preload value to maintain consistent overflow interval */
        TMR0 = 6;

        /* If LED state is OFF, turn both indicator LEDs off and reset counter */
        if(led_state == LED_OFF)
        {
            PORTBbits.RB0 = LED_OFF;  /* Turn off left indicator LED */
            PORTBbits.RB7 = LED_OFF;  /* Turn off right indicator LED */
            timer_count = 0;          /* Reset blink counter when LEDs are disabled */
            return;
        }

        /* Increment overflow count toward the next blink toggle point */
        timer_count++;

        /* Check if enough overflows have elapsed for one blink half-period */
        if(timer_count >= 20000)
        {
            /* Reset counter to start the next blink half-period */
            timer_count = 0;

            /* Control LEDs based on the current indicator status */
            switch(status)
            {
                case e_ind_off:
                    /* Both indicators off: keep both LEDs off */
                    PORTBbits.RB0 = LED_OFF;
                    PORTBbits.RB7 = LED_OFF;
                    break;

                case e_ind_left:
                    /* Left indicator: toggle left LED, keep right LED off */
                    PORTBbits.RB0 = !PORTBbits.RB0;
                    PORTBbits.RB7 = LED_OFF;
                    break;

                case e_ind_right:
                    /* Right indicator: keep left LED off, toggle right LED */
                    PORTBbits.RB0 = LED_OFF;
                    PORTBbits.RB7 = !PORTBbits.RB7;
                    break;

                case e_ind_hazard:
                    /* Hazard mode: toggle both LEDs simultaneously */
                    PORTBbits.RB0 = !PORTBbits.RB0;
                    PORTBbits.RB7 = !PORTBbits.RB7;
                    break;
            }
        }
    }
}
