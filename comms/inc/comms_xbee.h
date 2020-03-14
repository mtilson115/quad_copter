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

// RX Messages
#define COMMS_SET_THROTTLE              (0x01)
#define COMMS_SET_THROTTLE_SIZE         (5)
#define COMMS_SET_PID                   (0x02)
#define COMMS_SET_PID_SIZE              (25)
#define COMMS_CALIBRATE                 (0x03)
#define COMMS_CALIBRATE_SIZE            (1)
#define COMMS_PITCH_ROLL                (0x04)
#define COMMS_PITCH_ROLL_SIZE           (13)
#define COMMS_DEBUG_M_PR                (0x05)
#define COMMS_DEBUG_M_PR_SIZE           (2)
#define COMMS_MOTOR_CAL                 (0x06)
#define COMMS_MOTOR_CAL_SIZE            (15)
#define COMMS_ERASE_CAL                 (0x07)
#define COMMS_ERASE_CAL_SIZE            (1)
#define COMMS_SAVE_MOTOR_CAL            (0x08)
#define COMMS_SAVE_MOTOR_CAL_SIZE       (1)
#define COMMS_SET_COMP_FILT_CONST       (0x09)
#define COMMS_SET_COMP_FILT_CONST_SIZE  (5)
#define COMMS_GET_PID                   (0x0A)
#define COMMS_GET_PID_SIZE              (1)

// TX Messages
#define COMMS_DBG_HDR_MOTOR_PITCH_ROLL  (17)
#define COMMS_BATTERY_VOLTAGE           (20)
#define COMMS_CALIBRATION               (1)
#define COMMS_DBG_HDR_PITCH_ROLL        (21)
#define COMMS_HDR_MOTION6               (22)
#define COMMS_PRINTF                    (23)
#define COMMS_ACK                       (24)
#define COMMS_PID                       (25)

typedef struct __attribute__((packed)) {
    uint16_t    len;
    uint8_t*    data;
}comms_xbee_msg_t;

typedef struct {
    void        (*cb)(uint8_t* data,uint16_t len);
    uint8_t     msg_id;
}comms_xbee_rx_cb_t;

typedef void (*comms_xbee_lost_connection_cb_t)(void);

void COMMS_xbee_send(comms_xbee_msg_t msg);
void COMMS_xbee_init(void);
uint8_t COMMS_xbee_ready( void );
ret_t COMMS_xbee_register_rx_cb( comms_xbee_rx_cb_t cb_data );
ret_t COMMS_xbee_register_lost_connection_cb( comms_xbee_lost_connection_cb_t cb_func );
uint32_t COMMS_xbee_get_fault( void );
void COMMS_xbee_send_ack(uint32_t rx_id);

/*
 * Debug data
 */
OS_TCB* COMMS_xbee_get_tcb( void );
extern CPU_STK comms_xbee_stack[COMMS_XBEE_STK_SIZE];

#ifdef	__cplusplus
}
#endif

#endif
