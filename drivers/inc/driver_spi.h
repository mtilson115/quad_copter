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
    BOOL use_interrupts;
} spi_init_t;

typedef void (*spi_call_back_t)( uint8_t* spi_data, uint32_t data_len );

/*******************************************************************************
 * Public Functions
 ******************************************************************************/

/*******************************************************************************
 * SPI1_init
 *
 * Description: Initializes the SPI identified by the spi_num_e using the passed
 *              baud rate.
 *
 * Inputs:      spi_num_e spi - the spi to initialize
 *              uint32_t baud - the SPIxBRG value to intialize the SPI to.
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 02/15/2016 - Mitchell S. Tilson
 *
 * Notes:
 *
 ******************************************************************************/
void SPI_init( spi_num_e spi, spi_init_t spi_settings );

/*******************************************************************************
 * SPI_enable
 *
 * Description: Enables/Disables the selected SPI
 *
 * Inputs:      spi_num_e spi - the spi to enable/disable
 *              BOOL enable - TRUE to enable FALSE to disable
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 02/15/2016 - Mitchell S. Tilson
 *
 * Notes:
 *
 ******************************************************************************/
void SPI_enable( spi_num_e spi, BOOL enable );

/*******************************************************************************
 * SPI_write
 *
 * Description: Writes to the selected SPI
 *
 * Inputs:      spi_num_e spi - the spi to write data to.
 *              uint8_t* data - a pointer to the data to send
 *              uint32_t data_len - the number of bytes to send
 *
 * Returns:
 *
 * Revision:    Initial Creation 02/15/2016 - Mitchell S. Tilson
 *
 * Notes:
 *
 ******************************************************************************/
spi_ret_e SPI_write( spi_num_e spi, uint8_t* data, uint32_t data_len );

/*******************************************************************************
 * SPI_register_read_call_back
 *
 * Description: Registers a call back to be called when there is data available.
 *
 * Inputs:      spi_call_back_t spi_read_call_back_func - a call back function
 *
 * Returns:     BOOL TRUE/FALSE - TRUE if the registration worked false otherwise.
 *
 * Revision:    Initial Creation 02/15/2016 - Mitchell S. Tilson
 *
 * Notes:       Only 1 call back is allowed per peripheral.
 *
 ******************************************************************************/
// BOOL SPI_register_read_call_back( spi_call_back_t* spi_read_call_back_func );

#ifdef	__cplusplus
}
#endif

#endif	/* DRIVER_SPI_H */
