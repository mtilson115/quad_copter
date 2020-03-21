/*******************************************************************************
 * File:    driver_uart.h
 * Author:  Mitchell S. Tilson
 * Created: 02/20/2014
 *
 * Description: This file provides a driver interface to the UART periperial.
 ******************************************************************************/
#pragma once
#ifndef DRIVER_UART_H
#define	DRIVER_UART_H

#ifdef	__cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "type_defs.h"
#include "perif_sfr_map.h"
#include <stdbool.h>

/*******************************************************************************
 * Defines
 ******************************************************************************/
#define UART_PERIF_NUM 4

/*******************************************************************************
 * Type Defs
 ******************************************************************************/
typedef struct {
    bool        enabled;
    int32_t     baud;
    int32_t     data_bits;
    int32_t     parity;
    int32_t     stop_bits;
} UART_ctrl_type;

typedef struct
{
    volatile REG_SET_PTRs   mode;
    volatile REG_SET_PTRs   sta;
    volatile uint32_t*      tx;
    volatile uint32_t*      rx;
    volatile uint32_t*      brg;
}UART_REGS;

/*******************************************************************************
 * Enumerations
 ******************************************************************************/

typedef enum {
    UART_1 = 0,
    UART_2,
    UART_3,
    UART_4,
    UART_SIZE,
} UART_ID;

typedef enum {
    PARITY_EVEN,
    PARITY_ODD,
    PARITY_NONE,
} PARITY_type;

/*******************************************************************************
 * Public Data
 ******************************************************************************/
UART_REGS const UART[UART_PERIF_NUM];

/*******************************************************************************
 * Public Functions
 ******************************************************************************/

UART_ID Uart_init( UART_ID id, int32_t baud, int32_t data_bits, PARITY_type parity, int32_t stop_bits );

void Uart_enable( UART_ID id );

void Uart_disable( UART_ID id );

int32_t Uart_read( UART_ID id, int32_t byte_count, uint8_t* buffer, bool block );

int32_t Uart_write( UART_ID id, int32_t byte_count, uint8_t* buffer );

void Uart_check_baud( UART_ID id );


#ifdef	__cplusplus
}
#endif

#endif	/* DRIVER_UART_H */

