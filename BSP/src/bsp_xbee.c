/*******************************************************************************
 * File:    bsp_xbee.c
 * Author:  Mitchell S. Tilson
 * Created: 01/07/2019
 *
 * Description: This file contains the hardware abstraction layer for the XBEE
 *              module.
 ******************************************************************************/

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "bsp_xbee.h"
#include "driver_spi.h"
#include "type_defs.h"
#include "bsp.h"
#include <p32xxxx.h>
#include <assert.h>

/*******************************************************************************
 * Local definitions
 ******************************************************************************/
#define MAX_TCB_CNT (2)

/*******************************************************************************
 * Local static function definitions
 ******************************************************************************/
static void bsp_xbee_int_init( void );
static void bsp_xbee_spi_en_seq( void );

static OS_TCB* bsp_xbee_tcb_list[MAX_TCB_CNT] = {0};
static uint32_t bsp_xbee_tcb_list_cnt = 0;

/*******************************************************************************
 * Public function section
 ******************************************************************************/

/*******************************************************************************
 * BSP_xbee_init
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
void BSP_xbee_init( void )
{
    // The INT2 interrupt should be enabled after this function is called
    bsp_xbee_int_init();

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
    bsp_xbee_spi_en_seq();
}

/*******************************************************************************
 * BSP_xbee_int_handler
 *
 * Description: This is the handler for the interrupt generated by the XBEE module
 *
 *
 * Inputs:      none
 *
 * Returns:     none
 *
 * Revision:    Initial Creation 01/17/2019 - Mitchell S. Tilson
 *
 ******************************************************************************/
void BSP_xbee_int_handler( void )
{
    OS_ERR err;
    for( uint32_t tcb_idx = 0; tcb_idx < bsp_xbee_tcb_list_cnt; tcb_idx++ )
    {
        OSTaskSemPost(bsp_xbee_tcb_list[tcb_idx],OS_OPT_POST_NO_SCHED,&err);
    }
    IFS0bits.INT2IF = 0; // Clear the interrupt status flag
}

/*******************************************************************************
 * BSP_xbee_register_tcb
 *
 * Description: Register a task control block (TCB) with the xbee BSP code
 *
 *
 * Inputs:      the tcb
 *
 * Returns:     none
 *
 * Revision:    Initial Creation 01/18/2019 - Mitchell S. Tilson
 *
 ******************************************************************************/
void BSP_xbee_register_tcb(OS_TCB* tcb)
{
    if( bsp_xbee_tcb_list_cnt++ <= MAX_TCB_CNT )
    {
        bsp_xbee_tcb_list[bsp_xbee_tcb_list_cnt-1] = tcb;
    }
}

/*******************************************************************************
 * BSP_xbee_write_read
 *
 * Description: Sends or receives data from the xbee module over spi
 *
 *
 * Inputs:      uint8_t* wdata - the data to write
 *              uint8_t* rdata - a pointer to the buffer to read data into
 *              uint16_t len - the length of the data in bytes
 *
 * Returns:     none
 *
 * Revision:    Initial Creation 01/20/2019 - Mitchell S. Tilson
 *
 * Notes:       The write and read data arrays must be the same length.
 *              Its ok if the wdata and rdata point to the same location.  It
 *              just means the wdata will be overwritten with the result.
 *
 ******************************************************************************/
void BSP_xbee_write_read( uint8_ua_t* wdata, uint8_ua_t* rdata, uint16_t len )
{
    spi_ret_e ret_val = SPI_write_read( SPI2, wdata, rdata, len );
    assert(ret_val==SPI_SUCCESS);
}

/*******************************************************************************
 * bsp_xbee_int_init
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
static void bsp_xbee_int_init( void )
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
 * bsp_xbee_spi_en_seq
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
static void bsp_xbee_spi_en_seq( void )
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
void BSP_xbee_test( void )
{
    uint8_t wdata = 0x00;
    uint8_t rdata = 0x00;
    uint32_t cksum = 0;
    SPI_write_read( SPI2, &wdata, &rdata, sizeof(wdata) );
    // Check for start of frame
    if( rdata == 0x7E )
    {
        uint8_t MSB_len = 0x00;
        SPI_write_read( SPI2, &MSB_len, &MSB_len, sizeof(MSB_len) );
        uint8_t LSB_len = 0x00;
        SPI_write_read( SPI2, &LSB_len, &LSB_len, sizeof(LSB_len) );
        uint16_t MSG_len = LSB_len | (MSB_len << 8);
        // MSG_len is the data length in the frame excluding the
        // checksum
        for( uint32_t msg_idx = 0; msg_idx < MSG_len; msg_idx++ )
        {
            rdata = 0x00;
            wdata = 0x00;
            SPI_write_read( SPI2, &wdata, &rdata, sizeof(wdata) );
            cksum += rdata;
        }
        cksum &= 0xFF;
        cksum = 0xFF - cksum;
        uint8_t xbee_cksum = 0;
        wdata = 0x00;
        SPI_write_read( SPI2, &wdata, &xbee_cksum, sizeof(wdata) );
        if( xbee_cksum == cksum )
        {
            xbee_cksum = 0;
        }
    }
}
