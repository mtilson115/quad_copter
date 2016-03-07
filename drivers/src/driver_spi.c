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
    void (*spi_int_prio_func)( uint32_t prio, uint32_t sub_prio );
    void (*spi_int_clear_flags_func)( void );
    BOOL initialized;
} spi_config_t;

/*******************************************************************************
 * Local Functions
 ******************************************************************************/
int32_t spi_set_cfg_pointer( spi_num_e spi );
void spi1_int_en( BOOL enable );
void spi1_int_prio( uint32_t prio, uint32_t sub_prio );
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

spi_config_t* curr_spi_cfg_p = &SPI1_cfg;

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
 * Notes:       Updates for interrupt enabling.
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
    curr_spi_cfg_p->spi_int_en_func( FALSE );

    // Ensure the peripheral is off
    curr_spi_cfg_p->SPIxCON->ON = 0;

    // Ensure the rx buffer is empty via reading it
    tmp_rx_buff = *curr_spi_cfg_p->SPIxBUFF;

    // Enable enhanced buffer
    curr_spi_cfg_p->SPIxCON->ENHBUF = 1;

    // Set up the intterrupts for enhanced buffering
    // Set a TX interrupt when the last data is shifted out
    curr_spi_cfg_p->SPIxCON->STXISEL = 0;

    // Interrupt when the buffer is full
    curr_spi_cfg_p->SPIxCON->SRXISEL = 3;

    // Clear the interrupt flags
    curr_spi_cfg_p->spi_int_clear_flags_func();

    // Set up the interrupt priorities
    curr_spi_cfg_p->spi_int_prio_func( spi_settings.interrupt_prio, spi_settings.interrupt_sub_prio );

    // Enable interrupts
    curr_spi_cfg_p->spi_int_en_func( TRUE );

    // Write the baud rate register
    *curr_spi_cfg_p->SPIxBRG = spi_settings.baud;

    // Clear the overflow flag
    curr_spi_cfg_p->SPIxSTAT->SPIROV = 0;

    // Set the master mode enabled
    curr_spi_cfg_p->SPIxCON->MSTEN = 1;

    // Set the Slave Select bit to benabled
    curr_spi_cfg_p->SPIxCON->MSSEN = 1;

    // Set the spi mode 
    if( spi_settings.data_width & 2 )
    {
        curr_spi_cfg_p->SPIxCON->MODE32 = 1;
    }
    if( spi_settings.data_width & 1 )
    {
        curr_spi_cfg_p->SPIxCON->MODE16 = 1;
    }

    // Set the interrupt setting ( 1 == interrupt when the buffer is empty )
    curr_spi_cfg_p->SPIxCON->STXISEL = 1;

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
    if( -1 == spi_set_cfg_pointer( spi ) )
    {
        return;
    }

    if( !curr_spi_cfg_p->initialized )
    {
        return;
    }

    // TRUE and FALSE are defined as 1 and 0
    curr_spi_cfg_p->SPIxCON->ON = enable;
}

/*******************************************************************************
 * SPI_write
 *
 * Description: Writes to the selected SPI
 *
 * Inputs:      spi_num_e spi - the spi to write data to.
 *              uint8_t* data - a pointer to the data to send
 *              uint32_t data_len - the number of bytes to send
 *
 * Returns:     spi_ret_e - ERROR, BUFF_FULL, or SUCCESS.
 *
 * Revision:    Initial Creation 02/16/2016 - Mitchell S. Tilson
 *
 * Notes:
 *
 ******************************************************************************/
spi_ret_e SPI_write( spi_num_e spi, uint8_t* data, uint32_t data_len )
{
    uint32_t i = 0;

    if( -1 == spi_set_cfg_pointer( spi ) )
    {
        return SPI_ERROR;
    }

    if( !curr_spi_cfg_p->SPIxCON->ON )
    {
        return SPI_ERROR;
    }

    for( i = 0; i < data_len; i++ )
    {
        while( curr_spi_cfg_p->SPIxSTAT->SPIBUSY );
        *curr_spi_cfg_p->SPIxBUFF = data[i];
    }
    return SPI_SUCCESS;
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

/*******************************************************************************
 * spi1_int_en
 *
 * Description: Sets the SPI 1 interrupt enable
 *
 * Inputs:      BOOL enable - TRUE to enable FALSE otherwise
 *
 * Returns:     none
 *
 * Revision:    Initial Creation 02/16/2016 - Mitchell S. Tilson
 *
 * Notes:       None
 *
 ******************************************************************************/
void spi1_int_en( BOOL enable )
{
    IEC0bits.SPI1AEIE   = enable;
    IEC0bits.SPI1ARXIE  = enable;
    IEC0bits.SPI1ATXIE  = enable;
    return;
}

/*******************************************************************************
 * spi1_int_prio
 *
 * Description: Sets the interrupt priority
 *
 * Inputs:      prio - the priority to set
 *
 * Returns:     none
 *
 * Revision:    Initial Creation 03/06/2016 - Mitchell S. Tilson
 *
 * Notes:       None
 *
 ******************************************************************************/
void spi1_int_prio( uint32_t prio, uint32_t sub_prio )
{
    if( prio <= 7 && sub_prio <= 3 )
    {
        IPC6bits.SPI1AIP = prio;
        IPC6bits.SPI1AIS = sub_prio;
    }
    return;
}

/*******************************************************************************
 * spi1_clear_int_flags
 *
 * Description: Clears the SPI1 interrupt flags
 *
 * Inputs:      none
 *
 * Returns:     none
 *
 * Revision:    Initial Creation 03/06/2016 - Mitchell S. Tilson
 *
 * Notes:       None
 *
 ******************************************************************************/
void spi1_clear_int_flags( void )
{
    IFS0bits.SPI1AEIF   = 0;
    IFC0bits.SPI1ARXIF  = 0;
    IFC0bits.SPI1ATXIF  = 0;
    return;
}
