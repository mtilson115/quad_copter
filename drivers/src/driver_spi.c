/*******************************************************************************
 * File:    driver_spi.c
 * Author:  Mitchell S. Tilson
 * Created: 02/15/2016
 *
 * Description: This file contians the driver interface for the SPI module in
 *              in master mode.
 ******************************************************************************/

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "type_defs.h"
#include "driver_spi.h"
#include "perif_sfr_map.h"
#include <p32xxxx.h>

/*******************************************************************************
 * Local Defines
 ******************************************************************************/

/*******************************************************************************
 * Local Type Defs
 ******************************************************************************/
typedef struct {
    volatile __SPI1CONbits_t* SPIxCON; // SPI control register
    volatile uint32_t* SPIxBUFF; // SPI buffer register
    volatile uint32_t* SPIxBRG; // SPI baud rate register
    volatile __SPI1STATbits_t* SPIxSTAT; // SPI status register
    spi_num_e spi; // The SPI bus number
    void (*spi_int_en_func)( BOOL enable);
    void (*spi_int_prio_func)( uint32_t prio );
    void (*spi_int_clear_flags_func)( void );
    BOOL initialized;
} spi_config_t;

volatile __IEC0bits_t* IECx; // interrupt enable register
volatile __IPC5bits_t* IPCx; // interrupt priority register
&IEC0bits,
&IPC5bits,

/*******************************************************************************
 * Local Functions
 ******************************************************************************/
void spi1_int_en( BOOL enable );
void spi1_int_prio( uint32_t prio );
void spi1_clear_int_flags( void );

/*******************************************************************************
 * Local Data
 ******************************************************************************/
spi_config_t SPI1_cfg = {
    &SPI1CONbits,
    &SPI1BUF,
    &SPI1BRG,
    &SPI1STATbits,
    SPI1,
    &spi1_int_en,
    &spi1_int_prio,
    &spi1_clear_int_flags,
    FALSE,
};

/*******************************************************************************
 * Public function section
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
 * Revision:    Initial Creation 01/24/2016 - Mitchell S. Tilson
 *
 * Notes:       
 *
 ******************************************************************************/
void SPI_init( spi_num_e spi, spi_init_t spi_settings )
{

    uint32_t tmp_rx_buff;

    if( -1 == spi_set_cfg_pointer( spi ) )
    {
        return;
    }

    // Set the intialized flag
    curr_spi_cfg_p->initialized = FALSE;

    // Disable interrupts
    *curr_spi_cfg_p->spi_int_en_func( FALSE );

    // Ensure the peripheral is off
    curr_spi_cfg_p->SPIxCON->ON = 0;

    // Ensure the rx buffer is empty via reading it
    tmp_rx_buff = curr_spi_cfg_p->SPIxBUFF;

    // Enable enhanced buffer
    curr_spi_cfg_p->SPIxCON->ENHBUF = 1;

    // Clear the interrupt flags
    *curr_spi_cfg_p->spi_int_clear_flags_func();

    // Set up the interrupt priorities
    *curr_spi_cfg_p->spi_int_prio_func(spi_settings.interrupt_prio);

    // Enable interrupts
    *curr_spi_cfg_p->spi_int_en_func( TRUE );

    // Write the baud rate register
    *curr_spi_cfg_p->SPIxBRG = spi_settings.baud;

    // Clear the overflow flag
    curr_spi_cfg_p->SPIxSTAT->SPIROV = 0;

    // Set the master mode enabled
    curr_spi_cfg_p->SPIxCON->MSTEN = 1;

    // Set the Slave Select bit to benabled
    curr_spi_cfg_p->SPIxCON->MSSEN = 1;

    // Set the spi mode ( 0 == 8bit )
    curr_spi_cfg_p->SPIxCON->MODE = spi_settings.data_width;

    // Set the interrupt setting ( 1 == interrupt when the buffer is empty )
    curr_spi_cfg_p->SPIxCON->STXISEL = 1;

    // Set the read interrupt to interrupt as soon as there is data
    curr_spi_cfg_p->SPIxCON->SRXISEL = 1;

    // Set the intialized flag
    curr_spi_cfg_p->initialized = TRUE;
}

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
void SPI_enable( spi_num_e spi, BOOL enable )
{
    if( -1 == spi_set_cfg_pointer( pwm ) )
    {
        return;
    }

    if( !curr_spi_cfg_p->initialized )
    {
        return;
    }

    // TRUE and FALSE are defined as 1 and 0
    curr_pwm_cfg_p->SPIxCON->ON = enable;
}

/*******************************************************************************
 * spi_set_cfg_pointer
 *
 * Description: Sets the configuration pointer
 *
 * Inputs:      spi_num_e pwm - the spi to set the configuration for
 *
 * Returns:     int32_t - -1 if the init fails
 *
 * Revision:    Initial Creation 02/15/2016 - Mitchell S. Tilson
 *
 * Notes:       None
 *
 ******************************************************************************/
int32_t spi_set_cfg_pointer( spi_num_e spi )
{
    if( spi != SPI1 )
    {
        return -1;
    }

    // Get the appropriate configuration
    // to modify
    switch( spi )
    {
        case SPI1:
            curr_spi_cfg_p = &SPI1_cfg;
            break;

        default:
            curr_spi_cfg_p = &SPI1_cfg;
            break;
    }
}

