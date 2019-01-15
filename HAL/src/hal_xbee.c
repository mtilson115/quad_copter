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
#include "type_defs.h"
#include "bsp.h"
#include <p32xxxx.h>
#include <os.h>

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
        .baud = 7,              // Exactly 5MHz
        .interrupt_prio = 4,
        .interrupt_sub_prio = 1,
        .data_width = SPI_8BIT,
        .use_interrupts = FALSE,
    };
    SPI_init( SPI2, spi_settings );
    SPI_enable( SPI2, TRUE );

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
    TRISEbits.TRISE9 = 1;

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
 *              2. Holds the DOUT low (UART_RX pin)
 *              3. Releases reset
 *
 ******************************************************************************/
static void hal_xbee_spi_en_seq( void )
{
    /*
     * Configure the pins as outputs
     */
    // Configure reset pin as digital (only necessary becasue it is duplexed with
    // an analog pin)
    AD1PCFGbits.PCFG3 = 1;

    // Configure the reset pin as an output
    TRISBbits.TRISB3 = 0;

    // Configure reset pin as CMOS
    ODCBbits.ODCB3 = 0;

    // Configure U1RX (RF2) as output
    TRISFbits.TRISF2 = 0;

    // Configure U1RX (RF2) pin as CMOS
    ODCFbits.ODCF2 = 0;

    // Delay just to be safe.  Probably not necessary
    BSP_Dly(2);

    /*
     * Place the xbee in reset for 100ms while also holding
     * DOUT low
     */
    PORTBbits.RB3 = 0;
    PORTFbits.RF2 = 0;
    OS_ERR err;
    OSTimeDlyHMSM(0u, 0u, 0u, 100u,OS_OPT_TIME_HMSM_STRICT,&err);

    /*
     * Release reset
     */
    PORTBbits.RB3 = 1;
}

// Temp function for testing.  It is called using the default interrupt function
void HAL_xbee_test( void )
{
    uint8_t data = 0x00;
    uint32_t cksum = 0;
    SPI_write( SPI2, &data, sizeof(data) );
    // Check for start of frame
    if( data == 0x7E )
    {
        uint8_t MSB_len = 0x00;
        SPI_write( SPI2, &MSB_len, sizeof(MSB_len) );
        uint8_t LSB_len = 0x00;
        SPI_write( SPI2, &LSB_len, sizeof(LSB_len) );
        uint16_t MSG_len = LSB_len | (MSB_len << 8);
        // MSG_len is the data length in the frame excluding the
        // checksum
        for( uint32_t msg_idx = 0; msg_idx < MSG_len; msg_idx++ )
        {
            data = 0x00;
            SPI_write( SPI2, &data, sizeof(data) );
            cksum += data;
        }
        cksum &= 0xFF;
        cksum = 0xFF - cksum;
        uint8_t xbee_cksum = 0;
        SPI_write( SPI2, &xbee_cksum, sizeof(data) );
        if( xbee_cksum == cksum )
        {
            xbee_cksum = 0;
        }
    }
}
