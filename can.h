#ifndef CAN_H
#define CAN_H

#include <stdint.h>

/* ECAN mode and FIFO buffer full status access macros */
#define   F_ECAN_MODE2_FP		CANCON & 0x0F          /* Read current ECAN operation mode bits */
#define   F_ECANFIFO_0			RXB0CONbits.RXFUL      /* Check if receive buffer 0 is full */
#define   F_ECANFIFO_1			RXB1CONbits.RXFUL      /* Check if receive buffer 1 is full */
#define   F_ECANFIFO_2			B0CONbits.RXFUL        /* Check if programmable buffer 0 is full */
#define   F_ECANFIFO_3			B1CONbits.RXFUL        /* Check if programmable buffer 1 is full */
#define   F_ECANFIFO_4			B2CONbits.RXFUL        /* Check if programmable buffer 2 is full */
#define   F_ECANFIFO_5			B3CONbits.RXFUL        /* Check if programmable buffer 3 is full */
#define   F_ECANFIFO_6			B4CONbits.RXFUL        /* Check if programmable buffer 4 is full */
#define   F_ECANFIFO_7			B5CONbits.RXFUL        /* Check if programmable buffer 5 is full */

/* Set CAN operation mode without waiting; clears upper 3 bits then ORs in new mode */
#define CAN_SET_OPERATION_MODE_NO_WAIT(mode)		\
{													\
	CANCON &= 0x1F;									\
	CANCON |= mode;									\
}

/* Byte index offsets for accessing CAN message buffer fields */
#define EIDH					0    /* Extended ID high byte offset */
#define EIDL					1    /* Extended ID low byte offset */
#define SIDH					2    /* Standard ID high byte offset */
#define SIDL					3    /* Standard ID low byte offset */
#define DLC						4    /* Data length code byte offset */
#define D0						5    /* Data byte 0 offset */
#define D1						6    /* Data byte 1 offset */
#define D2						7    /* Data byte 2 offset */
#define D3						8    /* Data byte 3 offset */
#define D4						9    /* Data byte 4 offset */
#define D5						10   /* Data byte 5 offset */
#define D6						11   /* Data byte 6 offset */
#define D7						12   /* Data byte 7 offset */

/* Initialize CAN module with baud rate, filters, and pin configuration */
void init_can(void);

/* Transmit a CAN frame with the given message ID, data buffer, and byte count */
void can_transmit(uint16_t msg_id, const uint8_t *data, uint8_t len);

/* Receive a CAN frame and populate message ID, data buffer, and byte count */
void can_receive(uint16_t *msg_id, uint8_t *data, uint8_t *len);

#endif
