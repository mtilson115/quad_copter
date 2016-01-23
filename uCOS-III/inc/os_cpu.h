/*
*********************************************************************************************************
*                                               uC/OS-III
*                                        The Real-Time Kernel
*
*                            (c) Copyright 2010, Micrium, Inc., Weston, FL
*                                         All Rights Reserved
*
*                                               MIPS32 4K
*
*                                                 MPLAB
*
* File:    os_cpu.h
* Version: v3.00
*********************************************************************************************************
*/

#ifdef  OS_CPU_GLOBALS
#define OS_CPU_EXT
#else
#define OS_CPU_EXT  extern
#endif

/*
************************************************************************************************************************
*                                                        MACROS
************************************************************************************************************************
*/

#define  OS_TASK_SW()     __asm__ volatile("syscall");

/*
************************************************************************************************************************
*                                                TIMESTAMP CONFIGURATION
*
* Note(s) : (1) OS_TS_GET() is generally defined as CPU_TS_Get32() to allow CPU timestamp timer to be of any data type 
*               size.
*
*           (2) For architectures that provide 32-bit or higher precision free running counters 
*               (i.e. cycle count registers):
*
*               (a) OS_TS_GET() may be defined as CPU_TS_TmrRd() to improve performance when retrieving the timestamp.
*
*               (b) CPU_TS_TmrRd() MUST be configured to be greater or equal to 32-bits to avoid truncation of TS.
************************************************************************************************************************
*/

#if      OS_CFG_TS_EN == 1u
#define  OS_TS_GET()               (CPU_TS)CPU_TS_TmrRd()   /* See Note #2a.                                          */
#else
#define  OS_TS_GET()               (CPU_TS)0u
#endif

/*
************************************************************************************************************************
*                                                      PROTOTYPES
************************************************************************************************************************
*/

void  OSCtxSw              (void);
void  OSIntCtxSw           (void);
void  OSStartHighRdy       (void);

void  OS_CPU_PendSVHandler (void);


void  OS_CPU_SysTickHandler(void);
void  OS_CPU_SysTickInit   (CPU_INT32U  cnts);

