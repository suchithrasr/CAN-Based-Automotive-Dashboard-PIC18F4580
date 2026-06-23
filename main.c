
/* File:   main.c
 * Author: Suchithra S
 *
 * Created on 24 April, 2026, 10:11 AM
 */


#include <xc.h>
#include <stdint.h>
#include "can.h"
#include "clcd.h"
#include "msg_id.h"
#include "message_handler.h"
#include "timer0.h"

static void init_leds(void) {
    /* Configure PORTB: RB3 as input (CAN RX), all others as output for LEDs */
    TRISB = 0x08;

    /* Turn off all LEDs at startup */
    PORTB = 0x00;
}

static void init_config(void) {
    /* Initialize CLCD display for dashboard output */
    init_clcd();

    /* Initialize CAN module to receive data from ECU1 */
    init_can();

    /* Initialize indicator LED output pins */
    init_leds();

    /* Enable peripheral interrupts for CAN and Timer */
    PEIE = 1;

    /* Enable global interrupts to allow ISR execution */
    GIE = 1;

    /* Initialize Timer0 for indicator LED blink timing */
    init_timer0();
}

void main(void) {
    /* Initialize all peripherals before entering the main loop */
    init_config();

    /* Print static dashboard header row on CLCD line 1 */
    clcd_print((const unsigned char*)"SPD IND RPM  GR", LINE1(0));

    /* Continuously process incoming CAN messages and update the display */
    while (1) {
        process_canbus_data();
    }

    return;
}

