#include <xc.h>
#include <string.h>
#include "message_handler.h"
#include "msg_id.h"
#include "can.h"
#include "clcd.h"

/* Global LED state and indicator status shared with the Timer0 ISR */
volatile unsigned char led_state = LED_OFF, status = e_ind_off;

void handle_speed_data(uint8_t *data, uint8_t len) {
    /* Display received speed string at the start of CLCD line 2 */
    clcd_print(data, LINE2(0));
}

void handle_gear_data(uint8_t *data, uint8_t len) {
    //Implement the gear function

    switch (data[0]) {
        case 0: clcd_print((const unsigned char *) "  N ", LINE2(12));
            break;
        case 1: clcd_print((const unsigned char *) "  1 ", LINE2(12));
            break;
        case 2: clcd_print((const unsigned char *) "  2 ", LINE2(12));
            break;
        case 3: clcd_print((const unsigned char *) "  3 ", LINE2(12));
            break;
        case 4: clcd_print((const unsigned char *) "  4 ", LINE2(12));
            break;
        case 5: clcd_print((const unsigned char *) "  5 ", LINE2(12));
            break;
        case 6: clcd_print((const unsigned char *) "  R ", LINE2(12));
            break;
        case 7: clcd_print((const unsigned char *) "  C ", LINE2(12));
            break;
            //        default:
            //            clcd_print((const unsigned char *)data,LINE2(14));
    }

}

void handle_rpm_data(uint8_t *data, uint8_t len) {
    //Implement the rpm function
    /* Display received RPM string at column 8 of CLCD line 2 */
    clcd_print(data, LINE2(8));
}

void handle_indicator_data(uint8_t *data, uint8_t len) {
    //Implement the indicator function

    switch (data[0]) {
        case 0:
             /* Indicator off: disable LEDs and show OFF label */
            status = e_ind_off;
            led_state = LED_OFF;
            clcd_print("OFF ", LINE2(4));
            break;
        case 1:
            /* Left indicator: enable LEDs and show L label */
            status = e_ind_left;
            led_state = LED_ON;
            clcd_print(" L  ", LINE2(4));
            break;
        case 2:
            /* Right indicator: enable LEDs and show R label */
            status = e_ind_right;
            led_state = LED_ON;
            clcd_print(" R  ", LINE2(4));
            break;
        case 3:
            /* Hazard mode: enable LEDs and show H label */
            status = e_ind_hazard;
            led_state = LED_ON;
            clcd_print(" H  ", LINE2(4));
            break;
    }
}

void process_canbus_data() {
//    process the CAN bus data

    uint16_t msg_id;
    uint8_t data[8];
    uint8_t len;
 
   /* receive a CAN frame from the bus */
    can_receive(&msg_id, data, &len);

    /* Route the received message to its handler based on message ID */
    switch (msg_id) {
        case SPEED_MSG_ID:
            /* Forward speed data to the speed display handler */
            handle_speed_data(data, len);
            break;
        case INDICATOR_MSG_ID:
            /* Forward indicator data to the indicator display and LED handler */
            handle_indicator_data(data, len);
            break;
        case RPM_MSG_ID:
             /* Forward RPM data to the RPM display handler */
            handle_rpm_data(data, len);
            break;
        case GEAR_MSG_ID:
            /* Forward gear data to the gear display handler */
            handle_gear_data(data, len);
            break;
    }

    return;

}