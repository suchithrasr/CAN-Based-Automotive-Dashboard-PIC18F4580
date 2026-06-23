/*
 * File:   ecu1_sensor.c
 * Author: Suchithra S
 *
 * Created on 24 April, 2026, 10:29 AM
 */

#include "ecu1_sensor.h"
#include "adc.h"
#include "can.h"
#include "msg_id.h"

void get_speed()
{
    // Implement the speed function
    /* Read raw 10-bit ADC value from speed sensor on channel 4 */
    unsigned int adc_reg_val = read_adc(CHANNEL4);
    /* Convert ADC value to speed in km/h (0?1023 mapped to 0?100) */
    unsigned int speed =  (adc_reg_val / 10.23);
   
    char buff[4];
    buff[0] = (speed / 100) + '0';
    buff[1] = (speed / 10 % 10) + '0';
    buff[2] = (speed % 10) + '0';
    buff[3] = '\0';

    /* Transmit speed string over CAN bus to ECU3 for display */
    can_transmit(SPEED_MSG_ID, (const uint8_t *)buff, sizeof(buff));

    for(int wait = 100; wait > 0; wait--);  

    return;
}

void process_indicator()
{
    //Implement the indicator function
   
    /* Read keypad to detect indicator switch press using state-change mode */
    char key = read_digital_keypad(STATE_CHANGE);
    /* Retain the current indicator state across function calls */
    static IndicatorStatus cur_ind_status = e_ind_off;
   
    switch(key)
    {
        case SWITCH1:
            /* SW1 pressed: activate left indicator */
            cur_ind_status = e_ind_left;
            break;
       
        case SWITCH2:
            /* SW2 pressed: activate right indicator */
            cur_ind_status = e_ind_right;
            break;
           
        case SWITCH3:
            /* SW3 pressed: activate hazard (both indicators) */
            cur_ind_status = e_ind_hazard;
            break;
           
        case SWITCH4:
            /* SW4 pressed: turn off all indicators */
            cur_ind_status = e_ind_off;
            break;
    }
   
    uint8_t data[1];
    data[0] = (uint8_t)cur_ind_status;
   
     /* Transmit indicator status over CAN bus to ECU3 */
    can_transmit(INDICATOR_MSG_ID, data, 1);

    for(int wait = 200; wait > 0; wait--);  
   
    return;
}