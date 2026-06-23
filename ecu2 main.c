#include "ecu2_sensor.h"
#include "adc.h"
#include "can.h"
#include "msg_id.h"
#include <xc.h>

void init_config(void)
{
    init_adc();
    init_can();
    init_digital_keypad();
  
}

int main()
{
    init_config();
    while(1)
    {
       unsigned int rpm =  get_rpm();
       char rpm_str[5];
       rpm_str[0] = (rpm / 1000) % 10 + '0';
       rpm_str[1] = (rpm / 100) % 10 + '0';
       rpm_str[2] = (rpm / 10) % 10 + '0';
       rpm_str[3] = (rpm % 10) + '0';
       rpm_str[4] = '\0';
       
       can_transmit(RPM_MSG_ID,rpm_str,4);
       for (int wait = 1000; wait >0; wait--);
        char gear = get_gear_pos();
        can_transmit(GEAR_MSG_ID,&gear,1);
        for(int i = 1000;i > 0;i--);
    }
}
