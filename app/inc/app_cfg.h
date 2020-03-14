/*
*********************************************************************************************************
*                                              uC/OS-III
*                                        The Real-Time Kernel
*
*                          (c) Copyright 2004-2010; Micrium, Inc.; Weston, FL               
*
*               All rights reserved.  Protected by international copyright laws.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                       APPLICATION CONFIGURATION
*
*
* Filename      : app_cfg.h
* Version       : V1.00
* Programmer(s) : FT
*********************************************************************************************************
*/

#ifndef  APP_CFG_MODULE_PRESENT
#define  APP_CFG_MODULE_PRESENT

#include <lib_cfg.h>

/*
*********************************************************************************************************
*                                            TASK PRIORITIES
*********************************************************************************************************
*/

#define APP_CFG_TASK_START_PRIO     1u
#define APP_BUTTON_READER_PRIO      3u
#define COMMS_XBEE_PRIO             6u
#define ALG_STABILIZER_PRIO         5u


/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*********************************************************************************************************
*/

#define APP_CFG_TASK_START_STK_SIZE     512u
#define APP_BUTTON_READER_STK_SIZE      512u
#define COMMS_XBEE_STK_SIZE             512u
#define ALG_STABILIZER_STK_SIZE         512u



/*
*********************************************************************************************************
*                                            TASK STACK SIZES LIMIT
*********************************************************************************************************
*/

#define  APP_CFG_TASK_START_STK_SIZE_PCT_FULL             90u

#define  APP_CFG_TASK_START_STK_SIZE_LIMIT       (APP_CFG_TASK_START_STK_SIZE * (100u - APP_CFG_TASK_START_STK_SIZE_PCT_FULL))   / 100u

/*
*********************************************************************************************************
*                                            USER CONSTANTS
*********************************************************************************************************
*/

#define  APP_CFG_COMMS_USE_UART (1)
#define  APP_CFG_COMMS_USE_SPI  (0)

#endif

