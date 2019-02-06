/*******************************************************************************
 * File:    comms_xbee.h
 * Author:  Mitchell S. Tilson
 * Created: 01/19/2019
 *
 * Description: This file contains communication layer for the xbee
 *
 ******************************************************************************/
#pragma once
#ifndef COMMS_XBEE_H
#define	COMMS_XBEE_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <os.h>

typedef struct __attribute__((packed)) {
    uint16_t    len;
    uint8_t*    data;
}comms_xbee_msg_t;

void COMMS_xbee_send(comms_xbee_msg_t msg);
void COMMS_xbee_init(void);
OS_TCB* COMMS_xbee_get_tcb( void );
uint8_t COMMS_xbee_ready( void );

#ifdef	__cplusplus
}
#endif

#endif
