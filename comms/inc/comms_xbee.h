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
#include "app_cfg.h"
#include "type_defs.h"

#define COMMS_SET_THROTTLE (0x01)

typedef struct __attribute__((packed)) {
    uint16_t    len;
    uint8_t*    data;
}comms_xbee_msg_t;

typedef struct {
    void        (*cb)(uint8_t* data,uint16_t len);
    uint8_t     msg_id;
}comms_xbee_rx_cb_t;

void COMMS_xbee_send(comms_xbee_msg_t msg);
void COMMS_xbee_init(void);
uint8_t COMMS_xbee_ready( void );
ret_t COMMS_xbee_register_rx_cb( comms_xbee_rx_cb_t cb_data );

/*
 * Debug data
 */
OS_TCB* COMMS_xbee_get_tcb( void );
extern CPU_STK comms_xbee_stack[COMMS_XBEE_STK_SIZE];

#ifdef	__cplusplus
}
#endif

#endif
