/*
 * File:   main.c
 * Author: Suchithra S
 *
 * Created on 17 April, 2026, 9:38 AM
 */

#include <xc.h>
#include "ecu1_sensor.h"
#include "adc.h"
#include "can.h"
#include "digital_keypad.h"
//#include "msg_id.h"

void init_config(void)
{
    /* Initialize Digital Keypad */
    init_digital_keypad();

    /* Initialize ADC */
    init_adc();

    /* Initialize CAN */
    init_can();
}
void main(void) {
    
    init_config();
    while(1)
    {
        get_speed();
        process_indicator();
       // __delay_ms(100);
    }
    return;
}
