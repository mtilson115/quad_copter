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
#include "type_defs.h"
#include "comms_xbee.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <os.h>


/*******************************************************************************
 * Local Type Defs
 ******************************************************************************/

/*******************************************************************************
 * Constants
 ******************************************************************************/

/*******************************************************************************
 * Local Data
 ******************************************************************************/

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
    OS_ERR err;
    while( !COMMS_xbee_ready() )
    {
        // Delay 100ms (this part isn't really time sensitive)
        OSTimeDlyHMSM(0u, 0u, 0u, 100u,OS_OPT_TIME_HMSM_STRICT,&err);
    }
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
static uint8_t buffer[100];
void BSP_Printf( const char* format, ... )
{
    va_list arg;
    uint32_t len = 0;

    va_start (arg, format);
    len = vsnprintf(&buffer[0],sizeof(buffer),format,arg);
    va_end (arg);

    comms_xbee_msg_t msg;
    msg.data = buffer;
    msg.len = len;
    COMMS_xbee_send(msg);
}
