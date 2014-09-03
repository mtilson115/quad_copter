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

/*******************************************************************************
 * Defines
 ******************************************************************************/
    #define UART_PERIF_NUM 2

/*******************************************************************************
 * Type Defs
 ******************************************************************************/
    typedef struct {
        BOOL    enabled;
        int32_t     baud;
        int32_t     data_bits;
        int32_t     parity;
        int32_t     stop_bits;
    } UART_ctrl_type;

    typedef struct
    {
	volatile REG_SET    mode;
	volatile REG_SET    sta;
	volatile REG_SET    tx;
	volatile REG_SET    rx;
	volatile REG_SET    brg;
    }UART_REGS;

/*******************************************************************************
 * Enumerations
 ******************************************************************************/

    typedef enum {
        UART_1,
        UART_2,
        UART_3,
        UART_4,
        UART_5,
        UART_6,
        UART_INVALID,
    } UART_ID;

    typedef enum {
        PARITY_EVEN,
        PARITY_ODD,
        PARITY_NONE,
    } PARITY_type;

/*******************************************************************************
 * Public Data
 ******************************************************************************/
extern UART_REGS * const UART[UART_PERIF_NUM];

/*******************************************************************************
 * Public Functions
 ******************************************************************************/

    extern UART_ID  Uart_init( UART_ID id, int32_t baud, int32_t data_bits, PARITY_type parity, int32_t stop_bits );

    extern void     Uart_enable( UART_ID id );

    extern void     Uart_disable( UART_ID id );

    extern int32_t      Uart_read( UART_ID id, int32_t byte_count, uint8_t* buffer, BOOL block );

    extern int32_t      Uart_write( UART_ID id, int32_t byte_count, uint8_t* buffer );

    extern void     Uart_check_baud( UART_ID id );


#ifdef	__cplusplus
}
#endif

#endif	/* DRIVER_UART_H */

