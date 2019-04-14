/*******************************************************************************
 * File:    bsp_xbee.h
 * Author:  Mitchell S. Tilson
 * Created: 01/17/2019
 *
 * Description: This file contains the hardware abstraction layer for the XBEE
 *              module.
 ******************************************************************************/
#pragma once
#ifndef BSP_XBEE_H
#define	BSP_XBEE_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <os.h>
#include <stdint.h>

void BSP_xbee_init(void);
void BSP_xbee_register_tcb(OS_TCB* tcb);
void BSP_xbee_write_read(uint8_t* wdata, uint8_t* rdata, uint16_t len);
void BSP_xbee_test(void);
void BSP_xbee_int_handler(void);
void BSP_xbee_reset( void );

#ifdef	__cplusplus
}
#endif

#endif
