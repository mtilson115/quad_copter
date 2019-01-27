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
 * Constants
 ******************************************************************************/
#define XBEE_MAX_RX (128)
#define XBEE_MAX_TX (128)

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
#define XBEE_TX_REQ     (0x20)
#define XBEE_STATUS     (0x8A)
#define XBEE_AT_CMD     (0x08)
#define XBEE_AT_CMD_RSP (0x88)

/*******************************************************************************
 * AT Commands
 ******************************************************************************/
// AT commands
#define XBEE_PORT           ("C0")
#define XBEE_DEST_ADDR      ("DL")
#define XBEE_IP_PROTOCOL    ("IP")
#define XBEE_SSID           ("ID")

// AT status
#define AT_CMD_STATUS_OK (0)

/*******************************************************************************
 * Options
 ******************************************************************************/
#define XBEE_TX_EN_ACK      (0x00)
#define XBEE_TX_DISABLE_ACK (0x01)
#define XBEE_TX_UDP_OPT     (0x00)

/*******************************************************************************
 * Misc
 ******************************************************************************/
#define XBEE_IP_UDP (0x00)

/*******************************************************************************
 * Local Type Defs
 ******************************************************************************/
// API section
typedef struct __attribute__ ((packed)) {
    uint8_t     start_delim;
    uint8_t     MSB_len;
    uint8_t     LSB_len;
    uint8_t*    frame_data_ptr;
    uint8_t     cksum;
}comms_xbee_api_msg_t;

typedef struct __attribute__ ((packed)) {
    uint32_t    __reserved;
    uint8_t     ip_0;
    uint8_t     ip_1;
    uint8_t     ip_2;
    uint8_t     ip_3;
} comms_xbee_64bit_ipv4_addr_t;

typedef struct __attribute__ ((packed)) {
    uint8_t     ip_0;
    uint8_t     ip_1;
    uint8_t     ip_2;
    uint8_t     ip_3;
} comms_xbee_ipv4_addr_t;

typedef struct __attribute__ ((packed)) {
    uint8_t                         api_frame_id;
    uint8_t                         frame_id;
    comms_xbee_64bit_ipv4_addr_t    addr;
    uint8_t                         opt;
    uint8_t                         data[XBEE_MAX_TX];
}comms_xbee_tx_frame_t;

typedef struct __attribute__ ((packed)) {
    uint8_t                         api_frame_id;
    uint8_t                         frame_id;
    comms_xbee_ipv4_addr_t          addr;
    uint8_t                         dest_port[2];
    uint8_t                         src_port[2];
    uint8_t                         protocol;
    uint8_t                         opt;
    uint8_t                         data[XBEE_MAX_TX];
}comms_xbee_tx_frame_ipv4_t;

typedef struct __attribute__ ((packed)){
    uint8_t     api_frame_id;
    uint8_t     status;
}comms_xbee_rx_status_t;

typedef struct __attribute__ ((packed)) {
    uint8_t     api_frame_id;
    uint8_t     frame_id;
    uint8_t     cmd[2];
}comms_xbee_at_cmd_read_t;

// Internal section
typedef struct {
    uint32_t    xbee_state;
    uint16_t    xbee_port;
    uint8_t     xbee_protocol;
    uint8_t     xbee_dl_ip[4];
    char        xbee_ssid[32];
} comms_xbee_status_t;

/*******************************************************************************
 * Local Data
 ******************************************************************************/

// Status
static comms_xbee_status_t comms_xbee_status = {
    .xbee_state = XBEE_HW_RESET,
    .xbee_port = 0,
    .xbee_protocol = 3,
    .xbee_dl_ip = {0,0,0,0},
    .xbee_ssid = {0},
};

// Task Data
static OS_TCB comms_xbee_TCB;
static CPU_STK comms_xbee_stack[COMMS_XBEE_STK_SIZE];

// Rx buffer
static uint8_t comms_xbee_rx_buff[XBEE_MAX_RX];

/*******************************************************************************
 * IPV4 Addrs
 ******************************************************************************/
comms_xbee_64bit_ipv4_addr_t comms_xbee_64bitaddr_desktop = {
    .ip_0   = 10,
    .ip_1   = 0,
    .ip_2   = 0,
    .ip_3   = 67,
};

comms_xbee_ipv4_addr_t comms_xbee_addr_desktop = {
    .ip_0   = 10,
    .ip_1   = 0,
    .ip_2   = 0,
    .ip_3   = 67,
};

uint8_t comms_xbee_src_port[2] = {0x13,0x8D}; // 5005
uint8_t comms_xbee_dest_port[2] = {0x13,0x8D}; // 5005

/*******************************************************************************
 * Local Function Section
 ******************************************************************************/
static void comms_xbee_task(void *p_arg);
static void comms_xbee_set_rx_buffer(uint8_t* buffer, size_t len);
static void comms_xbee_handle_status(comms_xbee_api_msg_t* api_msg);
static uint8_t comms_xbee_compute_cksum(comms_xbee_api_msg_t* api_msg);
static comms_xbee_api_msg_t comms_xbee_at_cmd_rd(const char at_cmd[2]);
static void comms_send_api_msg(comms_xbee_api_msg_t* api_msg);
static void comms_xbee_handle_at_rsp(comms_xbee_api_msg_t* api_msg);

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
    static uint8_t frame_id = 0;
    if( (comms_xbee_status.xbee_state == XBEE_REMOTE_MGR_CONNECTED) && msg.len <= XBEE_MAX_TX )
    {
        static comms_xbee_api_msg_t api_msg;
        static comms_xbee_tx_frame_ipv4_t tx_frame;

        // Build the TX frame
        tx_frame.api_frame_id = XBEE_TX_REQ;
        tx_frame.frame_id = frame_id;
        tx_frame.addr = comms_xbee_addr_desktop;
        memcpy(tx_frame.dest_port,comms_xbee_dest_port,sizeof(tx_frame.dest_port));
        memcpy(tx_frame.src_port,comms_xbee_src_port,sizeof(tx_frame.src_port));
        tx_frame.protocol = XBEE_IP_UDP;
        tx_frame.opt = XBEE_TX_UDP_OPT;
        memcpy(tx_frame.data,msg.data,msg.len);
        uint16_t len = sizeof(tx_frame)-sizeof(tx_frame.data)+msg.len;

        // Build the API frame
        api_msg.start_delim = XBEE_START_DELIM;
        api_msg.MSB_len = (uint8_t)((len & 0xFF00) >> 8);
        api_msg.LSB_len = (uint8_t)(len & 0xFF);
        api_msg.frame_data_ptr = (uint8_t*)&tx_frame;
        api_msg.cksum = comms_xbee_compute_cksum(&api_msg);

        OS_ERR err;
        // Queue the message
        OSTaskQPost(&comms_xbee_TCB,(void*)&api_msg,sizeof(api_msg),OS_OPT_POST_NO_SCHED,&err);
        if( err == OS_ERR_Q_MAX || err == OS_ERR_MSG_POOL_EMPTY )
        {
            __builtin_software_breakpoint();
        }
        OSTaskSemPost(&comms_xbee_TCB,OS_OPT_POST_NONE,&err);
        if( err == OS_ERR_SEM_OVF )
        {
            __builtin_software_breakpoint();
        }
        // Trigger the semaphore
        frame_id++;
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
 * COMMS_xbee_ready
 *
 * Description: Informs the system if the xbee module is ready for writting
 *
 * Inputs:      None
 *
 * Returns:     uint8_t - comms_xbee_status.xbee_state == XBEE_REMOTE_MGR_CONNECTED.
 *
 * Revision:    Initial Creation 01/20/2019 - Mitchell S. Tilson
 *
 ******************************************************************************/
uint8_t COMMS_xbee_ready( void )
{
    return (uint8_t)(comms_xbee_status.xbee_state == XBEE_REMOTE_MGR_CONNECTED);
}

/*******************************************************************************
 * Local Function Section
 ******************************************************************************/
static void comms_xbee_task(void *p_arg)
{
    // TRISEbits.TRISE6 = 0;  // output
    // ODCEbits.ODCE6 = 0; // CMOS outout
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
                        case XBEE_AT_CMD_RSP:
                            comms_xbee_handle_at_rsp(&api_msg);
                            break;
                        default:
                            break;
                    }
                }
            }
            // else no valid data to read
        }

        if( msg )
        {
            comms_xbee_api_msg_t* api_msg = (comms_xbee_api_msg_t*)msg;
            comms_send_api_msg(api_msg);
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
    comms_xbee_status.xbee_state = status->status;
    if( comms_xbee_status.xbee_state == XBEE_REMOTE_MGR_CONNECTED )
    {
        // Read the configuration of the module
        comms_xbee_at_cmd_rd(XBEE_PORT);
    }
}

static void comms_xbee_handle_at_rsp(comms_xbee_api_msg_t* api_msg)
{
    char at_cmd[2];
    memcpy(at_cmd,&api_msg->frame_data_ptr[2],sizeof(at_cmd));
    if( 0 == memcmp(at_cmd,XBEE_PORT,sizeof(at_cmd)) )
    {
        uint8_t status = api_msg->frame_data_ptr[sizeof(at_cmd)+2];
        if( status == AT_CMD_STATUS_OK )
        {
            comms_xbee_status.xbee_port = ((uint16_t)(api_msg->frame_data_ptr[sizeof(at_cmd)+3] & 0xFF)) << 8;
            comms_xbee_status.xbee_port |= (uint16_t)(api_msg->frame_data_ptr[sizeof(at_cmd)+4]);
            comms_xbee_at_cmd_rd(XBEE_IP_PROTOCOL);
        }
    }
    else if( 0 == memcmp(at_cmd,XBEE_IP_PROTOCOL,sizeof(at_cmd)) )
    {
        uint8_t status = api_msg->frame_data_ptr[sizeof(at_cmd)+2];
        if( status == AT_CMD_STATUS_OK )
        {
            comms_xbee_status.xbee_protocol = api_msg->frame_data_ptr[sizeof(at_cmd)+3];
            comms_xbee_at_cmd_rd(XBEE_DEST_ADDR);
        }
    }
    else if( 0 == memcmp(at_cmd,XBEE_DEST_ADDR,sizeof(at_cmd)) )
    {
        uint8_t status = api_msg->frame_data_ptr[sizeof(at_cmd)+2];
        if( status == AT_CMD_STATUS_OK )
        {
            memcpy(comms_xbee_status.xbee_dl_ip,&api_msg->frame_data_ptr[sizeof(at_cmd)+3],sizeof(comms_xbee_status.xbee_dl_ip));
            comms_xbee_at_cmd_rd(XBEE_SSID);
        }
    }
    else if( 0 == memcmp(at_cmd,XBEE_SSID,sizeof(at_cmd)) )
    {
        uint8_t status = api_msg->frame_data_ptr[sizeof(at_cmd)+2];
        if( status == AT_CMD_STATUS_OK )
        {
            uint16_t len = ((uint16_t)(api_msg->MSB_len << 8)) | (uint16_t)(api_msg->LSB_len);
            memcpy(comms_xbee_status.xbee_ssid,&api_msg->frame_data_ptr[5],len-5);
        }
    }
}

static comms_xbee_api_msg_t comms_xbee_at_cmd_rd(const char at_cmd[2])
{
    comms_xbee_api_msg_t api_msg;
    comms_xbee_at_cmd_read_t at_cmd_rd;
    at_cmd_rd.api_frame_id = XBEE_AT_CMD;
    at_cmd_rd.frame_id = 1;
    memcpy(at_cmd_rd.cmd,at_cmd,2);
    api_msg.start_delim = XBEE_START_DELIM;
    uint16_t len = (uint16_t)sizeof(at_cmd_rd);
    api_msg.MSB_len = (uint8_t)((len & 0xFF00) >> 8);
    api_msg.LSB_len = (uint8_t)(len & 0xFF);
    api_msg.frame_data_ptr = (uint8_t*)&at_cmd_rd;
    api_msg.cksum = comms_xbee_compute_cksum(&api_msg);
    comms_send_api_msg(&api_msg);
}

static void comms_send_api_msg(comms_xbee_api_msg_t* api_msg)
{
    uint16_t len = ((uint16_t)(api_msg->MSB_len << 8)) | (uint16_t)(api_msg->LSB_len);
    uint32_t header_size = sizeof(api_msg->start_delim)+sizeof(api_msg->MSB_len)+sizeof(api_msg->LSB_len);

    // Send the start delim and the message length
    BSP_xbee_write_read(&api_msg->start_delim,comms_xbee_rx_buff,header_size);

    // Send the actual message
    BSP_xbee_write_read(api_msg->frame_data_ptr,&comms_xbee_rx_buff[header_size],len);

    // Send the checksum
    BSP_xbee_write_read(&api_msg->cksum,&comms_xbee_rx_buff[header_size+len],(uint16_t)sizeof(api_msg->cksum));
}
