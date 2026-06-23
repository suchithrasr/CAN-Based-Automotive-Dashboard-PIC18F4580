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

void init_config(void)
{
    /* Initialize keypad input pins for indicator switch detection */
    init_digital_keypad();

    /* Initialize ADC module for speed sensor readings */
    init_adc();

    /* Initialize CAN module for ECU1 to ECU3 data transmission */
    init_can();
}

void main(void) {
    
    init_config();

    while(1)
    {
        /* Read speed sensor and transmit value over CAN to ECU3 */
        get_speed();

        /* Read indicator switches and transmit status over CAN to ECU3 */
        process_indicator();
    }

    return;
}
