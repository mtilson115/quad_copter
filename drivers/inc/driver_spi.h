/*******************************************************************************
 * File:    driver_spi.h
 * Author:  Mitchell S. Tilson
 * Created: 02/15/2016
 *
 * Description: This file provides a driver interface to the SPI periperial in
 *              master mode.
 ******************************************************************************/
#pragma once
#ifndef DRIVER_SPI_H
#define	DRIVER_SPI_H

#ifdef	__cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "type_defs.h"

/*******************************************************************************
 * Enumerations
 ******************************************************************************/
typedef enum {
    SPI1 = 1,
    SPI2 = 2,
    SPI3 = 3,
    SPI4 = 4,
    SPI_INVALID = 5,
} spi_num_e;

typedef enum {
    SPI_SUCCESS,
    SPI_BUFF_FULL,
    SPI_ERROR,
} spi_ret_e;

typedef enum {
    SPI_8BIT = 0,
    SPI_16BIT = 1,
    SPI_32BIT = 2,
} spi_width_e;

/*******************************************************************************
 * Type Defs
 ******************************************************************************/
typedef struct {
    uint32_t baud;
    uint32_t interrupt_prio;
    uint32_t interrupt_sub_prio;
    spi_width_e data_width;  // 8bit, 16bit, 32bit
    bool use_interrupts;
} spi_init_t;

/*******************************************************************************
 * Public Functions
 ******************************************************************************/

void SPI_init( spi_num_e spi, spi_init_t spi_settings );

void SPI_enable( spi_num_e spi, bool enable );

spi_ret_e SPI_write_read( spi_num_e spi, uint8_ua_t* wdata, uint8_ua_t* rdata, uint32_t data_len );

#ifdef	__cplusplus
}
#endif

#endif	/* DRIVER_SPI_H */
