/*
*********************************************************************************************************
*                                               uC/OS-III
*                                         The Real-Time Kernel
*
*                             (c) Copyright 1998-2012, Micrium, Weston, FL
*                                          All Rights Reserved
*
*
*                                            PIC32 Sample code
*
* File : APP.C
*********************************************************************************************************
*/

#include "includes.h"
#include "app_accel_gyro.h"
#include "button_reader.h"
#include "driver_i2c.h"
#include <p32xxxx.h>

/*
*********************************************************************************************************
*                                          CONFIGURATION BITS
*********************************************************************************************************
*/

#pragma config FNOSC    = PRIPLL        // Oscillator Selection
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider (PIC32 Starter Kit: use divide by 2 only)
#pragma config FPLLMUL  = MUL_20        // PLL Multiplier
#pragma config FPLLODIV = DIV_1         // PLL Output Divider
#pragma config FPBDIV   = DIV_1         // Peripheral Clock divisor
#pragma config FWDTEN   = OFF           // Watchdog Timer
#pragma config WDTPS    = PS1           // Watchdog Timer Postscale
#pragma config FCKSM    = CSDCMD        // Clock Switching & Fail Safe Clock Monitor
#pragma config OSCIOFNC = OFF           // CLKO Enable
#pragma config POSCMOD  = XT            // Primary Oscillator
#pragma config IESO     = OFF           // Internal/External Switch-over
#pragma config FSOSCEN  = OFF           // Secondary Oscillator Enable
#pragma config CP       = OFF           // Code Protect
#pragma config BWP      = OFF           // Boot Flash Write Protect
#pragma config PWP      = OFF           // Program Flash Write Protect
#pragma config ICESEL   = ICS_PGx1      // ICE/ICD Comm Channel Select
#pragma config DEBUG    = OFF           // Debugger Disabled for Starter Kit

/*
*********************************************************************************************************
*                                                VARIABLES
*********************************************************************************************************
*/

static  OS_TCB    App_TaskStartTCB;
static  CPU_STK   App_TaskStartStk[APP_CFG_TASK_START_STK_SIZE];

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  App_TaskCreate  (void);
static  void  App_ObjCreate   (void);

static  void  App_TaskStart   (void  *p_arg);

/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.
*
* Arguments   : none
*********************************************************************************************************
*/

int  main (void)
{
    OS_ERR   os_err;

    CPU_Init();                                                           /* Initialize the uC/CPU services                           */

    BSP_IntDisAll();

    OSInit(&os_err);                                                      /* Init uC/OS-III.                                          */

    OSTaskCreate((OS_TCB      *)&App_TaskStartTCB,                        /* Create the start task                                    */
                 (CPU_CHAR    *)"Start",
                 (OS_TASK_PTR  )App_TaskStart,
                 (void        *)0,
                 (OS_PRIO      )APP_CFG_TASK_START_PRIO,
                 (CPU_STK     *)&App_TaskStartStk[0],
                 (CPU_STK_SIZE )APP_CFG_TASK_START_STK_SIZE_LIMIT,
                 (CPU_STK_SIZE )APP_CFG_TASK_START_STK_SIZE,
                 (OS_MSG_QTY   )0u,
                 (OS_TICK      )0u,
                 (void        *)0,
                 (OS_OPT       )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR      *)&os_err);

    OSStart(&os_err);                                                     /* Start multitasking (i.e. give control to uC/OS-III).     */

    (void)&os_err;

    return (0);
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
* Arguments   : p_arg   is the argument passed to 'AppStartTask()' by 'OSTaskCreate()'.
*********************************************************************************************************
*/

static  void  App_TaskStart (void *p_arg)
{
    OS_ERR  err;


    (void)p_arg;

    BSP_InitIO();                                                       /* Initialize BSP functions                                 */

    Mem_Init();                                                 /* Initialize memory managment module                   */
    Math_Init();                                                /* Initialize mathematical module                       */

#if (OS_CFG_STAT_TASK_EN > 0u)
    OSStatTaskCPUUsageInit(&err);                               /* Determine CPU capacity                               */
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif

    App_TaskCreate();                                           /* Create Application tasks                             */

    App_ObjCreate();                                            /* Create Applicaiton kernel objects                    */

    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
        // LED_Toggle(1);
        OSTimeDlyHMSM(0u, 0u, 2u, 0u,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);
    }
}

/*
*********************************************************************************************************
*                                          AppTaskCreate()
*
* Description : Create application tasks.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : AppTaskStart()
*
* Note(s)     : none.
*********************************************************************************************************
*/

static void App_TaskCreate(void)
{
    start_reader();
}


/*
*********************************************************************************************************
*                                          App_ObjCreate()
*
* Description : Create application kernel objects tasks.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : AppTaskStart()
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_ObjCreate (void)
{
}
