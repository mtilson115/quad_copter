/*
*********************************************************************************************************
*                                         Board Support Package
*
*                              (c) Copyright 2010, Micrium, Inc., Weston, FL
*                                           All Rights Reserved
*
*                                               Microchip
*                                      PIC32 Ethernet Starter Kit
*********************************************************************************************************
*/

#include "includes.h"
#include "osc.h"
#include "system.h"
#include "timer.h"
#include <cp0defs.h>
#include "app_test.h"
#include "comms_xbee.h"

/*
*********************************************************************************************************
*                                                DEFINES
*********************************************************************************************************
*/

#define  BSP_TMR_RELOAD          (BSP_CLK_FREQ / (2 * OS_CFG_TICK_RATE_HZ))
#define  BSP_DLY_CONST            BSP_CLK_FREQ / 1000000

#define  PB3_MASK                 _PORTD_RD13_MASK
#define  PB2_MASK                 _PORTD_RD7_MASK
#define  PB1_MASK                 _PORTD_RD6_MASK
#define  PB3_TRIS                 TRISDbits.TRISD13
#define  PB2_TRIS                 TRISDbits.TRISD7
#define  PB1_TRIS                 TRISDbits.TRISD6
#define  PB3_IO                   LATDbits.LATD13
#define  PB2_IO                   LATDbits.LATD7
#define  PB1_IO                   LATDbits.LATD6
#define  PB3                      PORTDbits.RD13
#define  PB2                      PORTDbits.RD7
#define  PB1                      PORTDbits.RD6

/*
*********************************************************************************************************
*                                               CONSTANTS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            LOCAL FUNCTIONS
*********************************************************************************************************
*/

static  void  LED_Init      (void);

static  void  Tmr_Init      (void);

static  void  PB_IntInit    (void);
static  void  PB_Config     (void);
static  void  PB_Init       (void);

/*
*********************************************************************************************************
*                                                 LED ON
*
* Description : This function is used to control any or all the LEDs on the board.
*
* Arguments   : led    is the number of the LED to control
*                      0    indicates that you want ALL the LEDs to be ON
*                      1    turns ON LED D0 on the board
*                      .
*                      .
*                      8    turns on LED D9 on the board
*********************************************************************************************************
*/

void  LED_On (CPU_INT08U led)
{
    switch (led) {
        case 0:
             mPORTDSetBits((IOPORT_BIT_0 | IOPORT_BIT_1 | IOPORT_BIT_2));
             break;
        case 1:
             mPORTDSetBits(IOPORT_BIT_0);
             break;
        case 2:
             mPORTDSetBits(IOPORT_BIT_1);
             break;
        case 3:
             mPORTDSetBits(IOPORT_BIT_2);
             break;
        default:
             break;
    }
}

/*
*********************************************************************************************************
*                                                LED OFF
*
* Description : This function is used to control any or all the LEDs on the board.
*
* Arguments   : led    is the number of the LED to turn OFF
*                      0    indicates that you want ALL the LEDs to be OFF
*                      1    turns OFF LED D0 on the board
*                      .
*                      .
*                      8    turns OFF LED D8 on the board
*********************************************************************************************************
*/

void  LED_Off (CPU_INT08U led)
{
    switch (led) {
        case 0:
             mPORTDClearBits((IOPORT_BIT_0 | IOPORT_BIT_1 | IOPORT_BIT_2));
             break;
        case 1:
             mPORTDClearBits(IOPORT_BIT_0);
             break;
        case 2:
             mPORTDClearBits(IOPORT_BIT_1);
             break;
        case 3:
             mPORTDClearBits(IOPORT_BIT_2);
             break;
        default:
             break;
    }
}

/*
*********************************************************************************************************
*                                              LED TOGGLE
*
* Description : This function is used to alternate the state of an LED
*
* Arguments   : led    is the number of the LED to control
*                      0    indicates that you want ALL the LEDs to toggle
*                      1    toggle LED D0 on the board
*                      .
*                      .
*                      8    toggle LED D8 on the board
*********************************************************************************************************
*/

void  LED_Toggle (CPU_INT08U led)
{
    switch (led) {
        case 0:
             mPORTDToggleBits((IOPORT_BIT_0 | IOPORT_BIT_1 | IOPORT_BIT_2));
             break;
        case 1:
             mPORTDToggleBits(IOPORT_BIT_0);
             break;
        case 2:
             mPORTDToggleBits(IOPORT_BIT_1);
             break;
        case 3:
             mPORTDToggleBits(IOPORT_BIT_2);
             break;
        default:
             break;
    }
}

/*
*********************************************************************************************************
*                                       LED_Init()
*
* Description: This function initializes all of the board's leds
*
* Arguments  : None
*
* Returns    : None
*********************************************************************************************************
*/

void  LED_Init (void)
{
    LED_Off(0);                                                               /* Turn off all of the LEDs                   */
}

/*
*********************************************************************************************************
*                                          INITIALIZE I/Os
*
* Description : This function initializes the GPIO pins.  All the I/O pins are initialized in this function
*               so you don't have to look at multiple places for I/O initialization.
*
* Arguments   : None
*
*********************************************************************************************************
*/

static  void  BSP_IO_Init (void)
{
    // No LEDs on the quad copter setup
    //mPORTDOutputConfig((IOPORT_BIT_0 | IOPORT_BIT_1 | IOPORT_BIT_2));         /* Each LED pin is set up as an output        */
}

/*
*********************************************************************************************************
*                                       Tmr_Init()
*
* Description: This function should intialize the timers used by your application
*
* Arguments  : None
*
* Returns    : None
*********************************************************************************************************
*/

void  Tmr_Init (void)
{
    OpenCoreTimer(BSP_TMR_RELOAD);                                            /* Initialize the Count and Compare registers */
    mConfigIntCoreTimer((CT_INT_ON | CT_INT_PRIOR_3));                        /* Enable the core timer's interrupt          */
}

/*
*********************************************************************************************************
*                                  BSP_TickISR_Handler()
*
* Description :
*
* Arguments   : None
*********************************************************************************************************
*/

void  BSP_TickISR_Handler (void)
{
    UpdateCoreTimer(BSP_TMR_RELOAD);                                          /* Prepare for the next core timer interrupt  */
    mCTClearIntFlag();                                                        /* Clear the core timer interrupt             */
}

/*
*********************************************************************************************************
*                                    BSP_Except_Handler()
*
* Description: This function offers a means of servicing exceptions that are not triggered by interrupts
*              or the syscall instruction.
*
* Arguments  : None
*
* Returns    : None
*********************************************************************************************************
*/

void  BSP_Except_Handler (void)
{
    uint32_t except_cause = _CP0_GET_CAUSE();
    except_cause = (except_cause & 0x7C) >> 2;
    uint32_t bad_var = _CP0_GET_BADVADDR();
    uint32_t excep_addr =_CP0_GET_EPC();
    OS_TCB* test_tcb = app_test_get_tcb();
    OS_TCB* comms_tcb = COMMS_xbee_get_tcb();
    CPU_STK* test_stk;
    app_test_get_stack( test_stk );
    switch( except_cause )
    {
        case 0x04: // addr error (load or fetch)
        case 0x05: // addr error store
        case 0x06: // Bus error instruction fetch
        case 0x07: // Bus error data reference load or store
        case 0x0A: // Reserved instruction exception
        case 0x0B: // coprocessor unusable except
        case 0x0C: // Arithmetic overflow
        case 0x0D: // Trap except
            while(1);
            break;
        default:
            return;
    }
}

/*
*********************************************************************************************************
*                                    BSP_DefaultHandler()
*
* Description: If an interrupt occurs, and no application-specific code has been written for that
*              interrupt, this function will be invoked.
*
* Arguments  : None
*
* Returns    : None
*********************************************************************************************************
*/

void  BSP_DefaultHandler (void)
{
}

/*
*********************************************************************************************************
*                                    OSProbe_TmrInit()
*
* Description: Initialize the timer used by uC/Probe.
*
* Arguments  : None
*
* Returns    : None
*********************************************************************************************************
*/

void  OSProbe_TmrInit (void)
{
    OpenTimer1(T1_ON | T1_PS_1_1 | T1_SOURCE_INT, 0);
}

/*
*********************************************************************************************************
*                                    OSProbe_TmrRd()
*
* Description: Read the value of the 32-bit timer used by uC/Probe
*
* Arguments  : None
*
* Returns    : The current 32-bit value of the timer
*********************************************************************************************************
*/

CPU_INT32U  OSProbe_TmrRd (void)
{
    return  (ReadTimer1());
}

/*
*********************************************************************************************************
*                                        PB_Init()
*
* Description: This function performs the initialization for the push buttons.
*
* Arguments  : None
*
* Returns    : None
*********************************************************************************************************
*/

static  void  PB_Init (void)
{
    /*
     * There isn't any push buttons
    PB_Config();                                                        // Configure the port pins
    PB_IntInit();                                                       // Configure interrupt settings
    */
}

/*
*********************************************************************************************************
*                                        PB_Config()
*
* Description: This function configures the port pins for the push button.
*
* Arguments  : None
*
* Returns    : None
*********************************************************************************************************
*/

static  void  PB_Config (void)
{
    PB3_TRIS = 1;                                                       /* Set the pin corresponding to our push button as input    */
    PB2_TRIS = 1;                                                       /* Set the pin corresponding to our push button as input    */
    PB1_TRIS = 1;                                                       /* Set the pin corresponding to our push button as input    */
}

/*
*********************************************************************************************************
*                                        PB_IntInit()
*
* Description: This function performs the interrupt initialization for the push button.
*
* Arguments  : None
*
* Returns    : None
*********************************************************************************************************
*/

static  void  PB_IntInit (void)
{
    CPU_INT32U  dummy_read;
    CPU_INT32U  config1;
    CPU_INT32U  config2;


    config1 = CN19_PULLUP_ENABLE;

    config2 = CHANGE_INT_ON
            | CHANGE_INT_PRI_3;

    CNCON = 0x8000;                                                     /* Enable the change notice module                          */

    EnableCN19();                                                       /* Enable change notice pin 19, tied to our push button     */

    ConfigCNPullups(config1);                                           /* Enable a weak pull-up corresponding to the CN pin        */

    dummy_read = PORTD;                                                 /* Perform a dummy read to clear any mismatch conditions    */

    mCNClearIntFlag();                                                  /* Clear the int32_t flag just in case it was triggered         */

    ConfigIntCN(config2);                                               /* Enable CN interrupts at priority level 3                 */
}

/*
*********************************************************************************************************
*                                                BSP_Dly()
*
* Description: Perform a short delay (~1uS) by performing multiple NOP.
*
* Arguments  : us           The number of uS to delay (approximate).
*
* Returns    : None
*********************************************************************************************************
*/

void  BSP_Dly (CPU_INT32U  us)
{
    CPU_INT32U  dly;


    dly = us * BSP_DLY_CONST;

    while (dly--) {
        __asm__("NOP");
    }
}

/*
*********************************************************************************************************
*                                             BSP_IntDisAll()
*
* Description: Disable all interrupts at the interrupt controller.
*
* Arguments  : None
*
* Returns    : None
*********************************************************************************************************
*/

void  BSP_IntDisAll (void)
{
    INTDisableInterrupts();
}

/*
*********************************************************************************************************
*                                      BSP_InitIntCtrl()
*
* Description: This function intializes the external interrupt controller. Note that it sets up for multivector
*               interrupts.
*
* Arguments  : None
*
* Returns    : None
*********************************************************************************************************
*/

static  void  BSP_InitIntCtrl (void)
{
    INTCONSET = 0x1000;
    INTEnableSystemMultiVectoredInt();
}

/*
*********************************************************************************************************
*                                             BSP_InitIO()
*
* Description: Initialize all the I/O devices.
*
* Arguments  : None
*
* Returns    : None
*********************************************************************************************************
*/

void  BSP_InitIO (void)
{
                                                                        /* Enable optimal performance                       */
    SYSTEMConfigPerformance(BSP_CLK_FREQ);
    mOSCSetPBDIV(OSC_PB_DIV_1);                                         /* Use 1:1 CPU Core:Peripheral clocks               */

#if JTAG_ENABLE
    DDPCONbits.JTAGEN = 1;                                              /* Maintain the port pins for JTAG use              */
#else
    DDPCONbits.JTAGEN = 0;                                              /* Free the JTAG port pins for use as GPIO          */
#endif

    BSP_InitIntCtrl();                                                  /* Initialize the interrupt controller              */
    BSP_IO_Init();                                                      /* Initialize the board's I/Os                      */
    Tmr_Init();                                                         /* Initialize the timers                            */
    //No LEDs
    //LED_Init();                                                         /* Initialize LEDs                                  */
    //No Push buttons
    //PB_Init();                                                          /* Initialize the push buttons                      */
}
