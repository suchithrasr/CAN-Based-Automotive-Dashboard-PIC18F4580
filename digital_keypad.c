/*
 * File:   digital_keypad.c
 * Author: Suchitra S
 *
 * Created on 24 April, 2026, 10:29 AM
 */


#include <xc.h>
#include "digital_keypad.h"

void init_digital_keypad(void)
{
    /* Configure lower nibble of PORTC as inputs for keypad switches */
	TRISC = TRISC | INPUT_PINS;
}

unsigned char read_digital_keypad(unsigned char detection_type)
{
	static unsigned char once = 1;

	if (detection_type == STATE_CHANGE)
	{
		if (((KEY_PORT & INPUT_PINS) != ALL_RELEASED) && once)
		{
            /* Lock out further detections until key is released */
			once = 0;
            /* Return the active key state from port pins */
			return (KEY_PORT & INPUT_PINS);
		}
        /* Re-enable detection once all keys are released */
		else if ((KEY_PORT & INPUT_PINS) == ALL_RELEASED)
		{
			once = 1;
		}
	}
	else if (detection_type == LEVEL)
	{
		return (KEY_PORT & INPUT_PINS);
	}
    /* Return 0xFF to indicate no valid key event occurred */
	return 0xFF;
}
