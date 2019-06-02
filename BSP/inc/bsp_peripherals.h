/*
*********************************************************************************************************
*                                         Board Support Package
*
*                             (c) Copyright 2010, Micrium, Inc., Weston, FL
*                                           All Rights Reserved
*
*                                               Microchip
*                                      PIC32 Ethernet Starter Kit
*********************************************************************************************************
*/

#include  <app_cfg.h>

/*
*********************************************************************************************************
*                                        PERIPHERAL DEFINITIONS
*********************************************************************************************************
*/
                                                                        /* Define enabled peripherals                               */
#define  _TMR1                                                       /* The RS-232 connector on the Explorer 16 is tied to UART2 */

/*
*********************************************************************************************************
*                                       PERIPHERAL CONFIGURATIONS
*********************************************************************************************************
*/

#define  BSP_BAUD_RATE  57600

#if  PROBE_COM_METHOD_RS232 == DEF_TRUE
#define  BSP_UART       BSP_UART1
#else
#define  BSP_UART       BSP_UART2
#endif

