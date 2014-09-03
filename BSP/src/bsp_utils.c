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
#include "driver_uart.h"
#include "type_defs.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/*******************************************************************************
 * Local Type Defs
 ******************************************************************************/

/*******************************************************************************
 * Constants
 ******************************************************************************/

/*******************************************************************************
 * Local Data
 ******************************************************************************/
UART_ID id_;

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
    id_ = Uart_init(UART_1, 115200, 8, PARITY_NONE, 1);
    Uart_enable(id_);
}

/*******************************************************************************
 * void BSP_Printf
 *
 * Description: printf that uses UART1
 *
 * Inputs:      Variable
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 03/24/2014 - Mitchell S. Tilson
 *
 ******************************************************************************/
void BSP_Printf( const int8_t* format, ... )
{
    va_list arg;
    int8_t* buffer;
    uint32_t len = 0;
		
    va_start (arg, format);
    len = vsnprintf(0,0,format,arg);
    va_end (arg);
    
    buffer = malloc(++len);

	va_start (arg, format);
    len = vsnprintf(buffer,len,format,arg);
    va_end (arg);
	
    Uart_write(id_,len,(uint8_t*)&buffer[0],TRUE);
		
    free(buffer);
}
