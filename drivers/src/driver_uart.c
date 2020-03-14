/*******************************************************************************
 * File:    driver_uart.c
 * Author:  Mitchell S. Tilson
 * Created: 02/20/2014
 *
 * Description: This file provides a driver interface to the UART periperial.
 ******************************************************************************/

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "driver_uart.h"
#include <stdbool.h>
#include <p32xxxx.h>

/*******************************************************************************
 * Local Type Defs
 ******************************************************************************/

/*******************************************************************************
 * Local Data
 ******************************************************************************/
UART_REGS const UART[UART_PERIF_NUM] = {
    // Uart 1
    {
        .mode = {
            .reg = &U1MODE,
            .clr = &U1MODECLR,
            .set = &U1MODESET,
            .inv = &U1MODEINV,
        },
        .sta = {
            .reg = &U1STA,
            .clr = &U1STACLR,
            .set = &U1STASET,
            .inv = &U1STAINV,
        },
        .tx = &U1TXREG,
        .rx = &U1RXREG,
        .brg = &U1BRG,
    },
    // Uart 2
    {
        .mode = {
            .reg = &U2MODE,
            .clr = &U2MODECLR,
            .set = &U2MODESET,
            .inv = &U2MODEINV,
        },
        .sta = {
            .reg = &U2STA,
            .clr = &U2STACLR,
            .set = &U2STASET,
            .inv = &U2STAINV,
        },
        .tx = &U2TXREG,
        .rx = &U2RXREG,
        .brg = &U2BRG,
    },
    // Uart 3
    {
        .mode = {
            .reg = &U3MODE,
            .clr = &U3MODECLR,
            .set = &U3MODESET,
            .inv = &U3MODEINV,
        },
        .sta = {
            .reg = &U3STA,
            .clr = &U3STACLR,
            .set = &U3STASET,
            .inv = &U3STAINV,
        },
        .tx = &U3TXREG,
        .rx = &U3RXREG,
        .brg = &U3BRG,
    },
    // Uart 4
    {
        .mode = {
            .reg = &U4MODE,
            .clr = &U4MODECLR,
            .set = &U4MODESET,
            .inv = &U4MODEINV,
        },
        .sta = {
            .reg = &U4STA,
            .clr = &U4STACLR,
            .set = &U4STASET,
            .inv = &U4STAINV,
        },
        .tx = &U4TXREG,
        .rx = &U4RXREG,
        .brg = &U4BRG,
    },
};

UART_ctrl_type UART_ctrl[UART_PERIF_NUM];
/*******************************************************************************
 * Local Functions
 ******************************************************************************/
void uart_mode_setup( UART_ID id );
void uart_stat_setup( UART_ID id );
void uart_set_baud( UART_ID id );

/*******************************************************************************
 * Public Function Section
 ******************************************************************************/

/*******************************************************************************
 * Uart_init
 *
 * Description:     Initializes the uart identified by the ID (only 2 uarts are
 *                  are supported)
 *
 * Inputs:      UART_ID id - uart number (1 or 2)
 *              int32_t buad - buad rate
 *              int32_t data_bits - data bits (8 or 9)
 *              int32_t parity - parity
 *              int32_t stop_bits - stop bits
 *
 * Returns:     UART_ID - the id of the uart to be used by the user of the
 *              driver
 *
 * Revision:    Initial Creation 20FEB2014 - Mitchell S. Tilson
 *
 * Notes:       The uart will always be set up to only use the TX and RX pins.
 *              RTS and CTS will never be used.
 *
 ******************************************************************************/
UART_ID  Uart_init( UART_ID id, int32_t baud, int32_t data_bits, PARITY_type parity, int32_t stop_bits )
{
    // Ensure the uart number is valid
    if( id < UART_1 || id > UART_4 )
    {
        return UART_INVALID;
    }

    // Set up the variables
    UART_ctrl[id].enabled = false;
    UART_ctrl[id].baud = baud;
    UART_ctrl[id].data_bits = data_bits;
    UART_ctrl[id].parity = parity;
    UART_ctrl[id].stop_bits = stop_bits;

    // Set up the mode register
    uart_mode_setup( id );

    // Set up the stat register
    uart_stat_setup( id );

    // Set the baud rate
    uart_set_baud( id );

    return id;
}

/*******************************************************************************
 * Uart_enable
 *
 * Description:     Enables the UART
 *
 * Inputs:      UART_ID id - the uart to enable
 *
 * Revision:    Initial Creation 23FEB2014 - Mitchell S. Tilson
 *
 ******************************************************************************/
void Uart_enable( UART_ID id )
{
    *UART[id].sta.set       = STA_URXEN(1);
    *UART[id].sta.set       = STA_UTXEN(1);
    *UART[id].mode.set      = MODE_ON(1);
}

/*******************************************************************************
 * Uart_disable
 *
 * Description:     Disables the UART
 *
 * Inputs:      UART_ID id - the uart to Disable
 *
 * Revision:    Initial Creation 23FEB2014 - Mitchell S. Tilson
 *
 ******************************************************************************/
void Uart_disable( UART_ID id )
{
    *UART[id].mode.clr = MODE_ON(1);
    __asm__ volatile("nop");
    __asm__ volatile("nop");
    *UART[id].sta.clr = STA_URXEN(1);
    *UART[id].sta.clr = STA_UTXEN(1);
    UART_ctrl[id].enabled  = false;
}

/*******************************************************************************
 * Uart_read
 *
 * Description:     Reads data into the passed buffer
 *
 * Inputs:      UART_ID id - the uart to read
 *              int32_t byte_count - the number of bytes to read
 *              uint8_t* buffer - the buffer to read the characters into
 *              bool block - wether or not to block the caller
 *
 * Revision:    Initial Creation 23FEB2014 - Mitchell S. Tilson
 *
 * Notes:       This implementation only works with 8 bit data.
 *
 ******************************************************************************/
int32_t Uart_read( UART_ID id, int32_t byte_count, uint8_t* buffer, bool block )
{
    int32_t i = 0;
    int32_t bytes_read = 0;

    // Blocking read
    if( block ) {
        while( byte_count != 0 ) {
            while( !(*UART[id].sta.reg & STA_URXDA(1)) );
            buffer[i++] = (uint8_t)(*UART[id].rx);
            byte_count--;
            bytes_read++;
        }

    // Non-blocking read (this will return with no data if there isn't any)
    } else {
        do {
            buffer[i++] = (uint8_t)(*UART[id].rx);
            byte_count--;
            bytes_read++;
        } while(*UART[id].sta.reg & STA_URXDA(1));
    }

    return bytes_read;
}

/*******************************************************************************
 * Uart_write
 *
 * Description:     Writes data from the passed buffer
 *
 * Inputs:      UART_ID id - the uart to write
 *              int32_t byte_count - the number of bytes to write
 *              uint8_t* buffer - the buffer to read the characters from
 *              bool block - wether or not to block the caller
 *
 * Revision:    Initial Creation 23FEB2014 - Mitchell S. Tilson
 *
 ******************************************************************************/
int32_t Uart_write( UART_ID id, int32_t byte_count, uint8_t* buffer )
{
    int32_t bytes_written = 0;

	while( bytes_written != byte_count ) {
		while( !(*UART[id].sta.reg & STA_TRMT(1)) ) {};
		*UART[id].tx = buffer[bytes_written++];
	}

    return bytes_written;
}

/*******************************************************************************
 * Uart_check_baudr
 *
 *
 * Description:     Puts the uart in a mode where the clock is on UxBCLK
 *
 * Inputs:      UART_ID id - the uart to write
 *
 *
 * Revision:    Initial Creation 23FEB2014 - Mitchell S. Tilson
 *
 ******************************************************************************/
void Uart_check_baud( UART_ID id )
{
    // Disable the uart
    Uart_disable( id );

    // Change the UEN
    // *UART[id].mode.set = MODE_UEN(3);

    // Enable the uart->
    Uart_enable( id );
}

/*******************************************************************************
 * Local Function Section
 ******************************************************************************/

/*******************************************************************************
 * uart_mode_setup
 *
 * Description:     Sets up the MODE register
 *
 * Inputs:      UART_ID id - uart number
 *
 * Revision:    Initial Creation 23FEB2014 - Mitchell S. Tilson
 *
 *
 ******************************************************************************/
void uart_mode_setup( UART_ID id )
{
    // Set up the register values
    *UART[id].mode.clr = MODE_ON(1);        // off until everything is configured

    // Wait two clock cycles just in case (the manual says not to write
    // the registers directly after clearing ON when the PBCLK divisor is 1:1)
    __asm__ volatile("nop");
    __asm__ volatile("nop");

    *UART[id].mode.clr = MODE_SIDL(1);      // continue operation in idle mode
    *UART[id].mode.clr = MODE_IREN(1);      // disable IrDa mode
    *UART[id].mode.set |= MODE_RTSMD(1);     // RTS in simplex mode
    *UART[id].mode.clr = MODE_UEN(1);       // RX and TX pins only
    *UART[id].mode.clr = MODE_WAKE(1);      // wake on startbit while in sleep is disabled
    *UART[id].mode.clr = MODE_LPBACK(1);    // loop back is off
    *UART[id].mode.clr = MODE_ABAUD(1);     // baud rate must be set by user
    *UART[id].mode.clr = MODE_RXINV(1);     // pull up RX line while not in use
    *UART[id].mode.clr = MODE_BRGH(1);      // standard baud rates

    // parity and data selection bits
    if( UART_ctrl[id].parity == PARITY_NONE ) {
        if( UART_ctrl[id].data_bits == 9 ) {
            *UART[id].mode.set = MODE_PDSEL(3);
        }
        if( UART_ctrl[id].data_bits == 8 ) {
            *UART[id].mode.clr = MODE_PDSEL(3);
            U4MODEbits.PDSEL = 0;
        }
    } else if( UART_ctrl[id].parity == PARITY_EVEN ) {
        *UART[id].mode.set = MODE_PDSEL(1);
    } else if( UART_ctrl[id].parity == PARITY_ODD ) {
        *UART[id].mode.set = MODE_PDSEL(2);
    }

    *UART[id].mode.reg |= MODE_STSEL((UART_ctrl[id].stop_bits - 1)); // set up the stop bits
}

/*******************************************************************************
 * uart_stat_setup
 *
 * Description:     Sets up the STAT register
 *
 * Inputs:      UART_ID id - uart number
 *
 * Revision:    Initial Creation 23FEB2014 - Mitchell S. Tilson
 *
 *
 ******************************************************************************/
void uart_stat_setup( UART_ID id )
{
    // Set up the STAT register
    *UART[id].sta.clr = STA_ADM_EN(1);    // no automatic address
    // No uart interrupts are set up for now
    // ADDR is left to default
    // UTXISEL is left to default
    *UART[id].sta.clr = STA_UTXINV(1);    // do not invert the idle state
    *UART[id].sta.clr = STA_URXEN(1);     // RX is disabled
    *UART[id].sta.clr = STA_UTXBRK(1);    // Disable sending the break for now
    *UART[id].sta.clr = STA_UTXEN(1);     // TX is disabled
    // UTXBF                            // Read only bit indicating the buffer is full
    // TRMT                             // Read only bit indicating the transmit shift register is full
    // URXISEL                          // Left to default because interrupts are not used
    *UART[id].sta.clr = STA_ADDEN(1);     // No address detection
    // RIDLE                            // Read only bit indicating receiver is idle
    // PERR                             // Read only Parity error occured
    // FERR                             // Read only Framing error
    *UART[id].sta.clr = STA_OERR(1);      // Can only be cleared by software indicating Receive buffer overrun
    // URXDA                            // Read only bit indicating that there is data in the UART*/
}

/*******************************************************************************
 * uart_set_baud
 *
 * Description:     Calculates and sets the buad rate
 *
 * Inputs:      UART_ID id - uart number
 *
 * Revision:    Initial Creation 23FEB2014 - Mitchell S. Tilson
 *
 *
 ******************************************************************************/
void uart_set_baud( UART_ID id )
{
    float BRG;

    // Periferal bus clock is set up to run at the same rate as the sysclock
    BRG = ((80000000/(16*UART_ctrl[id].baud))-1)+0.5;

    // Set the BRG register
    *UART[id].brg = (REG)BRG;
}
