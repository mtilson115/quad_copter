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
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <p32xxxx.h>

/*******************************************************************************
 * Constants
 ******************************************************************************/
#define XBEE_MAX_HDR        (12)
#define XBEE_CKSM_SZ        (1)
// This value should include any TX headers because the
// RX buffer is written to any time a TX is completed
#define XBEE_MAX_TX         (128)
#define XBEE_MAX_RX         (XBEE_MAX_TX + XBEE_MAX_HDR + XBEE_CKSM_SZ)
#define XBEE_MAX_IPV4_RX    (64)
#define XBEE_MAX_CB         (3)

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
#define XBEE_RX_MSG     (0xB0)

/*******************************************************************************
 * AT Commands
 ******************************************************************************/
// AT commands
#define XBEE_PORT           ("C0")
#define XBEE_DEST_ADDR      ("DL")
#define XBEE_IP_PROTOCOL    ("IP")
#define XBEE_SSID           ("ID")
#define XBEE_MY_IP          ("MY")

// AT status
#define AT_CMD_STATUS_OK (0)

/*******************************************************************************
 * Options
 ******************************************************************************/
#define XBEE_TX_EN_ACK      (0x00)
#define XBEE_TX_DISABLE_ACK (0x01)
#define XBEE_TX_UDP_OPT     (0x00)
#define XBEE_TX_TCP_OPT     (0x00) // Leave socket open after transmission

/*******************************************************************************
 * Misc
 ******************************************************************************/
#define XBEE_IP_UDP (0x00)
#define XBEE_IP_TCP (0x01)
#define XBEE_IP     (XBEE_IP_TCP)

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

typedef struct __attribute__ ((packed)) {
    uint8_t     api_frame_id;
    uint8_t     src_ipv4_addr[4];
    uint16_t    dest_port;
    uint16_t    src_port;
    uint8_t     protocol;
    uint8_t     status;
    uint8_t     rx_data[XBEE_MAX_IPV4_RX];
}comms_xbee_ipv4_rx_t;

// Internal section
typedef struct {
    uint32_t    xbee_state;
    uint16_t    xbee_port;
    uint8_t     xbee_protocol;
    uint8_t     xbee_dl_ip[4];
    char        xbee_ssid[32];
    uint8_t     xbee_my_ip[4];
    // TODO: Add power level
} comms_xbee_status_t;

typedef struct __attribute__((packed)) {
    comms_xbee_api_msg_t        msg;
    comms_xbee_tx_frame_ipv4_t  frame;
} tx_frame_data_t;

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
    .xbee_my_ip = {0,0,0,0},
};

// Task Data
OS_TCB comms_xbee_TCB;
CPU_STK comms_xbee_stack[COMMS_XBEE_STK_SIZE];

// Rx buffer
static uint8_t comms_xbee_rx_buff[XBEE_MAX_RX];

// Msg queue memory
OS_MEM comms_xbee_tx_mem_ctrl_blk;
#define TX_MEM_BLK_SIZE (sizeof(comms_xbee_tx_frame_ipv4_t)+sizeof(comms_xbee_api_msg_t))
#define TX_Q_DEPTH (20)
static uint8_t comms_tx_xbee_mem[TX_MEM_BLK_SIZE*TX_Q_DEPTH];

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

comms_xbee_ipv4_addr_t comms_xbee_addr_work_laptop = {
    .ip_0   = 10,
    .ip_1   = 0,
    .ip_2   = 0,
    .ip_3   = 177,
};

comms_xbee_ipv4_addr_t comms_xbee_addr_xbee_ctrl = {
    .ip_0   = 192,
    .ip_1   = 168,
    .ip_2   = 1,
    .ip_3   = 200,
};

//uint8_t comms_xbee_src_port[2] = {0x13,0x8D}; // 5005
uint8_t comms_xbee_src_port[2] = {0x00,0x00}; // 0 for TCP IP?
uint8_t comms_xbee_dest_port[2] = {0x13,0x8D}; // 5005

/*******************************************************************************
 * Call back array
 ******************************************************************************/
comms_xbee_rx_cb_t xbee_rx_cbs[XBEE_MAX_CB];
static uint32_t xbee_cb_count = 0;

/*******************************************************************************
 * Local Function Section
 ******************************************************************************/
static void comms_xbee_task(void *p_arg);
static void comms_xbee_set_rx_buffer(uint8_t* buffer, size_t len);
static void comms_xbee_handle_status(comms_xbee_api_msg_t* api_msg);
static uint8_t comms_xbee_compute_cksum(comms_xbee_api_msg_t* api_msg);
static comms_xbee_api_msg_t comms_xbee_at_cmd_rd(const char at_cmd[2]);
static void comms_xbee_send_api_msg(comms_xbee_api_msg_t* api_msg);
static void comms_xbee_handle_at_rsp(comms_xbee_api_msg_t* api_msg);
static void comms_xbee_handle_rx_ipv4(comms_xbee_api_msg_t* api_msg);
static void comms_xbee_handle_int_msg( void );
static void comms_xbee_handle_rx_during_tx( uint16_t bytes_read );
static void comms_xbee_parse_rx( comms_xbee_api_msg_t* api_msg );

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
    OS_ERR err;
    if( (comms_xbee_status.xbee_state == XBEE_JOINED) && msg.len <= XBEE_MAX_TX )
    {
        tx_frame_data_t* tx_frame_data = (tx_frame_data_t*)OSMemGet(&comms_xbee_tx_mem_ctrl_blk,&err);
        if( err != OS_ERR_NONE )
        {
            while(1);
        }
        comms_xbee_api_msg_t* api_msg = &tx_frame_data->msg;
        comms_xbee_tx_frame_ipv4_t* tx_frame = &tx_frame_data->frame;

        // Build the TX frame
        tx_frame->api_frame_id = XBEE_TX_REQ;
        tx_frame->frame_id = frame_id;
        tx_frame->addr = comms_xbee_addr_xbee_ctrl;
        memcpy(tx_frame->dest_port,comms_xbee_dest_port,sizeof(tx_frame->dest_port));
        memcpy(tx_frame->src_port,comms_xbee_src_port,sizeof(tx_frame->src_port));
        tx_frame->protocol = XBEE_IP;
        tx_frame->opt = XBEE_TX_TCP_OPT;
        memcpy(tx_frame->data,msg.data,msg.len);
        uint16_t len = sizeof(comms_xbee_tx_frame_ipv4_t)-sizeof(tx_frame->data)+msg.len;

        // Build the API frame
        api_msg->start_delim = XBEE_START_DELIM;
        api_msg->MSB_len = (uint8_t)((len & 0xFF00) >> 8);
        api_msg->LSB_len = (uint8_t)(len & 0xFF);
        api_msg->frame_data_ptr = (uint8_t*)tx_frame;
        api_msg->cksum = comms_xbee_compute_cksum(api_msg);

        // Queue the message
        OSTaskQPost(&comms_xbee_TCB,(void*)api_msg,sizeof(comms_xbee_api_msg_t),OS_OPT_POST_NO_SCHED,&err);
        if( err == OS_ERR_Q_MAX || err == OS_ERR_MSG_POOL_EMPTY )
        {
            OSMemPut(&comms_xbee_tx_mem_ctrl_blk,api_msg,&err);
            while(1);
            // break;
        }
        // Trigger the semaphore
        OSTaskSemPost(&comms_xbee_TCB,OS_OPT_POST_NONE,&err);
        if( err == OS_ERR_SEM_OVF )
        {
            OSMemPut(&comms_xbee_tx_mem_ctrl_blk,api_msg,&err);
            while(1);
            // break;
        }
        // frame_id++;
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

    memset(comms_tx_xbee_mem,0x00,sizeof(comms_tx_xbee_mem));

    // register a memory block for the communications
    OSMemCreate((OS_MEM     *)&comms_xbee_tx_mem_ctrl_blk,
                (CPU_CHAR   *)"Comms XBEE Mem",
                (void       *)comms_tx_xbee_mem,
                (OS_MEM_QTY  )TX_Q_DEPTH,
                (OS_MEM_SIZE )TX_MEM_BLK_SIZE,
                (OS_ERR     *)&err);
    assert(err==OS_ERR_NONE);

    // Create the thread
    OSTaskCreate((OS_TCB      *)&comms_xbee_TCB,
                 (CPU_CHAR    *)"Comms XBEE Task",
                 (OS_TASK_PTR  )comms_xbee_task,
                 (void        *)0,
                 (OS_PRIO      )COMMS_XBEE_PRIO,
                 (CPU_STK     *)&comms_xbee_stack[0],
                 (CPU_STK_SIZE )COMMS_XBEE_STK_SIZE/10,
                 (CPU_STK_SIZE )COMMS_XBEE_STK_SIZE,
                 (OS_MSG_QTY   )TX_Q_DEPTH,
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
 * COMMS_xbee_get_tcb
 *
 * Description: Returns a pointer to the task control block.  This is used in the
 *              exception handler.
 *
 * Inputs:      None
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 01/20/2019 - Mitchell S. Tilson
 *
 ******************************************************************************/
OS_TCB* COMMS_xbee_get_tcb( void )
{
    return &comms_xbee_TCB;
}

/*******************************************************************************
 * COMMS_xbee_ready
 *
 * Description: Informs the system if the xbee module is ready for writting
 *
 * Inputs:      None
 *
 * Returns:     uint8_t - comms_xbee_status.xbee_state == XBEE_JOINED.
 *
 * Revision:    Initial Creation 01/20/2019 - Mitchell S. Tilson
 *
 ******************************************************************************/
uint8_t COMMS_xbee_ready( void )
{
    return (uint8_t)(comms_xbee_status.xbee_state == XBEE_JOINED);
}

/*******************************************************************************
 * COMMS_xbee_register_rx_cb
 *
 * Description: Registers a function to be called for a specific RX message
 *
 * Inputs:      comms_xbee_rx_cb_t cb_data - the function pointer and RX message
 *              to call this function for.
 *
 * Returns:     ret_t - rSUCCESS if the registration succeeded and rFAILURE if
 *              there is too many registered return functions
 *
 * Revision:    Initial Creation 03/02/2019 - Mitchell S. Tilson
 *
 ******************************************************************************/
ret_t COMMS_xbee_register_rx_cb( comms_xbee_rx_cb_t cb_data )
{
    xbee_cb_count++;
    if( xbee_cb_count <= XBEE_MAX_CB )
    {
        xbee_rx_cbs[xbee_cb_count-1] = cb_data;
        return rSUCCESS;
    }
    return rFAILURE;
}

/*******************************************************************************
 * Local Function Section
 ******************************************************************************/
static void comms_xbee_task(void *p_arg)
{
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
        if( !msg || err == OS_ERR_PEND_WOULD_BLOCK )
        {
            comms_xbee_handle_int_msg();
        }

        if( err == OS_ERR_NONE && msg )
        {
            comms_xbee_api_msg_t* api_msg = (comms_xbee_api_msg_t*)msg;
            comms_xbee_send_api_msg(api_msg);
            OSMemPut(&comms_xbee_tx_mem_ctrl_blk,api_msg,&err);
        }
    }
}

static uint8_t comms_xbee_compute_cksum(comms_xbee_api_msg_t* api_msg)
{
    uint16_t len = (uint16_t)api_msg->LSB_len | ((uint16_t)api_msg->MSB_len << 8);
    uint32_t cksum = 0;
    for( uint32_t idx = 0; idx < len; idx++ )
    {
        cksum += (uint32_t)(api_msg->frame_data_ptr[idx]);
    }
    cksum &= 0xFF;
    cksum = 0xFF - cksum;
    return (uint8_t)cksum;
}

static void comms_xbee_handle_int_msg( void )
{
    // Read the first byte to see if we actually have data from the xbee
    comms_xbee_api_msg_t api_msg = {0};
    uint8_t empty = 0;
    uint16_t cksum_size = sizeof(uint8_t);
    BSP_xbee_write_read(&empty,&api_msg.start_delim,sizeof(api_msg.start_delim));
    if( api_msg.start_delim == XBEE_START_DELIM )
    {
        // We have valid data, read the length
        BSP_xbee_write_read(&api_msg.MSB_len,&api_msg.MSB_len,2*sizeof(api_msg.MSB_len));
        uint16_t len = (uint16_t)api_msg.LSB_len | ((uint16_t)api_msg.MSB_len << 8);

        // Read in the data
        if( len+cksum_size <= sizeof(comms_xbee_rx_buff) )
        {
            memset(comms_xbee_rx_buff,0x00,len);
            BSP_xbee_write_read(&empty,&comms_xbee_rx_buff[0],len+cksum_size);
        }
        else
        {
            // critical fault
            while(1);
        }

        api_msg.frame_data_ptr = &comms_xbee_rx_buff[0];
        comms_xbee_parse_rx(&api_msg);
    }
}

static void comms_xbee_parse_rx( comms_xbee_api_msg_t* api_msg )
{

    uint8_t cksum = comms_xbee_compute_cksum(api_msg);
    uint8_t xbee_cksum = 0;
    uint16_t len = (uint16_t)api_msg->LSB_len | ((uint16_t)api_msg->MSB_len << 8);

    /*
     * The checksum is the last byte that was read in the frame_data
     */
    xbee_cksum = api_msg->frame_data_ptr[len];
    if( xbee_cksum == cksum )
    {
        // Parse the data
        uint8_t api_frame_id = api_msg->frame_data_ptr[0];
        switch( api_frame_id )
        {
            case XBEE_STATUS:
                comms_xbee_handle_status(api_msg);
                break;
            case XBEE_AT_CMD_RSP:
                comms_xbee_handle_at_rsp(api_msg);
                break;
            case XBEE_RX_MSG:
                comms_xbee_handle_rx_ipv4(api_msg);
                break;
            default:
                break;
        }
    }
}

static void comms_xbee_handle_status(comms_xbee_api_msg_t* api_msg)
{
    comms_xbee_rx_status_t* status;
    status = (comms_xbee_rx_status_t*)api_msg->frame_data_ptr;
    comms_xbee_status.xbee_state = status->status;
    if( comms_xbee_status.xbee_state == XBEE_JOINED )
    {
        // Read the configuration of the module
        comms_xbee_at_cmd_rd(XBEE_PORT);
    }

    /*
     * Try and re-establish the connection
     */
    else if( comms_xbee_status.xbee_state == XBEE_LOST_CONNECTION )
    {
        BSP_xbee_reset();
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
            comms_xbee_at_cmd_rd(XBEE_MY_IP);
        }
    }
    else if( 0 == memcmp(at_cmd,XBEE_MY_IP,sizeof(at_cmd)) )
    {
        uint8_t status = api_msg->frame_data_ptr[sizeof(at_cmd)+2];
        if( status == AT_CMD_STATUS_OK )
        {
            memcpy(comms_xbee_status.xbee_my_ip,&api_msg->frame_data_ptr[sizeof(at_cmd)+3],sizeof(comms_xbee_status.xbee_my_ip));
        }
    }
}

static void comms_xbee_handle_rx_ipv4(comms_xbee_api_msg_t* api_msg)
{
    /*
     * Read the length of the API message
     */
    uint16_t len = ((uint16_t)(api_msg->MSB_len << 8)) | (uint16_t)(api_msg->LSB_len);

    /*
     * Check to see if the current data types can handle the length
     */
    if( len >= (sizeof(comms_xbee_ipv4_rx_t) + 1) )
    {
        while(1); // critical fault
    }

    /*
     * Allocate a pointer to the message and cast it to the RX data given
     */
    comms_xbee_ipv4_rx_t* ipv4_rx_msg;
    ipv4_rx_msg = (comms_xbee_ipv4_rx_t*)api_msg->frame_data_ptr;

    /*
     * This is were the registered call backs are called!
     */
    for( uint32_t cb_idx = 0; cb_idx < xbee_cb_count; cb_idx++ )
    {
        /*
         * The message ID is set when COMMS_xbee_register_rx_cb is called.
         * It's then checked here to see if the call back should be called.
         */
        if( xbee_rx_cbs[cb_idx].msg_id == ipv4_rx_msg->rx_data[0] )
        {
            uint16_t cb_data_len = len - (sizeof(comms_xbee_ipv4_rx_t) - XBEE_MAX_IPV4_RX);
            /*
             * Note that the message ID isn't removed here so that the callback can use it.
             */
            xbee_rx_cbs[cb_idx].cb(&ipv4_rx_msg->rx_data[0],cb_data_len);
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
    comms_xbee_send_api_msg(&api_msg);
}

static void comms_xbee_send_api_msg(comms_xbee_api_msg_t* api_msg)
{
    uint16_t len = ((uint16_t)(api_msg->MSB_len << 8)) | (uint16_t)(api_msg->LSB_len);
    uint32_t header_size = sizeof(api_msg->start_delim)+sizeof(api_msg->MSB_len)+sizeof(api_msg->LSB_len);
    memset(comms_xbee_rx_buff,0x00, sizeof(comms_xbee_rx_buff));

    // Send the start delim and the message length
    BSP_xbee_write_read(&api_msg->start_delim,comms_xbee_rx_buff,header_size);

    // Send the actual message
    BSP_xbee_write_read(api_msg->frame_data_ptr,&comms_xbee_rx_buff[header_size],len);

    // Send the checksum
    BSP_xbee_write_read(&api_msg->cksum,&comms_xbee_rx_buff[header_size+len],(uint16_t)sizeof(api_msg->cksum));

    // Handle any valid data form the xbee that was received while sending the message
    comms_xbee_handle_rx_during_tx(header_size+len+sizeof(api_msg->cksum));
}

static void comms_xbee_handle_rx_during_tx( uint16_t bytes_read )
{
    if( bytes_read > sizeof(comms_xbee_rx_buff) )
    {
        // Critical error
        while(1);
    }

    comms_xbee_api_msg_t api_msg = {0};
    uint16_t msg_len = 0;
    uint8_t empty = 0;
    uint16_t header_size = sizeof(api_msg.start_delim) + sizeof(api_msg.MSB_len) + sizeof(api_msg.LSB_len);
    uint16_t cksum_size = sizeof(uint8_t);
    for( int32_t byte_idx = 0; byte_idx < bytes_read; byte_idx++ )
    {
        if( comms_xbee_rx_buff[byte_idx] == XBEE_START_DELIM )
        {
            api_msg.start_delim = comms_xbee_rx_buff[byte_idx];
            uint16_t msg_len_read = bytes_read - byte_idx;
            if( msg_len_read == 1 )
            {
                // Read the length
                BSP_xbee_write_read(&empty,&api_msg.MSB_len,2*sizeof(api_msg.MSB_len));
                msg_len = (uint16_t)api_msg.LSB_len | ((uint16_t)api_msg.MSB_len << 8);
            }
            else if( msg_len_read == 2 )
            {
                api_msg.MSB_len = comms_xbee_rx_buff[++byte_idx];
                // Read LSB_len
                BSP_xbee_write_read(&empty,&api_msg.LSB_len,sizeof(api_msg.LSB_len));
                msg_len = (uint16_t)api_msg.LSB_len | ((uint16_t)api_msg.MSB_len << 8);
            }
            // The length was read
            else if( msg_len_read >= header_size )
            {
                api_msg.MSB_len = comms_xbee_rx_buff[++byte_idx];
                api_msg.LSB_len = comms_xbee_rx_buff[++byte_idx];
                msg_len = (uint16_t)api_msg.LSB_len | ((uint16_t)api_msg.MSB_len << 8);
            }
            if( msg_len_read > header_size )
            {
                memmove(&comms_xbee_rx_buff[0],&comms_xbee_rx_buff[byte_idx+1],msg_len_read-header_size);
                if( (msg_len_read-header_size) < (msg_len + cksum_size) )
                {
                    // Read the message
                    BSP_xbee_write_read(&empty,&comms_xbee_rx_buff[msg_len_read-header_size],msg_len-msg_len_read-header_size+cksum_size);
                }
            }
            else
            {
                // Read the message
                BSP_xbee_write_read(&empty,&comms_xbee_rx_buff[0],msg_len+cksum_size);
            }

            api_msg.frame_data_ptr = &comms_xbee_rx_buff[0];
            comms_xbee_parse_rx(&api_msg);
            if( msg_len_read > msg_len + cksum_size + header_size )
            {
                memmove(&comms_xbee_rx_buff[0],&comms_xbee_rx_buff[msg_len+1],msg_len_read-header_size-msg_len-1);
                bytes_read = msg_len_read-header_size-msg_len-1;
                byte_idx = -1;  // account for the fact that the for loop is about to increment byte_idx
            }
            else
            {
                break;
            }
        }
    }
}
