#ifndef ECU1_SENSOR_H
#define ECU1_SENSOR_H

#include <xc.h>
#include<stdint.h>
#include "digital_keypad.h"
#include "msg_id.h"

typedef enum {
    e_ind_off,
    e_ind_left,
    e_ind_right,
    e_ind_hazard
} IndicatorStatus;

extern volatile IndicatorStatus prev_ind_status, cur_ind_status;
extern volatile unsigned char led_state;

/* Read speed sensor, convert to km/h string, and transmit over CAN */
void get_speed();
/* Read indicator keypad switch and transmit current indicator status over CAN */
void process_indicator();

#endif

