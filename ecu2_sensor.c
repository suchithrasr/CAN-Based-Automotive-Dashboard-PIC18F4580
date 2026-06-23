
#include "ecu2_sensor.h"
#include "adc.h"
#include "can.h"
#include "msg_id.h"
#include <stdio.h>
#include <string.h>

static unsigned char gear_index = 0;
static const char gear_name[8][2] = {"N","1","2","3","4","5","R","C"};
unsigned char key;
unsigned int rpm;

uint16_t get_rpm(void)
{
    rpm = (read_adc(CHANNEL4)/10.23)*60;
    return rpm;
//    char rpm_str[8];
//    sprintf(rpm_str,"%u",rpm);
//    can_transmit(RPM_MSG_ID,(const uint8_t *)rpm_str, (uint8_t)strlen(rpm_str));
//    for(int i = 100;i > 0;i--);
}

unsigned char get_gear_pos()
{
     key = read_digital_keypad(STATE_CHANGE);

        if(key == SWITCH1)   
        {
            if(gear_index==7)
                gear_index=0;
            else if(gear_index <6)
            {
                gear_index++;
            }
        }
        else if(key == SWITCH2)  
        {
            if(gear_index==7)
                gear_index=0;
            else if(gear_index > 0)
            {
              gear_index--;
            }
        }
        else if(key == SWITCH3)  
        {
            gear_index = 7;   
        }
 
    return gear_index;
       
}

void ecu2_sensor(void)
{
       rpm =  get_rpm();
       char rpm_str[5];
       rpm_str[0] = (rpm / 1000) % 10 + '0';
       rpm_str[1] = (rpm / 100) % 10 + '0';
       rpm_str[2] = (rpm / 10) % 10 + '0';
       rpm_str[3] = (rpm % 10) + '0';
       rpm_str[4] = '\0';
       can_transmit(RPM_MSG_ID,rpm_str,4);
       for(int i = 1000;i > 0;i--);
       
       char gear = get_gear_pos();
       can_transmit(GEAR_MSG_ID,&gear,1);
       for(int i = 1000;i > 0;i--);
}