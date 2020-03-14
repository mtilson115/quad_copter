/*******************************************************************************
 * File:    bsp_xbee.h
 * Author:  Mitchell S. Tilson
 * Created: 08/09/2019
 *
 * Description: This file provides the bsp_xbee.h API
 ******************************************************************************/
#pragma once
#ifndef BSP_XBEE_H
#define	BSP_XBEE_H

#ifdef	__cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <os.h>
#include <stdint.h>

void bsp_xbee_init(void);
void bsp_xbee_register_tcb(OS_TCB* tcb);
void bsp_xbee_write_read(uint8_t* wdata, uint8_t* rdata, uint16_t len);
void bsp_xbee_int_handler(void);
void bsp_xbee_reset( void );

#ifdef	__cplusplus
}
#endif
#endif  /* BSP_XBEE_H */
