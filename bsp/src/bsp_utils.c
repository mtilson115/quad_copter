/*******************************************************************************
 * File:    bsp_utils.c
 * Author:  Mitchell S. Tilson
 * Created: 03/24/2014
 *
 * Description: This file provides common utilities
 ******************************************************************************/

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "bsp_utils.h"
#include <p32xxxx.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <os.h>
#include "app_cfg.h"
#if APP_CFG_COMMS_USE_UART
    #include "driver_uart.h"
#elif APP_CFG_COMMS_USE_SPI
    #include "comms_xbee.h"
#endif


/*******************************************************************************
 * Local definitions
 ******************************************************************************/

/*******************************************************************************
 * Constants
 ******************************************************************************/

/*******************************************************************************
 * Local Data
 ******************************************************************************/
static bool uart_initialized = false;
static uint8_t printf_buff[128];
#if APP_CFG_COMMS_USE_UART
    UART_ID id_;
#endif

/*******************************************************************************
 * Local Functions
 ******************************************************************************/

/*******************************************************************************
 * Public Function Section
 ******************************************************************************/

/*******************************************************************************
 * void BSP_PrintfInit
 *
 * Description: Intializes the uart for communiations on UART 1
 *
 * Inputs:      None
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 03/24/2014 - Mitchell S. Tilson
 *
 ******************************************************************************/
void BSP_PrintfInit( void )
{
#if APP_CFG_COMMS_USE_SPI
    OS_ERR err;
    while( !comms_xbee_ready() )
    {
        // Delay 100ms (this part isn't really time sensitive)
        OSTimeDlyHMSM(0u, 0u, 0u, 100u,OS_OPT_TIME_HMSM_STRICT,&err);
    }
#elif APP_CFG_COMMS_USE_UART
    id_ = Uart_init(UART_4, 115200, 8, PARITY_NONE, 1);
    Uart_enable(id_);
    uart_initialized = true;
#endif
}

/*******************************************************************************
 * void BSP_Printf
 *
 * Description: printf replacement for sending data to a peripheral
 *
 * Inputs:      Variable
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 03/24/2014 - Mitchell S. Tilson
 *
 ******************************************************************************/
void BSP_Printf( const char* format, ... )
{
    va_list arg;
    uint32_t len = 0;

    va_start (arg, format);
    len = vsnprintf((char*)&printf_buff[1],sizeof(printf_buff)-1,format,arg);
    if( len > sizeof(printf_buff) )
    {
        while(1);
    }
    va_end (arg);
#if APP_CFG_COMMS_USE_SPI
    comms_xbee_msg_t msg;
    msg.data = printf_buff;
    msg.len = len;
    comms_xbee_send(msg);
#elif APP_CFG_COMMS_USE_UART
    if( uart_initialized )
    {
        Uart_write(id_,len,&printf_buff[1]);
    }
#endif
}
