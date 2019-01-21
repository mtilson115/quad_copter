/*******************************************************************************
 * File:    comms_xbee.c
 * Author:  Mitchell S. Tilson
 * Created: 01/19/2019
 *
 * Description: This file contains communication layer for the xbee
 *
 ******************************************************************************/

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "comms_xbee.h"
#include "bsp_xbee.h"
#include <os.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <p32xxxx.h>

/*******************************************************************************
 * Local Type Defs
 ******************************************************************************/
typedef struct __attribute__ ((packed)) {
    uint32_t    __reserved;
    uint8_t     ip_0;
    uint8_t     ip_1;
    uint8_t     ip_2;
    uint8_t     ip_3;
} comms_xbee_64bit_ipv4_addr_t;

typedef struct __attribute__ ((packed)) {
    uint8_t                         api_frame_id;
    uint8_t                         frame_id;
    comms_xbee_64bit_ipv4_addr_t    addr;
    uint8_t                         opt;
    uint8_t*                        data;
}comms_xbee_tx_frame_t;

typedef struct __attribute__ ((packed)){
    uint8_t                         api_frame_id;
    uint8_t                         status;
}comms_xbee_rx_status_t;

typedef struct __attribute__ ((packed)) {
    uint8_t                     start_delim;
    uint8_t                     MSB_len;
    uint8_t                     LSB_len;
    uint8_t*                    frame_data_ptr;
    uint8_t                     chksum;
}comms_xbee_api_msg_t;

/*******************************************************************************
 * Constants
 ******************************************************************************/
#define XBEE_MAX_RX (32)

/*******************************************************************************
 * XBEE states
 ******************************************************************************/
enum {
    XBEE_HW_RESET = 0,
    XBEE_WATCHDOG_RESET = 1,
    XBEE_JOINED = 2,
    XBEE_LOST_CONNECTION = 3,
    XBEE_REMOTE_MGR_CONNECTED = 0xE,
    XBEE_REMOTE_MGR_DISCONNECTED = 0xF,
};

/*******************************************************************************
 * API Constants
 ******************************************************************************/

/*******************************************************************************
 * Start delimiter
 ******************************************************************************/
#define XBEE_START_DELIM (0x7E)

/*******************************************************************************
 * API IDs
 ******************************************************************************/
#define XBEE_TX_REQ (0x00)
#define XBEE_STATUS (0x8A)

/*******************************************************************************
 * Local Data
 ******************************************************************************/
static uint32_t comms_xbee_state = XBEE_HW_RESET;
static OS_TCB comms_xbee_TCB;
static CPU_STK comms_xbee_stack[COMMS_XBEE_STK_SIZE];
static uint8_t comms_xbee_rx_buff[XBEE_MAX_RX];

/*******************************************************************************
 * IPV4 Addrs
 ******************************************************************************/
comms_xbee_64bit_ipv4_addr_t comms_xbee_addr_desktop = {
    .ip_0   = 10,
    .ip_1   = 0,
    .ip_2   = 0,
    .ip_3   = 67,
};

/*******************************************************************************
 * Local Function Section
 ******************************************************************************/
static void comms_xbee_task(void *p_arg);
static void comms_xbee_set_rx_buffer(uint8_t* buffer, size_t len);
static void comms_xbee_handle_status(comms_xbee_api_msg_t* api_msg);
static uint8_t comms_xbee_compute_cksum(comms_xbee_api_msg_t* api_msg);

/*******************************************************************************
 * Public Function Section
 ******************************************************************************/

/*******************************************************************************
 * comms_xbee_send
 *
 * Description: Sends a message a message via the xbee
 *
 * Inputs:      comms_xbee_msg_t msg - the message to send
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 01/20/2019 - Mitchell S. Tilson
 *
 ******************************************************************************/
void COMMS_xbee_send(comms_xbee_msg_t msg)
{
    if( comms_xbee_state == XBEE_JOINED || comms_xbee_state == XBEE_REMOTE_MGR_CONNECTED )
    {
        // Build the message
        // Queue the message
        // Trigger the semaphore
    }
}

/*******************************************************************************
 * comms_xbee_init
 *
 * Description: Initializes the xbee code
 *
 * Inputs:      None
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 01/20/2019 - Mitchell S. Tilson
 *
 ******************************************************************************/
void COMMS_xbee_init(void)
{
    OS_ERR err;

    // Create the thread
    OSTaskCreate((OS_TCB      *)&comms_xbee_TCB,
                 (CPU_CHAR    *)"Comms XBEE Task",
                 (OS_TASK_PTR  )comms_xbee_task,
                 (void        *)0,
                 (OS_PRIO      )COMMS_XBEE_PRIO,
                 (CPU_STK     *)&comms_xbee_stack[0],
                 (CPU_STK_SIZE )COMMS_XBEE_STK_SIZE/10,
                 (CPU_STK_SIZE )COMMS_XBEE_STK_SIZE,
                 (OS_MSG_QTY   )10u,
                 (OS_TICK      )0u,
                 (void        *)0,
                 (OS_OPT       )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR      *)&err);
    assert(err==OS_ERR_NONE);

    // Register the TCB with the xbee bsp code
    BSP_xbee_register_tcb(&comms_xbee_TCB);

    // Initialize the SPI, set up interrupts, and put the xbee in SPI mode
    BSP_xbee_init();
}

/*******************************************************************************
 * Local Function Section
 ******************************************************************************/
static void comms_xbee_task(void *p_arg)
{
    TRISEbits.TRISE6 = 0;  // output
    ODCEbits.ODCE6 = 0; // CMOS outout
    OS_ERR err;
    while(DEF_ON)
    {
        /*
         * Wait here until either the interrupt signals or a message is inteneded to be sent
         */
        uint32_t time_stamp = 0;
        OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,&time_stamp,&err);

        /*
         * Check for any messages in the queue.
         */
        uint16_t msg_size_bytes = 0;
        time_stamp = 0;
        void* msg = OSTaskQPend(0,OS_OPT_PEND_NON_BLOCKING,&msg_size_bytes,&time_stamp,&err);

        /*
         * There were no messages.  Read from the xbee.
         */
        if( !msg )
        {
            // Read the first byte to see if we actually have data from the xbee
            comms_xbee_api_msg_t api_msg = {0};
            BSP_xbee_write_read(&api_msg.start_delim,&api_msg.start_delim,sizeof(api_msg.start_delim));
            if( api_msg.start_delim == XBEE_START_DELIM )
            {
                // We have valid data, read the length
                BSP_xbee_write_read(&api_msg.MSB_len,&api_msg.MSB_len,2*sizeof(api_msg.MSB_len));
                uint16_t len = api_msg.LSB_len | (api_msg.MSB_len << 8);

                // Read in the data
                memset(&comms_xbee_rx_buff,0x00,len);
                BSP_xbee_write_read(&comms_xbee_rx_buff[0],&comms_xbee_rx_buff[0],len);

                // Compute the ckecksum
                api_msg.frame_data_ptr = &comms_xbee_rx_buff[0];
                uint8_t cksum = comms_xbee_compute_cksum(&api_msg);
                uint8_t xbee_cksum = 0;

                // Compare the checksum
                BSP_xbee_write_read(&xbee_cksum,&xbee_cksum,sizeof(xbee_cksum));
                if( xbee_cksum == cksum )
                {
                    // Parse the data
                    uint8_t api_frame_id = api_msg.frame_data_ptr[0];
                    switch( api_frame_id )
                    {
                        case XBEE_STATUS:
                            comms_xbee_handle_status(&api_msg);
                            break;
                        default:
                            break;
                    }
                }
            }
            // else no valid data to read
        }
    }
}

static uint8_t comms_xbee_compute_cksum(comms_xbee_api_msg_t* api_msg)
{
    uint16_t len = api_msg->LSB_len | (api_msg->MSB_len << 8);
    uint32_t cksum = 0;
    for( uint32_t idx = 0; idx < len; idx++ )
    {
        cksum += (uint32_t)(api_msg->frame_data_ptr[idx]);
    }
    cksum &= 0xFF;
    cksum = 0xFF - cksum;
    return (uint8_t)cksum;
}

static void comms_xbee_handle_status(comms_xbee_api_msg_t* api_msg)
{
    comms_xbee_rx_status_t* status;
    status = (comms_xbee_rx_status_t*)api_msg->frame_data_ptr;
    comms_xbee_state = status->status;
    if( comms_xbee_state == XBEE_REMOTE_MGR_CONNECTED )
    {
        while(1);
    }
}
