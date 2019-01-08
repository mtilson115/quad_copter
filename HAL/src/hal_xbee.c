/*******************************************************************************
 * File:    hal_xbee.c
 * Author:  Mitchell S. Tilson
 * Created: 01/07/2019
 *
 * Description: This file contains the hardware abstraction layer for the XBEE
 *              module.
 ******************************************************************************/

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "hal_xbee.h"
#include "driver_spi.h"
#include <p32xxxx.h>

/*******************************************************************************
 * Local static function definitions
 ******************************************************************************/
static void hal_xbee_int_init( void );
static void hal_xbee_spi_en_seq( void );

/*******************************************************************************
 * Public function section
 ******************************************************************************/

/*******************************************************************************
 * HAL_xbee_init
 *
 * Description: Initialzies the xbee module.
 *
 *
 * Inputs:      none
 *
 * Returns:     ret_t - rSUCCESS
 *
 * Revision:    Initial Creation 01/07/2019 - Mitchell S. Tilson
 *
 * Notes:       This function does the following:
 *              1. Inits the INT2 interrupt (SPI Attn)
 *              2. Enables the SPI
 *              3. Places the XBEE in reset
 *              4. Holds the DOUT low
 *              5. Releases reset
 *
 ******************************************************************************/
void HAL_xbee_init( void )
{
    // The INT2 interrupt should be enabled after this function is called
    hal_xbee_int_init();

    // Initialize the SPI
    spi_init_t spi_settings = {
        .baud = 7;              // Exactly 5MHz
        .interrupt_prio = 4;
        .interrupt_sub_prio = 1;
        .data_width = SPI_8BIT;
        .use_interrupts = FALSE;
    };
    SPI_init( SPI2, spi_settings );

    // Performs SPI enable sequence
    hal_xbee_spi_en_seq();
}

/*******************************************************************************
 * HAL_xbee_int_init
 *
 * Description: Initializes the SPI attn interrupt
 *
 *
 * Inputs:      none
 *
 * Returns:     ret_t - rSUCCESS
 *
 * Revision:    Initial Creation 01/07/2019 - Mitchell S. Tilson
 *
 * Notes:       This function does the following:
 *              1. Registers an interrupt callback function with the OS for the SPI_ATTN line on the XBEE
 *              2. Places the XBEE in reset
 *              3. Holds the DOUT low
 *              4. Releases reset
 *
 ******************************************************************************/
static void hal_xbee_int_init( void )
{
    // SPI_ATTN is on pin 9 of port E
    // This configures it as an input
    TRISEbits.TRISD9 = 1;

    // INT2 enable bit (disable the interrupt)
    IEC0bits.INT2IE = 0;

    // Set the interrupt polarity for falling edge
    INTCONbits.INT2EP = 0;

    // Clear the interrupt status flag
    IFS0bits.INT2IF = 0;

    // Interrupt priority
    IPC2bits.INT2IP = 4;
    // Interrupt sub-priority
    IPC2bits.INT2IS = 1;

    // INT2 enable bit
    IEC0bits.INT2IE = 1;
}

/*******************************************************************************
 * hal_xbee_spi_en_seq
 *
 * Description: Configures the Xbee to use SPI
 *
 *
 * Inputs:      none
 *
 * Returns:     ret_t - rSUCCESS
 *
 * Revision:    Initial Creation 01/07/2019 - Mitchell S. Tilson
 *
 * Notes:       This function does the following:
 *              1. Places the XBEE in reset
 *              2. Holds the DOUT low
 *              3. Releases reset
 *
 ******************************************************************************/
static void hal_xbee_spi_en_seq( void )
{
    // Configure the 

// Temp function for testing.  It is called using the default interrupt function
