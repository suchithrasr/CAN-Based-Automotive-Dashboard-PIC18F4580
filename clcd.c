/*
 * File:   clcd.c
 * Author: Suchithra.S
 *
 * Created on 24 April, 2026, 10:11 AM
 */

#include <xc.h>
#include "clcd.h"

void clcd_write(unsigned char byte, unsigned char control_bit)
{
    /* Set RS pin to select between instruction (0) or data (1) register */
    CLCD_RS = control_bit;

    /* Place the byte on the data port for the CLCD to read */
    CLCD_PORT = byte;

    /* Pulse EN high then low to latch the data into the CLCD (min 200ns) */
    CLCD_EN = HI;
    CLCD_EN = LO;

    /* Switch data port to input to read the busy flag from CLCD */
    PORT_DIR = INPUT;

    /* Set RW high to indicate a read operation for busy flag check */
    CLCD_RW = HI;

    /* Point RS to instruction register to read the busy flag status bit */
    CLCD_RS = INSTRUCTION_COMMAND;

    
    do
    {
        CLCD_EN = HI;
        CLCD_EN = LO;
    } while (CLCD_BUSY);

    /* Restore RW to write mode after busy flag clears */
    CLCD_RW = LO;

    /* Restore data port direction to output for the next write */
    PORT_DIR = OUTPUT;
}

void init_clcd()
{
    /* Configure Port D as output to drive the 8-bit CLCD data bus */
    TRISD = 0x00;

    /* Configure lower 3 bits of Port C as output for RS, RW, EN control lines */
    TRISC = TRISC & 0xF8;

    /* Set RW low to default to write mode */
    CLCD_RW = LO;

    /* Send cursor home command to reset display cursor position */
    CURSOR_HOME;

    /* Configure CLCD for 2-line, 5x8 dot matrix, 8-bit interface mode */
    TWO_LINE_5x8_MATRIX_8_BIT;

    /* Turn display on with cursor hidden */
    DISP_ON_AND_CURSOR_OFF;

    /* Clear the entire display screen */
    CLEAR_DISP_SCREEN;
}

void clcd_print(const unsigned char *data, unsigned char addr)
{
	clcd_write(addr, INSTRUCTION_COMMAND);
	while (*data != '\0')
	{
		clcd_write(*data++, DATA_COMMAND);
	}
}

void clcd_putch(const unsigned char data, unsigned char addr)
{
	clcd_write(addr, INSTRUCTION_COMMAND);
	clcd_write(data, DATA_COMMAND);
}