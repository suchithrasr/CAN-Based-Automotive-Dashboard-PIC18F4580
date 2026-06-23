/*
 * File:   can.c
 * Author: Suchithra S
 *
 * Created on 23 April, 2026, 9:55 PM
 */

/* File for CAN configuration, message ID setup, data transmission, and reception */
#include <xc.h>
#include "can.h"

/* Enum defining CAN operation modes for CANCON register */
typedef enum _CanOpMode {
    e_can_op_mode_normal = 0x00,   /* Normal operation mode for live bus communication */
    e_can_op_mode_loop   = 0x40,   /* Loopback mode for internal self-testing */
    e_can_op_mode_config = 0x80    /* Configuration mode for baud rate and filter setup */
} CanOpMode;

void init_can(void) {
    /* Configure RB2 as CAN TX output pin */
    TRISB2 = 0;

    /* Configure RB3 as CAN RX input pin */
    TRISB3 = 1;

    /* Switch CAN module to configuration mode to allow register writes */
    CAN_SET_OPERATION_MODE_NO_WAIT(e_can_op_mode_config);

    /* Wait until CANSTAT confirms configuration mode is active */
    while (CANSTAT != 0x80);

    /* Select ECAN Mode 0 (legacy mode with 2 receive buffers) */
    ECANCON = 0x00;

    /* Set baud rate prescaler: SJW=4TQ, BRP=4 for 8MHz operation */
    BRGCON1 = 0xE1;

    /* Set phase segment 1 to 4TQ, propagation segment to 4TQ, sample once */
    BRGCON2 = 0x1B;

    /* Set phase segment 2 to 4TQ */
    BRGCON3 = 0x03;

    /* Disable all receive filters so all messages are accepted */
    RXFCON0 = 0x00;

    /* Switch CAN module to normal operation mode */
    CAN_SET_OPERATION_MODE_NO_WAIT(e_can_op_mode_normal);

    /* Configure RXB0 to receive all message types */
    RXB0CON = 0x00;

    /* Set RXM bits to receive all messages regardless of filter match */
    RXB0CONbits.RXM0 = 1;
    RXB0CONbits.RXM1 = 1;
}

static uint16_t get_msg_id_std(void) {
    uint16_t id = 0;

    /* Reconstruct 11-bit standard ID from SIDH and SIDL registers */
    id = ((RXB0SIDL >> 5) & 0x7) | (RXB0SIDH << 3);
    return id;
}

static void set_msg_id_std(unsigned int id) {
    /* Write lower 3 bits of ID into SIDL register bits [7:5] */
    TXB0SIDL = (id & 0x7) << 5;

    /* Write upper 8 bits of ID into SIDH register */
    TXB0SIDH = (id >> 3);
}

void can_transmit(uint16_t msg_id, const uint8_t *data, uint8_t len) {
    uint8_t *ptr;

    /* Clear extended identifier registers (using standard frame only) */
    TXB0EIDH = 0x00;
    TXB0EIDL = 0x00;

    /* Load the standard message ID into transmit buffer registers */
    set_msg_id_std(msg_id);

    /* Set the data length code to the number of bytes to send */
    TXB0DLC = len;

    /* Get pointer to the first transmit data byte register */
    ptr = (uint8_t *) & TXB0D0;

    /* Copy each data byte into the transmit buffer sequentially */
    for (int i = 0; i < len; i++) {
        ptr[i] = data[i];
    }

    /* Request transmission by setting the TXREQ bit */
    TXB0REQ = 1;
}

void can_receive(uint16_t *msg_id, uint8_t *data, uint8_t *len) {
    uint8_t *ptr;

    /* Check if RXB0 has a new received message waiting */
    if (RXB0FUL) {
        /* Extract the 11-bit standard message ID from receive registers */
        *msg_id = get_msg_id_std();

        /* Read the number of received data bytes from DLC */
        *len = RXB0DLC;

        /* Get pointer to the first receive data byte register */
        ptr = (uint8_t *) & RXB0D0;

        /* Copy received bytes from hardware buffer to caller's data array */
        for (int i = 0; i < *len; i++) {
            data[i] = ptr[i];
        }

        /* Clear the receive buffer full flag to allow next message */
        RXB0FUL = 0;

        /* Clear the RXB0 interrupt flag */
        RXB0IF = 0;
    }

    /* Indicate no data available when buffer was empty */
    *len = 0;
}
