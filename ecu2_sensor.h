/* 
 * File:   ecu1_sensor.h
 * Author: Emertxe
 *
 * Created on March 20, 2025, 6:14 PM
 */
#ifndef ECU2_SENSOR_H
#define	ECU2_SENSOR_H

#include <stdint.h>
#include "digital_keypad.h"
#include <xc.h>

#define RPM_ADC_CHANNEL 0x04
#define GEAR_UP             SWITCH1
#define GEAR_DOWN           SWITCH2
#define COLLISION           SWITCH3
#define MAX_GEAR 6
//#define ENG_TEMP_ADC_CHANNEL 0x06
//
//#define LED_OFF 0
//#define LED_ON 1
//
//#define RIGHT_IND_ON() (PORTB = PORTB | 0xC0)
//#define RIGHT_IND_OFF() (PORTB = PORTB & ~0xC0)
//#define LEFT_IND_ON() (PORTB = PORTB | 0x03)
//#define LEFT_IND_OFF() (PORTB = PORTB & ~0x03)

//typedef enum {
//    e_ind_off,
//    e_ind_left,
//    e_ind_right
//} IndicatorStatus;
//
//extern volatile IndicatorStatus prev_ind_status, cur_ind_status;
//extern volatile unsigned char led_state;

uint16_t get_rpm(void);
unsigned char get_gear_pos(void);
void ecu2_sensor(void);

#endif	/* ECU1_SENSOR_H */

