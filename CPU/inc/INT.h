/********************************************************************
 * FileName:		INT.h
 * Dependencies:
 * Processor:		PICxx
 * Hardware:		N/A
 * Assembler:		N/A
 * Linker:		    N/A
 * Company:		    Microchip Technology, Inc.
 *
 * Software License Agreement:
 * The software supplied herewith by Microchip Technology Incorporated
 * (the “Company”) for its PICmicro® Microcontroller is intended and
 * supplied to you, the Company’s customer, for use solely and
 * exclusively on Microchip PICmicro Microcontroller products. The
 * software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *********************************************************************
 * File Description:
 *
 * EVIC Peripheral Library
 *
 * Change History:
 * Name         Date            Changes
 * S Justice    05/22/06        Initial Version
 *
 * $Id: INT.h,v 1.5 2006/11/03 16:44:10 C12923 Exp $
 * $Name:  $

 ********************************************************************/

#ifndef _PIC32INT_HEADER_FILE
#define _PIC32INT_HEADER_FILE

#include <p32xxxx.h>
#include "type_defs.h"

/*********************************************************************
 * Function:        mINTDisableInterrupts()
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    Interrupts are disabled
 *
 * Overview:        Interrupts are disabled by clearing the IE bit
 *                  in the status register
 ********************************************************************/
extern inline uint32_t __attribute__((always_inline)) INTDisableInterrupts(void) 
{
    uint32_t status = 0;
    
    __asm__ volatile("di    %0" : "=r"(status));

    return status;
}
/*********************************************************************
 * Function:        mINTEnableInterrupts()
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    Interrupts are enabled
 *
 * Overview:        Interrupts are enabled by setting the IE bit
 *                  in the status register
 ********************************************************************/
extern inline uint32_t __attribute__((always_inline)) INTEnableInterrupts(void)
{
    uint32_t status = 0;
    
    __asm__ volatile("ei    %0" : "=r"(status));

    return status;
}
/*********************************************************************
 * Function:        INTRestoreInterrupts(uint32_t status)
 *
 * PreCondition:    None
 *
 * Input:           value of the status registor
 *
 * Output:          
 *
 * Side Effects:    Interrupts are restored to previous state
 *
 * Overview:        Interrupts are enabled by setting the IE bit
 *                  in the status register
 ********************************************************************/
extern inline void __attribute__((always_inline)) INTRestoreInterrupts(uint32_t status)
{
    if(status & 0x00000001)
        __asm__ volatile("ei");
    else
        __asm__ volatile("di");
}
/*********************************************************************
 * Function:        void INTEnableSystemMultiVectoredInt(void)
 *
 * PreCondition:    EBASE and IntCtl.VS set up
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    System interrupts are turned on
 *
 * Overview:        Enables system wide multi-vectored interrupts
 ********************************************************************/
void INTEnableSystemMultiVectoredInt(void);
/*********************************************************************
 * Function:       void INTEnableSystemSingleVectoredInt(void)
 *
 * PreCondition:    EBASE and IntCtrl.VS set up (VS = 0)
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    Single vectored interrupts are turned on
 *
 * Overview:        Enables system wide single vectored interrupts
 ********************************************************************/
void INTEnableSystemSingleVectoredInt(void);
/*********************************************************************
 * Function:       void mINTDisableSystemMultiVectoredInt(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    System interrupts are disabled and will not handle
 *                  any interrupt requests
 *
 * Overview:        Disables system interrupts
 ********************************************************************/
#define mINTDisableSystemMultiVectoredInt()     INTDisableInterrupts()
/*********************************************************************
 * Function:        void INTDisableSystemSingleVectoredInt(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    System interrupts are disabled and will not handle
 *                  any interrupt requests
 *
 * Overview:        Disables system interrupts
 ********************************************************************/
void INTDisableSystemSingleVectoredInt(void);

/*********************************************************************
 * Interrupt Priority and sub-priority defines
 *********************************************************************/
#define INT_PRIORITY_DISABLED       0
#define INT_PRIORITY_LEVEL_1        1
#define INT_PRIORITY_LEVEL_2        2
#define INT_PRIORITY_LEVEL_3        3
#define INT_PRIORITY_LEVEL_4        4
#define INT_PRIORITY_LEVEL_5        5
#define INT_PRIORITY_LEVEL_6        6
#define INT_PRIORITY_LEVEL_7        7

#define INT_SUB_PRIORITY_DISABLED   0
#define INT_SUB_PRIORITY_LEVEL_0    0
#define INT_SUB_PRIORITY_LEVEL_1    1
#define INT_SUB_PRIORITY_LEVEL_2    2
#define INT_SUB_PRIORITY_LEVEL_3    3
/*********************************************************************
 * Core Timer Control Interrupt Functions
 *
 * Function:        void    mCTClearIntFlag(void)
 *					int32_t     mCTGetIntFlag(void)
 *					void    mCTSetIntEnable(void)
 *					int32_t     mCTGetIntEnable(void)
 *                  void    mCTClearIntEnable(void)
 *					void    mCTSetIntPriority(int32_t priority)
 *					int32_t     mCTGetIntPriority(void)
 *					void    mCTSetIntSubPriority(subPriority)
 *					int32_t     mCTGetIntSubPriority(void)
 *
 * PreCondition:    EBASE and IntCtl.VS set up
 *
 * Inputs:			priority:  		A value between 0 - 7 inclusive.
 *					subPriority:	A value between 0 - 3 inclusive
 *
 * Output:          For 'Get' functions, the returned values are of
 *					same type as 'Set' functions.
 *
 * Side Effects:    None
 *
 * Overview:        None
 ********************************************************************/
#define mCTClearIntFlag()                   (IFS0CLR = 1)
#define mCTGetIntFlag()                     (IFS0bits.CTIF)
#define mCTIntEnable(enable)                (IEC0SET = enable)
#define mCTGetIntEnable()                   (IEC0bits.CTIE)
#define mCTSetIntPriority(priority)         (IPC0CLR = (7 << 2), IPC0SET = (priority << 2))
#define mCTGetIntPriority()                 (IPC0bits.CTIP)
#define mCTSetIntSubPriority(subPriority)   (IPC0CLR = 3, IPC0SET = subPriority)
#define mCTGetIntSubPriority()              (IPC0bits.CTIS) 
/*********************************************************************
 * Core Software Interrupt 0 Control Functions
 *
 * Function:        void    mCS0ClearIntFlag(void)
 *					int32_t     mCS0GetIntFlag(void)
 *					void    mCS0SetIntEnable(void)
 *					int32_t     mCS0GetIntEnable(void)
 *                  void    mCS0ClearIntEnable(void)
 *					void    mCS0SetIntPriority(int32_t priority)
 *					int32_t     mCS0GetIntPriority(void)
 *					void    mCS0SetIntSubPriority(subPriority)
 *					int32_t     mCS0GetIntSubPriority(void)
 *
 * PreCondition:    EBASE and IntCtl.VS set up
 *
 * Inputs:			priority:  		A value between 0 - 7 inclusive.
 *					subPriority:	A value between 0 - 3 inclusive
 *
 * Output:          For 'Get' functions, the returned values are of
 *					same type as 'Set' functions.
 *
 * Side Effects:    None
 *
 * Overview:        None
 ********************************************************************/
#define mCS0ClearIntFlag()                  (IFS0CLR = 2)
#define mCS0GetIntFlag()                    (IFS0bits.CS0IF)
#define mCS0IntEnable(enable)               (IEC0CLR = 2, IEC0SET = (enable << 1))
#define mCS0GetIntEnable()                  (IEC0bits.CS0IE)
#define mCS0SetIntPriority(priority)        (IPC0CLR = (7 << 10), IPC0SET = (priority << 10))
#define mCS0GetIntPriority()                (IPC0bits.CS0IP)
#define mCS0SetIntSubPriority(subPriority)  (IPC0CLR = (3 << 8), IPC0SET = (subPriority << 8))
#define mCS0GetIntSubPriority()             (IPC0bits.CS0IS) 
/*********************************************************************
 * Core Software Interrupt 1 Control Functions
 *
 * Function:        void    mCS1ClearIntFlag(void)
 *					int32_t     mCS1GetIntFlag(void)
 *					void    mCS1SetIntEnable(void)
 *					int32_t     mCS1GetIntEnable(void)
 *                  void    mCS1ClearIntEnable(void)
 *					void    mCS1SetIntPriority(int32_t priority)
 *					int32_t     mCS1GetIntPriority(void)
 *					void    mCS1SetIntSubPriority(subPriority)
 *					int32_t     mCS1GetIntSubPriority(void)
 *
 * PreCondition:    EBASE and IntCtl.VS set up
 *
 * Inputs:			priority:  		A value between 0 - 7 inclusive.
 *					subPriority:	A value between 0 - 3 inclusive
 *					edgeMode: 		1 = Rising, 0 = Falling
 *
 * Output:          For 'Get' functions, the returned values are of
 *					same type as 'Set' functions.
 *
 * Side Effects:    None
 *
 * Overview:        None
 ********************************************************************/
#define mCS1ClearIntFlag()                  (IFS0CLR = 4)
#define mCS1GetIntFlag()                    (IFS0bits.CS1IF)
#define mCS1GetIntEnable()                  (IEC0bits.CS1IE)
#define mCS1IntEnable(enable)               (IEC0CLR = 4, IEC0SET = (enable << 2))
#define mCS1SetIntPriority(priority)        (IPC0CLR = (7 << 18), IPC0SET = (priority << 18))
#define mCS1GetIntPriority()                (IPC0bits.CS1IP)
#define mCS1SetIntSubPriority(subPriority)  (IPC0CLR = (3 << 16), IPC0SET = (subPriority << 16))
#define mCS1GetIntSubPriority()             (IPC0bits.CS1IS) 

/*********************************************************************
 * External INTx Control Functions
 *
 * Function:        void    mINTxClearIntFlag(void)
 *					int32_t     mINTxGetIntFlag(void)
 *					void    mINTxSetIntEnable(void)
 *					int32_t     mINTxGetIntEnable(void)
 *                  void    mINTxClearIntEnable(void)
 *					void    mINTxSetIntPriority(int32_t priority)
 *					int32_t     mINTxGetIntPriority(void)
 *					void    mINTxSetIntSubPriority(subPriority)
 *					int32_t     mINTxGetIntSubPriority(void)
 *					void    mINTxSetEdgeRising(void)
 *					void    mINTxGetEdgeFalling(void)
 *					int32_t     mINTxGetEdgeMode(void)
 *
 * PreCondition:    EBASE and IntCtl.VS set up
 *
 * Inputs:			priority:  		A value between 0 - 7 inclusive.
 *					subPriority:	A value between 0 - 3 inclusive
 *
 * Output:          For 'Get' functions, the returned values are of
 *					same type as 'Set' functions.
 *
 * Side Effects:    None
 *
 * Overview:        None
 ********************************************************************/
/*********************************************************************
 * External INT0 Control Functions
 ********************************************************************/
#define mINT0ClearIntFlag()				        (IFS0CLR = 8)
#define mINT0GetIntFlag()				        (IFS0bits.INT0IF)
#define mINT0GetIntEnable()                     (IEC0bits.INT0IF)
#define mINT0IntEnable(enable)                  (IEC0CLR = 8, IEC0SET = (enable << 3))
#define mINT0SetIntPriority(priority)		    (IPC0CLR = (7 << 26), IPC0SET = (priority << 26))
#define mINT0GetIntPriority()				    (IPC0bits.INT0IP)
#define mINT0SetIntSubPriority(subPriority)     (IPC0CLR = (3 << 24), IPC0SET = (subPriority << 24))
#define mINT0GetIntSubPriority()			    (IPC0bits.INT0IS)
#define mINT0SetEdgeMode(mode)		            (INTCONCLR = 1, INTCONSET = mode)
#define mINT0GetEdgeMode()				        (INT0CONbits.INT0EP)
/*********************************************************************
 * External INT1 Control Functions
 ********************************************************************/
#define mINT1ClearIntFlag()				        (IFS0CLR = (1 << 7))
#define mINT1GetIntFlag()				        (IFS0bits.INT1IF)
#define mINT1GetIntEnable()                     (IEC0bits.INT1IF)
#define mINT1GetBit()                           (1 << 7)
#define mINT1IntEnable(enable)                  (IEC0CLR = (1 << 7), IEC0SET = (enable << 7))
#define mINT1SetIntPriority(priority)		    (IPC1CLR = (7 << 26), IPC1SET = (priority << 26))
#define mINT1GetIntPriority()				    (IPC1bits.INT1IP)
#define mINT1SetIntSubPriority(subPriority)     (IPC1CLR = (3 << 24), IPC1SET = (subPriority << 24))
#define mINT1GetIntSubPriority()			    (IPC1bits.INT1IS)
#define mINT1SetEdgeMode(mode)		            (INTCONCLR = (1 << 1), INTCONSET = (mode << 1))
#define mINT1GetEdgeMode()				        (INT0CONbits.INT1EP)
/*********************************************************************
 * External INT2 Control Functions
 ********************************************************************/
#define mINT2ClearIntFlag()				        (IFS0CLR = (1 << 11))
#define mINT2GetIntFlag()				        (IFS0bits.INT2IF)
#define mINT2GetIntEnable()                     (IEC0bits.INT2IF)
#define mINT2GetBit()                           (1<<11)
#define mINT2IntEnable(enable)                  (IEC0CLR = (1 << 11), IEC0SET = (enable << 11))
#define mINT2SetIntPriority(priority)		    (IPC2CLR = (7 << 26), IPC2SET = (priority << 26))
#define mINT2GetIntPriority()				    (IPC2bits.INT2IP)
#define mINT2SetIntSubPriority(subPriority)     (IPC2CLR = (3 << 24), IPC2SET = (subPriority << 24))
#define mINT2GetIntSubPriority()			    (IPC2bits.INT2IS)
#define mINT2SetEdgeMode(mode)		            (INTCONCLR = (1 << 2), INTCONSET = (mode << 2))
#define mINT2GetEdgeMode()				        (INT0CONbits.INT2EP)
/*********************************************************************
 * External INT3 Control Functions
 ********************************************************************/
#define mINT3ClearIntFlag()				        (IFS0CLR = (1 << 15))
#define mINT3GetIntFlag()				        (IFS0bits.INT3IF)
#define mINT3GetIntEnable()                     (IEC0bits.INT3IF)
#define mINT3IntEnable(enable)                  (IEC0CLR = (1 << 15), IEC0SET = (enable << 15))
#define mINT3SetIntPriority(priority)		    (IPC3CLR = (7 << 26), IPC3SET = (priority << 26))
#define mINT3GetIntPriority()				    (IPC3bits.INT3IP)
#define mINT3SetIntSubPriority(subPriority)     (IPC3CLR = (3 << 24), IPC3SET = (subPriority << 24))
#define mINT3GetIntSubPriority()			    (IPC3bits.INT3IS)
#define mINT3SetEdgeMode(mode)		            (INTCONCLR = (1 << 3), INTCONSET = (mode << 3))
#define mINT3GetEdgeMode()				        (INT0CONbits.INT3EP)
/*********************************************************************
 * External INT4 Control Functions
 ********************************************************************/
#define mINT4ClearIntFlag()				        (IFS0CLR = (1 << 19))
#define mINT4GetIntFlag()				        (IFS0bits.INT4IF)
#define mINT4GetIntEnable()                     (IEC0bits.INT4IF)
#define mINT4IntEnable(enable)                  (IEC0CLR = (1 << 19), IEC0SET = (enable << 19))
#define mINT4SetIntPriority(priority)		    (IPC4CLR = (7 << 26), IPC4SET = (priority << 26))
#define mINT4GetIntPriority()				    (IPC4bits.INT4IP)
#define mINT4SetIntSubPriority(subPriority)     (IPC4CLR = (3 << 24), IPC4SET = (subPriority << 24))
#define mINT4GetIntSubPriority()			    (IPC4bits.INT4IS)
#define mINT4SetEdgeMode(mode)		            (INTCONCLR = (1 << 4), INTCONSET = (mode << 4))
#define mINT4GetEdgeMode()				        (INT0CONbits.INT4EP)

/*********************************************************************
 * Timer x Interrupt Control Functions
 *
 * Function:        void    mTxClearIntFlag(void)
 *					int32_t     mTxGetIntFlag(void)
 *					void    mTxSetIntEnable(void)
 *					int32_t     mTxGetIntEnable(void)
 *                  void    mTxClearIntEnable(void)
 *					void    mTxSetIntPriority(int32_t priority)
 *					int32_t     mTxGetIntPriority(void)
 *					void    mTxSetIntSubPriority(subPriority)
 *					int32_t     mTxGetIntSubPriority(void)
 *
 * PreCondition:    EBASE and IntCtl.VS set up
 *
 * Inputs:			priority:  		A value between 0 - 7 inclusive.
 *					subPriority:	A value between 0 - 3 inclusive
 *
 * Output:          For 'Get' functions, the returned values are of
 *					same type as 'Set' functions.
 *
 * Side Effects:    None
 *
 * Overview:        None
 ********************************************************************/
/*********************************************************************
 * Timer 1 Interrupt Control Functions
 ********************************************************************/
#define mT1ClearIntFlag()                   (IFS0CLR = (1 << 4))
#define mT1GetIntFlag()                     (IFS0bits.T1IF)
#define mT1GetIntEnable()                   (IEC0bits.T1IE)
#define mT1IntEnable(enable)                (IEC0CLR = (1 << 4), IEC0SET = ((enable) << 4))
#define mT1SetIntPriority(priority)         (IPC1CLR = (7 << 2), IPC1SET = ((priority) << 2))
#define mT1GetIntPriority()                 (IPC1bits.T1IP)
#define mT1SetIntSubPriority(subPriority)   (IPC1CLR = (3), IPC1SET = (subPriority))
#define mT1GetIntSubPriority()              (IPC1bits.T1IS) 
/*********************************************************************
 * Timer 2 Interrupt Control Functions
 ********************************************************************/
#define mT2ClearIntFlag()                   (IFS0CLR = (1 << 8))
#define mT2GetIntFlag()                     (IFS0bits.T2IF)
#define mT2GetIntEnable()                   (IEC0bits.T2IE)
#define mT2IntEnable(enable)                (IEC0CLR = (1 << 8), IEC0SET = (enable << 8))
#define mT2SetIntPriority(priority)         (IPC2CLR = (7 << 2), IPC2SET = (priority << 2))
#define mT2GetIntPriority()                 (IPC2bits.T2IP)
#define mT2SetIntSubPriority(subPriority)   (IPC2CLR = (3), IPC2SET = (subPriority))
#define mT2GetIntSubPriority()              (IPC2bits.T2IS) 
/*********************************************************************
 * Timer 3 Interrupt Control Functions
 ********************************************************************/
#define mT3ClearIntFlag()                   (IFS0CLR = (1 << 12))
#define mT3GetIntFlag()                     (IFS0bits.T3IF)
#define mT3GetIntEnable()                   (IEC0bits.T3IE)
#define mT3IntEnable(enable)                (IEC0CLR = (1 << 12), IEC0SET = (enable << 12))
#define mT3SetIntPriority(priority)         (IPC3CLR = (7 << 2), IPC3SET = (priority << 2))
#define mT3GetIntPriority()                 (IPC3bits.T3IP)
#define mT3SetIntSubPriority(subPriority)   (IPC3CLR = (3), IPC3SET = (subPriority))
#define mT3GetIntSubPriority()              (IPC3bits.T3IS) 
/*********************************************************************
 * Timer 4 Interrupt Control Functions
 ********************************************************************/
#define mT4ClearIntFlag()                   (IFS0CLR = (1 << 16))
#define mT4GetIntFlag()                     (IFS0bits.T4IF)
#define mT4GetIntEnable()                   (IEC0bits.T4IE)
#define mT4IntEnable(enable)                (IEC0CLR = (1 << 16), IEC0SET = (enable << 16))
#define mT4SetIntPriority(priority)         (IPC4CLR = (7 << 2), IPC4SET = (priority << 2))
#define mT4GetIntPriority()                 (IPC4bits.T4IP)
#define mT4SetIntSubPriority(subPriority)   (IPC4CLR = (3), IPC4SET = (subPriority))
#define mT4GetIntSubPriority()              (IPC4bits.T4IS) 
/*********************************************************************
 * Timer 5 Interrupt Control Functions
 ********************************************************************/
#define mT5ClearIntFlag()                   (IFS0CLR = (1 << 20))
#define mT5GetIntFlag()                     (IFS0bits.T5IF)
#define mT5GetIntEnable()                   (IEC0bits.T5IE)
#define mT5IntEnable(enable)                (IEC0CLR = (1 << 20), IEC0SET = (enable << 20))
#define mT5SetIntPriority(priority)         (IPC5CLR = (7 << 2), IPC5SET = (priority << 2))
#define mT5GetIntPriority()                 (IPC5bits.T5IP)
#define mT5SetIntSubPriority(subPriority)   (IPC5CLR = (3), IPC5SET = (subPriority))
#define mT5GetIntSubPriority()              (IPC5bits.T5IS) 
/*********************************************************************
 * Input Capture x Interrupt Control Functions
 *
 * Function:        void    mICxClearIntFlag(void)
 *					int32_t     mICxGetIntFlag(void)
 *					void    mICxSetIntEnable(void)
 *					int32_t     mICxGetIntEnable(void)
 *                  void    mICxClearIntEnable(void)
 *					void    mICxSetIntPriority(int32_t priority)
 *					int32_t     mICxGetIntPriority(void)
 *					void    mICxSetIntSubPriority(subPriority)
 *					int32_t     mICxGetIntSubPriority(void)
 *
 * PreCondition:    EBASE and IntCtl.VS set up
 *
 * Inputs:			priority:  		A value between 0 - 7 inclusive.
 *					subPriority:	A value between 0 - 3 inclusive
 *
 * Output:          For 'Get' functions, the returned values are of
 *					same type as 'Set' functions.
 *
 * Side Effects:    None
 *
 * Overview:        None
 ********************************************************************/
/*********************************************************************
 * Input Capture 1 Interrupt Control Functions
 ********************************************************************/
#define mIC1ClearIntFlag()                  (IFS0CLR = (1 << 5))
#define mIC1GetIntFlag()                    (IFS0bits.IC1IF)
#define mIC1GetIntEnable()                  (IEC0bits.IC1IE)
#define mIC1IntEnable(enable)               (IEC0CLR = (1 << 5), IEC0SET = (enable << 5))
#define mIC1SetIntPriority(priority)        (IPC1CLR = (7 << 10), IPC1SET = (priority << 10))
#define mIC1GetIntPriority()                (IPC1bits.IC1IP)
#define mIC1SetIntSubPriority(subPriority)  (IPC1CLR = (3 << 8), IPC1SET = (subPriority << 8))
#define mIC1GetIntSubPriority()             (IPC1bits.IC1IS) 
/*********************************************************************
 * Input Capture 2 Interrupt Control Functions
 ********************************************************************/
#define mIC2ClearIntFlag()                  (IFS0CLR = (1 << 9))
#define mIC2GetIntFlag()                    (IFS0bits.IC2IF)
#define mIC2GetIntEnable()                  (IEC0bits.IC2IE)
#define mIC2IntEnable(enable)               (IEC0CLR = (1 << 9), IEC0SET = (enable << 9))
#define mIC2SetIntPriority(priority)        (IPC2CLR = (7 << 10), IPC2SET = (priority << 10))
#define mIC2GetIntPriority()                (IPC2bits.IC2IP)
#define mIC2SetIntSubPriority(subPriority)  (IPC2CLR = (3 << 8), IPC2SET = (subPriority << 8))
#define mIC2GetIntSubPriority()             (IPC2bits.IC2IS) 
/*********************************************************************
 * Input Capture 3 Interrupt Control Functions
 ********************************************************************/
#define mIC3ClearIntFlag()                  (IFS0CLR = (1 << 13))
#define mIC3GetIntFlag()                    (IFS0bits.IC3IF)
#define mIC3GetIntEnable()                  (IEC0bits.IC3IE)
#define mIC3IntEnable(enable)               (IEC0CLR = (1 << 13), IEC0SET = (enable << 13))
#define mIC3SetIntPriority(priority)        (IPC3CLR = (7 << 10), IPC3SET = (priority << 10))
#define mIC3GetIntPriority()                (IPC3bits.IC3IP)
#define mIC3SetIntSubPriority(subPriority)  (IPC3CLR = (3 << 8), IPC3SET = (subPriority << 8))
#define mIC3GetIntSubPriority()             (IPC3bits.IC3IS) 
/*********************************************************************
 * Input Capture 4 Interrupt Control Functions
 ********************************************************************/
#define mIC4ClearIntFlag()                  (IFS0CLR = (1 << 17))
#define mIC4GetIntFlag()                    (IFS0bits.IC4IF)
#define mIC4GetIntEnable()                  (IEC0bits.IC4IE)
#define mIC4IntEnable(enable)               (IEC0CLR = (1 << 17), IEC0SET = (enable << 17))
#define mIC4SetIntPriority(priority)        (IPC4CLR = (7 << 10), IPC4SET = (priority << 10))
#define mIC4GetIntPriority()                (IPC4bits.IC4IP)
#define mIC4SetIntSubPriority(subPriority)  (IPC4CLR = (3 << 8), IPC4SET = (subPriority << 8))
#define mIC4GetIntSubPriority()             (IPC4bits.IC4IS) 
/*********************************************************************
 * Input Capture 5 Interrupt Control Functions
 ********************************************************************/
#define mIC5ClearIntFlag()                  (IFS0CLR = (1 << 21))
#define mIC5GetIntFlag()                    (IFS0bits.IC5IF)
#define mIC5GetIntEnable()                  (IEC0bits.IC5IE)
#define mIC5IntEnable(enable)               (IEC0CLR = (1 << 21), IEC0SET = (enable << 21))
#define mIC5SetIntPriority(priority)        (IPC5CLR = (7 << 10), IPC5SET = (priority << 10))
#define mIC5GetIntPriority()                (IPC5bits.IC5IP)
#define mIC5SetIntSubPriority(subPriority)  (IPC5CLR = (3 << 8), IPC5SET = (subPriority << 8))
#define mIC5GetIntSubPriority()             (IPC5bits.IC5IS) 

/*********************************************************************
 * Output Compare x Interrupt Control Functions
 *
 * Function:        void    mOCxClearIntFlag(void)
 *					int32_t     mOCxGetIntFlag(void)
 *					void    mOCxSetIntEnable(void)
 *					int32_t     mOCxGetIntEnable(void)
 *                  void    mOCxClearIntEnable(void)
 *					void    mOCxSetIntPriority(int32_t priority)
 *					int32_t     mOCxGetIntPriority(void)
 *					void    mOCxSetIntSubPriority(subPriority)
 *					int32_t     mOCxGetIntSubPriority(void)
 *
 * PreCondition:    EBASE and IntCtl.VS set up
 *
 * Inputs:			priority:  		A value between 0 - 7 inclusive.
 *					subPriority:	A value between 0 - 3 inclusive
 *
 * Output:          For 'Get' functions, the returned values are of
 *					same type as 'Set' functions.
 *
 * Side Effects:    None
 *
 * Overview:        None
 ********************************************************************/
/*********************************************************************
 * Output Compare 1 Interrupt Control Functions
 ********************************************************************/
#define mOC1ClearIntFlag()                  (IFS0CLR = (1 << 6))
#define mOC1GetIntFlag()                    (IFS0bits.OC1IF)
#define mOC1GetIntEnable()                  (IEC0bits.OC1IE)
#define mOC1IntEnable(enable)               (IEC0CLR = (1 << 6), IEC0SET = (enable << 6))
#define mOC1SetIntPriority(priority)        (IPC1CLR = (7 << 18), IPC1SET = (priority << 18))
#define mOC1GetIntPriority()                (IPC1bits.OC1IP)
#define mOC1SetIntSubPriority(subPriority)  (IPC1CLR = (3 << 16), IPC1SET = (subPriority << 16))
#define mOC1GetIntSubPriority()             (IPC1bits.OC1IS) 
/*********************************************************************
 * Output Compare 2 Interrupt Control Functions
 ********************************************************************/
#define mOC2ClearIntFlag()                  (IFS0CLR = (1 << 10))
#define mOC2GetIntFlag()                    (IFS0bits.OC2IF)
#define mOC2GetIntEnable()                  (IEC0bits.OC2IE)
#define mOC2IntEnable(enable)               (IEC0CLR = (1 << 10), IEC0SET = (enable << 10))
#define mOC2SetIntPriority(priority)        (IPC2CLR = (7 << 18), IPC2SET = (priority << 18))
#define mOC2GetIntPriority()                (IPC2bits.OC2IP)
#define mOC2SetIntSubPriority(subPriority)  (IPC2CLR = (3 << 16), IPC2SET = (subPriority << 16))
#define mOC2GetIntSubPriority()             (IPC2bits.OC2IS) 
/*********************************************************************
 * Output Compare 3 Interrupt Control Functions
 ********************************************************************/
#define mOC3ClearIntFlag()                  (IFS0CLR = (1 << 14))
#define mOC3GetIntFlag()                    (IFS0bits.OC3IF)
#define mOC3GetIntEnable()                  (IEC0bits.OC3IE)
#define mOC3IntEnable(enable)               (IEC0CLR = (1 << 14), IEC0SET = (enable << 14))
#define mOC3SetIntPriority(priority)        (IPC3CLR = (7 << 18), IPC3SET = (priority << 18))
#define mOC3GetIntPriority()                (IPC3bits.OC3IP)
#define mOC3SetIntSubPriority(subPriority)  (IPC3CLR = (3 << 16), IPC3SET = (subPriority << 16))
#define mOC3GetIntSubPriority()             (IPC3bits.OC3IS) 
/*********************************************************************
 * Output Compare 4 Interrupt Control Functions
 ********************************************************************/
#define mOC4ClearIntFlag()                  (IFS0CLR = (1 << 18))
#define mOC4GetIntFlag()                    (IFS0bits.OC4IF)
#define mOC4GetIntEnable()                  (IEC0bits.OC4IE)
#define mOC4IntEnable(enable)               (IEC0CLR = (1 << 18), IEC0SET = (enable << 18))
#define mOC4SetIntPriority(priority)        (IPC4CLR = (7 << 18), IPC4SET = (priority << 18))
#define mOC4GetIntPriority()                (IPC4bits.OC4IP)
#define mOC4SetIntSubPriority(subPriority)  (IPC4CLR = (3 << 16), IPC4SET = (subPriority << 16))
#define mOC4GetIntSubPriority()             (IPC4bits.OC4IS) 
/*********************************************************************
 * Output Compare 5 Interrupt Control Functions
 ********************************************************************/
#define mOC5ClearIntFlag()                  (IFS0CLR = (1 << 22))
#define mOC5GetIntFlag()                    (IFS0bits.OC5IF)
#define mOC5GetIntEnable()                  (IEC0bits.OC5IE)
#define mOC5IntEnable(enable)               (IEC0CLR = (1 << 22), IEC0SET = (enable << 22))
#define mOC5SetIntPriority(priority)        (IPC5CLR = (7 << 18), IPC5SET = (priority << 18))
#define mOC5GetIntPriority()                (IPC5bits.OC5IP)
#define mOC5SetIntSubPriority(subPriority)  (IPC5CLR = (3 << 16), IPC5SET = (subPriority << 16))
#define mOC5GetIntSubPriority()             (IPC5bits.OC5IS) 

/*********************************************************************
 * Change Notice Interrupt Control Functions
 *
 * Function:        void    mCNClearIntFlag(void)
 *					int32_t     mCNGetIntFlag(void)
 *					void    mCNSetIntEnable(void)
 *					int32_t     mCNGetIntEnable(void)
 *                  void    mCNClearIntEnable(void)
 *					void    mCNSetIntPriority(int32_t priority)
 *					int32_t     mCNGetIntPriority(void)
 *					void    mCNSetIntSubPriority(subPriority)
 *					int32_t     mCNGetIntSubPriority(void)
 *
 * PreCondition:    EBASE and IntCtl.VS set up
 *
 * Inputs:			priority:  		A value between 0 - 7 inclusive.
 *					subPriority:	A value between 0 - 3 inclusive
 *
 * Output:          For 'Get' functions, the returned values are of
 *					same type as 'Set' functions.
 *
 * Side Effects:    None
 *
 * Overview:        None
 ********************************************************************/
#define mCNClearIntFlag()                   (IFS1CLR = 0x00000001)
#define mCNGetIntFlag()                     (IFS1bits.CNIF)
#define mCNGetIntEnable()                   (IEC1bits.CNIE)
#define mCNIntEnable(enable)                (IEC1SET = enable)
#define mCNIntDisable(disable)              (IEC1CLR = disable)
#define mCNSetIntPriority(priority)         (IPC6SET = (priority << 18))
#define mCNGetIntPriority()                 (IPC6bits.CNIP)
#define mCNSetIntSubPriority(subPriority)   (IPC6SET = (subPriority << 16))
#define mCNGetIntSubPriority()              (IPC6bits.CNIS) 

/*********************************************************************
 * SPI x Interrupt Control Functions
 *
 * Function:        void    mSPIxEClearIntFlag(void)
 *					int32_t     mSPIxEGetIntFlag(void)
 *					void    mSPIxESetIntEnable(void)
 *					int32_t     mSPIxEGetIntEnable(void)
 *                  void    mSPIxEClearIntEnable(void)
 *                  void    mSPIxTXClearIntFlag(void)
 *					int32_t     mSPIxTXGetIntFlag(void)
 *					void    mSPIxTXSetIntEnable(void)
 *					int32_t     mSPIxTXGetIntEnable(void)
 *                  void    mSPIxTXClearIntEnable(void)
 *                  void    mSPIxRXClearIntFlag(void)
 *					int32_t     mSPIxRXGetIntFlag(void)
 *					void    mSPIxRXSetIntEnable(void)
 *					int32_t     mSPIxRXGetIntEnable(void)
 *                  void    mSPIxRXClearIntEnable(void)
 *					void    mSSPIxetPriority(int32_t priority)
 *					int32_t     mSPIxGetIntPriority(void)
 *					void    mSPIxSetIntSubPriority(subPriority)
 *					int32_t     mSPIxGetIntSubPriority(void)
 *
 * PreCondition:    EBASE and IntCtl.VS set up
 *
 * Inputs:			priority:  		A value between 0 - 7 inclusive.
 *					subPriority:	A value between 0 - 3 inclusive
 *
 * Output:          For 'Get' functions, the returned values are of
 *					same type as 'Set' functions.
 *
 * Side Effects:    None
 *
 * Overview:        None
 ********************************************************************/
/*********************************************************************
 * SPI 1 Interrupt Control Functions
 ********************************************************************/
/*********************************************************************
 * SPI 1 Error
 ********************************************************************/
#define mSPI1EClearIntFlag()                (IFS0CLR = (1 << 24))
#define mSPI1EGetIntFlag()                  (IFS0bits.SPI1EIF)
#define mSPI1EGetIntEnable()                (IEC0bits.SPI1EIE)
#define mSPI1EIntEnable(enable)             (IEC0CLR = (1 << 24), IEC0SET = (enable << 24))
/*********************************************************************
 * SPI 1 Transfer
 ********************************************************************/
#define mSPI1TXClearIntFlag()               (IFS0CLR = (1 << 25))
#define mSPI1TXGetIntFlag()                 (IFS0bits.SPI1TXIF)
#define mSPI1TXGetIntEnable()               (IEC0bits.SPI1TXIE)
#define mSPI1TXIntEnable(enable)            (IEC0CLR = (1 << 25), IEC0SET = (enable << 25))
/*********************************************************************
 * SPI 1 Receive
 ********************************************************************/
#define mSPI1RXClearIntFlag()               (IFS0CLR = (1 << 26))
#define mSPI1RXGetIntFlag()                 (IFS0bits.SPI1RXIF)
#define mSPI1RXGetIntEnable()               (IEC0bits.SPI1RXIE)
#define mSPI1RXIntEnable(enable)            (IEC0CLR = (1 << 26), IEC0SET = (enable << 26))
/*********************************************************************
 * SPI 1 Vector
 ********************************************************************/
#define mSPI1ClearAllIntFlags()              ( IFS0CLR = (7 << 24) )
#define mSPI1IntDisable()                    ( IEC0CLR = (7 << 24) )
#define mSPI1SetIntPriority(priority)        (IPC6CLR = (7 << 2), IPC6SET = (priority << 2))
#define mSPI1GetIntPriority()                (IPC6bits.SPI1IP)
#define mSPI1SetIntSubPriority(subPriority)  (IPC6CLR = (3), IPC6SET = (subPriority))
#define mSPI1GetIntSubPriority()             (IPC6bits.SPI1IS) 
/*********************************************************************
 * SPI 2 Interrupt Control Functions
 ********************************************************************/
/*********************************************************************
 * SPI 2 Error
 ********************************************************************/
#define mSPI2EClearIntFlag()                (IFS1CLR = (1 << 5))
#define mSPI2EGetIntFlag()                  (IFS1bits.SPI2EIF)
#define mSPI2EGetIntEnable()                (IEC1bits.SPI2EIE)
#define mSPI2EIntEnable(enable)             (IEC1CLR = (1 << 5), IEC1SET = (enable << 5))
/*********************************************************************
 * SPI 2 Transfer
 ********************************************************************/
#define mSPI2TXClearIntFlag()               (IFS1CLR = (1 << 6))
#define mSPI2TXGetIntFlag()                 (IFS1bits.SPI2TXIF)
#define mSPI2TXGetIntEnable()               (IEC1bits.SPI2TXIE)
#define mSPI2TXIntEnable(enable)            (IEC1CLR = (1 << 6), IEC1SET = (enable << 6))
/*********************************************************************
 * SPI 2 Receive
 ********************************************************************/
#define mSPI2RXClearIntFlag()               (IFS1CLR = (1 << 7))
#define mSPI2RXGetIntFlag()                 (IFS1bits.SPI2RXIF)
#define mSPI2RXGetIntEnable()               (IEC1bits.SPI2RXIE)
#define mSPI2RXIntEnable(enable)            (IEC1CLR = (1 << 7), IEC1SET = (enable << 7))
/*********************************************************************
 * SPI 2 Vector
 ********************************************************************/
#define mSPI2ClearAllIntFlags()              ( IFS1CLR = (7 << 5) )
#define mSPI2IntDisable()                    ( IEC1CLR = (7 << 5) )
#define mSPI2SetIntPriority(priority)        ( IPC7CLR = (7 << 26), IPC7SET = (priority << 26))
#define mSPI2GetIntPriority()                (IPC7bits.SPI2IP)
#define mSPI2SetIntSubPriority(subPriority)  ( IPC7CLR = (3 << 24), IPC7SET = (subPriority << 24))
#define mSPI2GetIntSubPriority()             (IPC7bits.SPI2IS) 

/*********************************************************************
 * UART x Interrupt Control Functions
 *
 * Function:        void    mUxEClearIntFlag(void)
 *					int32_t     mUxEGetIntFlag(void)
 *					void    mUxESetIntEnable(void)
 *					int32_t     mUxEGetIntEnable(void)
 *                  void    mUxEClearIntEnable(void)
 *                  void    mUxTXClearIntFlag(void)
 *					int32_t     mUxTXGetIntFlag(void)
 *					void    mUxTXSetIntEnable(void)
 *					int32_t     mUxTXGetIntEnable(void)
 *                  void    mUxTXClearIntEnable(void)
 *                  void    mUxRXClearIntFlag(void)
 *					int32_t     mUxRXGetIntFlag(void)
 *					void    mUxRXSetIntEnable(void)
 *					int32_t     mUxRXGetIntEnable(void)
 *                  void    mUxRXClearIntEnable(void)
 *					void    mUxSetIntPriority(int32_t priority)
 *					int32_t     mUxGetIntPriority(void)
 *					void    mUxSetIntSubPriority(subPriority)
 *					int32_t     mUxGetIntSubPriority(void)
 *
 * PreCondition:    EBASE and IntCtl.VS set up
 *
 * Inputs:			priority:  		A value between 0 - 7 inclusive.
 *					subPriority:	A value between 0 - 3 inclusive
 *
 * Output:          For 'Get' functions, the returned values are of
 *					same type as 'Set' functions.
 *
 * Side Effects:    None
 *
 * Overview:        None
 ********************************************************************/
/*********************************************************************
 * UART 1 Interrupt Control Functions
 ********************************************************************/
/*********************************************************************
 * UART 1 General
 ********************************************************************/
#define mU1IntEnable(enable)                (IEC0SET = (enable << 27))
#define mU1IntDisable(enable)               (IEC0CLR = (enable << 27))
/*********************************************************************
 * UART 1 Error
 ********************************************************************/
#define mU1EClearIntFlag()                  (IFS0CLR = (1 << 27))
#define mU1EGetIntFlag()                    (IFS0bits.U1EIF)
#define mU1EGetIntEnable()                  (IEC0bits.U1EIE)
#define mU1EIntEnable(enable)               (IEC0CLR = (1 << 27), IEC0SET = (enable << 27))
/*********************************************************************
 * UART 1 Receive
 ********************************************************************/
#define mU1RXClearIntFlag()                 (IFS0CLR = (1 << 28))
#define mU1RXGetIntFlag()                   (IFS0bits.U1RXIF)
#define mU1RXGetIntEnable()                 (IEC0bits.U1RXIF)
#define mU1RXIntEnable(enable)              (IEC0CLR = (1 << 28), IEC0SET = (enable << 28))
/*********************************************************************
 * UART 1 Transfer
 ********************************************************************/
#define mU1TXClearIntFlag()                 (IFS0CLR = (1 << 29))
#define mU1TXGetIntFlag()                   (IFS0bits.U1TXIF)
#define mU1TXGetIntEnable()                 (IEC0bits.U1TXIE)
#define mU1TXIntEnable(enable)              (IEC0CLR = (1 << 29), IEC0SET = (enable << 29))
/*********************************************************************
 * UART 1 Vector
 ********************************************************************/
#define mU1ClearAllIntFlags()               ( IFS0CLR = (7 << 27) )
#define mU1IntDisableAll()                  ( IEC0CLR = (7 << 27) )
#define mU1SetIntPriority(priority)         ( IPC6CLR = (7 << 10), IPC6SET = (priority << 10))
#define mU1GetIntPriority()                 (IPC6bits.U1IP)
#define mU1SetIntSubPriority(subPriority)   ( IPC6CLR = (3 << 8), IPC6SET = (subPriority << 8))
#define mU1GetIntSubPriority()              (IPC6bits.U1IS) 
/*********************************************************************
 * UART 2 Interrupt Control Functions
 ********************************************************************/
/*********************************************************************
 * UART 2 General
 ********************************************************************/
#define mU2IntEnable(enable)                (IEC1SET = (enable << 8))
#define mU2IntDisable(enable)               (IEC1CLR = (enable << 8))
/*********************************************************************
 * UART 2 Error
 ********************************************************************/
#define mU2EClearIntFlag()                  (IFS1CLR = (1 << 8))
#define mU2EGetIntFlag()                    (IFS1bits.U2EIF)
#define mU2EGetIntEnable()                  (IEC1bits.U2EIE)
#define mU2EIntEnable(enable)               (IEC1CLR = (1 << 8), IEC1SET = (enable << 8))
/*********************************************************************
 * UART 2 Receive
 ********************************************************************/
#define mU2RXClearIntFlag()                 (IFS1CLR = (1 << 9))
#define mU2RXGetIntFlag()                   (IFS1bits.U2RXIF)
#define mU2RXGetIntEnable()                 (IEC1bits.U2RXIE)
#define mU2RXIntEnable(enable)              (IEC1CLR = (1 << 9), IEC1SET = (enable << 9))
/********************************************************************
 * UART 2 Transfer
 ********************************************************************/
#define mU2TXClearIntFlag()                 (IFS1CLR = (1 << 10))
#define mU2TXGetIntFlag()                   (IFS1bits.U2TXIF)
#define mU2TXGetIntEnable()                 (IEC1bits.U2TXIE)
#define mU2TXIntEnable(enable)              (IEC1CLR = (1 << 10), IEC1SET = (enable << 10))
/*********************************************************************
 * UART 2 Vector
 ********************************************************************/
#define mU2ClearAllIntFlags()              ( IFS1CLR = (7 << 8) )
#define mU2IntDisableAll()                 ( IEC1CLR = (7 << 8) )
#define mU2SetIntPriority(priority)        ( IPC8CLR = (7 << 2), IPC8SET = (priority << 2))
#define mU2GetIntPriority()                (IPC8bits.U2IP)
#define mU2SetIntSubPriority(subPriority)  ( IPC8CLR = (3), IPC8SET = (subPriority))
#define mU2GetIntSubPriority()             (IPC8bits.U2IS) 
/*********************************************************************
 * I2C x Interrupt Control Functions
 *
 * Function:        void    mI2CxBClearIntFlag(void)
 *					int32_t     mI2CxBGetIntFlag(void)
 *					void    mI2CxBSetIntEnable(void)
 *					int32_t     mI2CxBGetIntEnable(void)
 *                  void    mI2CxBClearIntEnable(void)
 *                  void    mI2CxSClearIntFlag(void)
 *					int32_t     mI2CxSGetIntFlag(void)
 *					void    mI2CxSSetIntEnable(void)
 *					int32_t     mI2CxSGetIntEnable(void)
 *                  void    mI2CxSClearIntEnable(void)
 *                  void    mI2CxMClearIntFlag(void)
 *					int32_t     mI2CxMGetIntFlag(void)
 *					void    mI2CxMSetIntEnable(void)
 *					int32_t     mI2CxMGetIntEnable(void)
 *                  void    mI2CxMClearIntEnable(void)
 *					void    mI2CxSetIntPriority(int32_t priority)
 *					int32_t     mI2CxGetIntPriority(void)
 *					void    mI2CxSetIntSubPriority(subPriority)
 *					int32_t     mI2CxGetIntSubPriority(void)
 *
 * PreCondition:    EBASE and IntCtl.VS set up
 *
 * Inputs:			priority:  		A value between 0 - 7 inclusive.
 *					subPriority:	A value between 0 - 3 inclusive
 *
 * Output:          For 'Get' functions, the returned values are of
 *					same type as 'Set' functions.
 *
 * Side Effects:    None
 *
 * Overview:        None
 ********************************************************************/
/*********************************************************************
 * I2C 1 Interrupt Control Functions
 ********************************************************************/
/*********************************************************************
 * I2C 1 Bus Colision
 ********************************************************************/
#define mI2C1BClearIntFlag()                (IFS0CLR = (1 << 30))
#define mI2C1BGetIntFlag()                  (IFS0bits.I2C1BCIF)
#define mI2C1BGetIntEnable()                (IEC0bits.I2C1BCIE)
#define mI2C1BIntEnable(enable)             (IEC0CLR = (1 << 30), IEC0SET = (enable << 30))
/*********************************************************************
 * I2C 1 Slave
 ********************************************************************/
#define mI2C1SClearIntFlag()                (IFS0CLR = (1 << 31))
#define mI2C1SGetIntFlag()                  (IFS0bits.I2C1SIF)
#define mI2C1SGetIntEnable()                (IEC0bits.I2C1SIE)
#define mI2C1SIntEnable(enable)             (IEC0CLR = (1 << 31), IEC0SET = (enable << 31))
/*********************************************************************
 * I2C 1 Master
 ********************************************************************/
#define mI2C1MClearIntFlag()                (IFS1CLR = (1))
#define mI2C1MGetIntFlag()                  (IFS1bits.I2C1MIF)
#define mI2C1MGetIntEnable()                (IEC1bits.I2C1MIE)
#define mI2C1MIntEnable(enable)             (IEC1CLR = (1), IEC1SET = (enable))
/*********************************************************************
 * I2C 1 Vector
 ********************************************************************/
#define mI2C1ClearAllIntFlags()             ( IFS0CLR = (3 << 30), IFS1CLR = 1)
#define mI2C1IntDisable()                   ( IEC0CLR = (3 << 30), IEC1CLR = 1)
#define mI2C1SetIntPriority(priority)       ( IPC6CLR = (7 << 18), IPC6SET = (priority << 18))
#define mI2C1GetIntPriority()               (IPC6bits.I2C1IS)
#define mI2C1SetIntSubPriority(subPriority) ( IPC6CLR = (3 << 16), IPC6SET = (subPriority << 16))
#define mI2C1GetIntSubPriority()            (IPC6bits.I2C1IS) 
/*********************************************************************
 * I2C 2 Interrupt Control Functions
 ********************************************************************/
/*********************************************************************
 * I2C 2 Bus Colision
 ********************************************************************/
#define mI2C2BClearIntFlag()                (IFS1CLR = (1 << 11))
#define mI2C2BGetIntFlag()                  (IFS1bits.I2C2BCIF)
#define mI2C2BGetIntEnable()                (IEC1bits.I2C2BCIE)
#define mI2C2BIntEnable(enable)             (IEC1CLR = (1 << 11), IEC1SET = (enable << 11))
/*********************************************************************
 * I2C 2 Slave
 ********************************************************************/
#define mI2C2SClearIntFlag()                (IFS1CLR = (1 << 12))
#define mI2C2SGetIntFlag()                  (IFS1bits.I2C2SIF)
#define mI2C2SGetIntEnable()                (IEC1bits.I2C2SIE)
#define mI2C2SIntEnable(enable)             (IEC1CLR = (1 << 12), IEC1SET = (enable << 12))
/*********************************************************************
 * I2C 2 Master
 ********************************************************************/
#define mI2C2MClearIntFlag()                (IFS1CLR = (1 << 13))
#define mI2C2MGetIntFlag()                  (IFS1bits.I2C2MIF)
#define mI2C2MGetIntEnable()                (IEC1bits.I2C2MIE)
#define mI2C2MIntEnable(enable)             (IEC1CLR = (1 << 13),  IEC1SET = (enable << 13))
/*********************************************************************
 * I2C 2 Vector
 ********************************************************************/
#define mI2C2ClearAllIntFlags()             ( IFS1CLR = (7<< 11) )
#define mI2C2IntDisable()                   ( IEC1CLR = (7 << 11) )
#define mI2C2SetIntPriority(priority)       ( IPC8CLR = (7 << 10), IPC8SET = (priority << 10))
#define mI2C2GetIntPriority()               (IPC8bits.I2C2IS)
#define mI2C2SetIntSubPriority(subPriority) ( IPC8CLR = (3 << 8), IPC8SET = (subPriority << 8))
#define mI2C2GetIntSubPriority()            (IPC8bits.I2C2IS) 
/*********************************************************************
 * Analog to Digital 1 Interrupt Control Functions
 *
 * Function:        void    mAD1ClearIntFlag(void)
 *					int32_t     mAD1GetIntFlag(void)
 *					void    mAD1SetIntEnable(void)
 *					int32_t     mAD1GetIntEnable(void)
 *                  void    mAD1ClearIntEnable(void)
 *					void    mAD1SetIntPriority(int32_t priority)
 *					int32_t     mAD1GetIntPriority(void)
 *					void    mAD1SetIntSubPriority(subPriority)
 *					int32_t     mGAD1etSubPriority(void)
 *
 * PreCondition:    EBASE and IntCtl.VS set up
 *
 * Inputs:			priority:  		A value between 0 - 7 inclusive.
 *					subPriority:	A value between 0 - 3 inclusive
 *
 * Output:          For 'Get' functions, the returned values are of
 *					same type as 'Set' functions.
 *
 * Side Effects:    None
 *
 * Overview:        None
 ********************************************************************/
#define mAD1ClearIntFlag()                  (IFS1CLR = (1 << 1))
#define mAD1GetIntFlag()                    (IFS1bits.AD1IF)
#define mAD1GetIntEnable()                  (IEC1bits.AD1IE)
#define mAD1IntEnable(enable)               ( IEC1CLR = (1 << 1), IEC1SET = (enable << 1))
#define mAD1SetIntPriority(priority)        ( IPC6CLR = (7 << 26), IPC6SET = (priority << 26))
#define mAD1GetIntPriority()                (IPC6bits.AD1IP)
#define mAD1SetIntSubPriority(subPriority)  ( IPC6CLR = (3 << 24), IPC6SET = (subPriority << 24))
#define mAD1GetIntSubPriority()             (IPC6bits.AD1IS) 
/*********************************************************************
 * Parallel master Port Interrupt Control Functions
 *
 * Function:        void    mPMPClearIntFlag(void)
 *					int32_t     mPMPGetIntFlag(void)
 *					void    mPMPSetIntEnable(void)
 *					int32_t     mPMPGetIntEnable(void)
 *                  void    mPMPClearIntEnable(void)
 *					void    mPMPSetIntPriority(int32_t priority)
 *					int32_t     mPMPGetIntPriority(void)
 *					void    mPMPSetIntSubPriority(subPriority)
 *					int32_t     mPMPGetIntSubPriority(void)
 *
 * PreCondition:    EBASE and IntCtl.VS set up
 *
 * Inputs:			priority:  		A value between 0 - 7 inclusive.
 *					subPriority:	A value between 0 - 3 inclusive
 *
 * Output:          For 'Get' functions, the returned values are of
 *					same type as 'Set' functions.
 *
 * Side Effects:    None
 *
 * Overview:        None
 ********************************************************************/
#define mPMPClearIntFlag()                  (IFS1CLR = (1 << 2))
#define mPMPGetIntFlag()                    (IFS1bits.PMPIF)
#define mPMPGetIntEnable()                  (IEC1bits.PMPIE)
#define mPMPIntEnable(enable)               ( IEC1CLR = (1 << 2), IEC1SET = (enable << 2))
#define mPMPSetIntPriority(priority)        ( IPC7CLR = (7 << 2), IPC7SET = (priority << 2))
#define mPMPGetIntPriority()                (IPC7bits.PMPIP)
#define mPMPSetIntSubPriority(subPriority)  ( IPC7CLR = (3), IPC7SET = (subPriority))
#define mPMPGetIntSubPriority()             (IPC7bits.PMPIS) 

/*********************************************************************
 * Comparator x Interrupt Control Functions
 *
 * Function:        void    mCMPxClearIntFlag(void)
 *					int32_t     mCMPxGetIntFlag(void)
 *					void    mCMPxSetIntEnable(void)
 *					int32_t     mCMPxGetIntEnable(void)
 *                  void    mCMPxClearIntEnable(void)
 *					void    mCMPxxSetIntPriority(int32_t priority)
 *					int32_t     mCMPxxGetIntPriority(void)
 *					void    mCMPxxSetIntSubPriority(subPriority)
 *					int32_t     mCMPxxGetIntSubPriority(void)
 *
 * PreCondition:    EBASE and IntCtl.VS set up
 *
 * Inputs:			priority:  		A value between 0 - 7 inclusive.
 *					subPriority:	A value between 0 - 3 inclusive
 *
 * Output:          For 'Get' functions, the returned values are of
 *					same type as 'Set' functions.
 *
 * Side Effects:    None
 *
 * Overview:        None
 ********************************************************************/
/*********************************************************************
 * Comparator 1 and 2 Interrupt Control Functions
 ********************************************************************/
/*********************************************************************
 * Comparator 1
 ********************************************************************/
#define mCMP1ClearIntFlag()                     (IFS1CLR = (1 << 3))
#define mCMP1GetIntFlag()                       (IFS1bits.CMP1IF)
#define mCMP1GetIntEnable()                     (IEC1bits.CMP1IE)
#define mCMP1IntEnable(enable)                  (IEC1CLR = (1 << 3), IEC1SET = (enable << 3))
#define mCMP1SetIntPriority(priority)           ( IPC7CLR = (7 << 10), IPC7SET = (priority << 10))
#define mCMP1GetIntPriority()                   (IPC7bits.CMP1IP)
#define mCMP1SetIntSubPriority(subPriority)     ( IPC7CLR = (3 << 8), IPC7SET = (subPriority << 8))
#define mCMP1GetIntSubPriority()                (IPC7bits.CMP1IS) 
/*********************************************************************
 * Comparator 2
 ********************************************************************/
#define mCMP2ClearIntFlag()                     (IFS1CLR = (1 << 4))
#define mCMP2GetIntFlag()                       (IFS1bits.CMP2IF)
#define mCMP2GetIntEnable()                     (IEC1bits.CMP2IE)
#define mCMP2IntEnable(enable)                  (IEC1CLR = (1 << 4), IEC1SET = (enable << 4))
#define mCMP2SetIntPriority(priority)           ( IPC7CLR = (7 << 18), IPC7SET = (priority << 18))
#define mCMP2GetIntPriority()                   (IPC7bits.CMP2IP)
#define mCMP2SetIntSubPriority(subPriority)     ( IPC7CLR = (3 << 16), IPC7SET = (subPriority << 16))
#define mCMP2GetIntSubPriority()                (IPC7bits.CMP2IS) 
/*********************************************************************
 * Fail Safe Clock Monitor Interrupt Control Functions
 *
 * Function:        void    mFSCMClearIntFlag(void)
 *					int32_t     mFSCMGetIntFlag(void)
 *					void    mFSCMSetIntEnable(void)
 *					int32_t     mFSCMGetIntEnable(void)
 *                  void    mFSCMClearIntEnable(void)
 *					void    mFSCMSetIntPriority(int32_t priority)
 *					int32_t     mFSCMGetIntPriority(void)
 *					void    mFSCMSetIntSubPriority(subPriority)
 *					int32_t     mFSCMGetIntSubPriority(void)
 *
 * PreCondition:    EBASE and IntCtl.VS set up
 *
 * Inputs:			priority:  		A value between 0 - 7 inclusive.
 *					subPriority:	A value between 0 - 3 inclusive
 *
 * Output:          For 'Get' functions, the returned values are of
 *					same type as 'Set' functions.
 *
 * Side Effects:    None
 *
 * Overview:        None
 ********************************************************************/
#define mFSCMClearIntFlag()                 (IFS1CLR = (1 << 14))
#define mFSCMGetIntFlag()                   (IFS1bits.FSCMIF)
#define mFSCMGetIntEnable()                 (IEC1bits.FSCMIE)
#define mFSCMIntEnable(enable)              (IEC1CLR = (1 << 14), IEC1SET = (enable << 14))
#define mFSCMSetIntPriority(priority)       (IPC8CLR = (7 << 18), IPC8SET = (priority << 18))
#define mFSCMGetIntPriority()               (IPC8bits.FSCMIP)
#define mFSCMSetIntSubPriority(subPriority) (IPC8CLR = (3 << 16), IPC8SET = (subPriority << 16))
#define mFSCMGetIntSubPriority()            (IPC8bits.FSCMIS) 

/*********************************************************************
 * Flash Control Event Interrupt Control Functions
 *
 * Function:        void    mFCEClearIntFlag(void)
 *					int32_t     mFCEGetIntFlag(void)
 *					void    mFCESetIntEnable(void)
 *					int32_t     mFCEGetIntEnable(void)
 *                  void    mFCEClearIntEnable(void)
 *					void    mFCESetIntPriority(int32_t priority)
 *					int32_t     mFCEGetIntPriority(void)
 *					void    mFCESetIntSubPriority(subPriority)
 *					int32_t     mFCEGetIntSubPriority(void)
 *
 * PreCondition:    EBASE and IntCtl.VS set up
 *
 * Inputs:			priority:  		A value between 0 - 7 inclusive.
 *					subPriority:	A value between 0 - 3 inclusive
 *
 * Output:          For 'Get' functions, the returned values are of
 *					same type as 'Set' functions.
 *
 * Side Effects:    None
 *
 * Overview:        None
 ********************************************************************/
#define mFCEClearIntFlag()                 (IFS1CLR = (1 << 15))
#define mFCEGetIntFlag()                   (IFS1bits.FCEIF)
#define mFCEGetIntEnable()                 (IEC1bits.FCEIE)
#define mFCEIntEnable(enable)              (IEC1CLR = (1 << 15), IEC1SET = (enable << 15))
#define mFCESetIntPriority(priority)       (IPC8CLR = (7 << 26), IPC8SET = (priority << 26))
#define mFCEGetIntPriority()               (IPC8bits.FCEIP)
#define mFCESetIntSubPriority(subPriority) (IPC8CLR = (3 << 24), IPC8SET = (subPriority << 24))
#define mFCEGetIntSubPriority()            (IPC8bits.FCEIS) 

/*********************************************************************
 * Real Time Clock Interrupt Control Functions
 *
 * Function:        void    mRTCCClearIntFlag(void)
 *					int32_t     mRTCCGetIntFlag(void)
 *					void    mRTCCSetIntEnable(void)
 *					int32_t     mRTCCGetIntEnable(void)
 *                  void    mRTCCClearIntEnable(void)
 *					void    mRTCCSetIntPriority(int32_t priority)
 *					int32_t     mRTCCGetIntPriority(void)
 *					void    mRTCCSetIntSubPriority(subPriority)
 *					int32_t     mRTCCGetIntSubPriority(void)
 *
 * PreCondition:    EBASE and IntCtl.VS set up
 *
 * Inputs:			priority:  		A value between 0 - 7 inclusive.
 *					subPriority:	A value between 0 - 3 inclusive
 *
 * Output:          For 'Get' functions, the returned values are of
 *					same type as 'Set' functions.
 *
 * Side Effects:    None
 *
 * Overview:        None
 ********************************************************************/
#define mRTCCClearIntFlag()                 (IFS1CLR = (1 << 16))
#define mRTCCGetIntFlag()                   (IFS1bits.RTCCIF)
#define mRTCCGetIntEnable()                 (IEC1bits.RTCCIE)
#define mRTCCIntEnable(enable)              (IEC1CLR = (1 << 16), IEC1SET = (enable << 16))
#define mRTCCSetIntPriority(priority)       (IPC9CLR = (7 << 2), IPC9SET = (priority << 2))
#define mRTCCGetIntPriority()               (IPC9bits.RTCCIP)
#define mRTCCSetIntSubPriority(subPriority) (IPC9CLR = (3), IPC9SET = (subPriority))
#define mRTCCGetIntSubPriority()            (IPC9bits.RTCCIS) 
/*********************************************************************
 * DMA x Channel Interrupt Control Functions
 *
 * Function:        void    mDMAxClearIntFlag(void)
 *					int32_t     mDMAxGetIntFlag(void)
 *					void    mDMAxSetIntEnable(void)
 *					int32_t     mDMAxGetIntEnable(void)
 *                  void    mDMAxClearIntEnable(void)
 *					void    mDMAxSetIntPriority(int32_t priority)
 *					int32_t     mDMAxGetIntPriority(void)
 *					void    mDMAxSetIntSubPriority(subPriority)
 *					int32_t     mDMAxGetIntSubPriority(void)
 *
 * PreCondition:    EBASE and IntCtl.VS set up
 *
 * Inputs:			priority:  		A value between 0 - 7 inclusive.
 *					subPriority:	A value between 0 - 3 inclusive
 *
 * Output:          For 'Get' functions, the returned values are of
 *					same type as 'Set' functions.
 *
 * Side Effects:    None
 *
 * Overview:        None
 ********************************************************************/
/*********************************************************************
 * DMA 0
 ********************************************************************/
#define mDMA0ClearIntFlag()                     (IFS1CLR = (1 << 17))
#define mDMA0GetIntFlag()                       (IFS1bits.DMA0IF)
#define mDMA0GetIntEnable()                     (IEC1bits.DMA0IE)
#define mDMA0IntEnable(enable)                  (IEC1CLR = (1 << 17), IEC1SET = (enable << 17))
#define mDMA0SetIntPriority(priority)           (IPC9CLR = (7 << 10), IPC9SET = (priority << 10))
#define mDMA0GetIntPriority()                   (IPC9bits.DMA0IP)
#define mDMA0SetIntSubPriority(subPriority)     (IPC9CLR = (3 << 8), IPC9SET = (subPriority << 8))
#define mDMA0GetIntSubPriority()                (IPC9bits.DMA0IS) 
/*********************************************************************
 * DMA 1
 ********************************************************************/
#define mDMA1ClearIntFlag()                     (IFS1CLR = (1 << 18))
#define mDMA1GetIntFlag()                       (IFS1bits.DMA1IF)
#define mDMA1GetIntEnable()                     (IEC1bits.DMA1IE)
#define mDMA1IntEnable(enable)                  (IEC1CLR = (1 << 18), IEC1SET = (enable << 18))
#define mDMA1SetIntPriority(priority)           (IPC9CLR = (7 << 18), IPC9SET = (priority << 18))
#define mDMA1GetIntPriority()                   (IPC9bits.DMA1IP)
#define mDMA1SetIntSubPriority(subPriority)     (IPC9CLR = (3 << 16), IPC9SET = (subPriority << 16))
#define mDMA1GetIntSubPriority()                (IPC9bits.DMA1IS) 
/*********************************************************************
 * DMA 2
 ********************************************************************/
#define mDMA2ClearIntFlag()                     (IFS1CLR = (1 << 19))
#define mDMA2GetIntFlag()                       (IFS1bits.DMA2IF)
#define mDMA2GetIntEnable()                     (IEC1bits.DMA2IE)
#define mDMA2IntEnable(enable)                  (IEC1CLR = (1 << 19), IEC1SET = (enable << 19))
#define mDMA2SetIntPriority(priority)           (IPC9CLR = (7 << 26), IPC9SET = (priority << 26))
#define mDMA2GetIntPriority()                   (IPC9bits.DMA2IP)
#define mDMA2SetIntSubPriority(subPriority)     (IPC9CLR = (3 << 24), IPC9SET = (subPriority << 24))
#define mDMA2GetIntSubPriority()                (IPC9bits.DMA2IS) 
/*********************************************************************
 * DMA 3
 ********************************************************************/
#define mDMA3ClearIntFlag()                     (IFS1CLR = (1 << 20))
#define mDMA3GetIntFlag()                       (IFS1bits.DMA3IF)
#define mDMA3GetIntEnable()                     (IEC1bits.DMA3IE)
#define mDMA3IntEnable(enable)                  (IEC1CLR = (1 << 20),    IEC1SET = (enable << 20))
#define mDMA3SetIntPriority(priority)           (IPC10CLR = (7 << 2), IPC10SET = (priority << 2))
#define mDMA3GetIntPriority()                   (IPC10bits.DMA3IP)
#define mDMA3SetIntSubPriority(subPriority)     (IPC10CLR = (3), IPC10SET = (subPriority))
#define mDMA3GetIntSubPriority()                (IPC10bits.DMA3IS) 
/*********************************************************************
 * DMA 4
 ********************************************************************/
#define mDMA4ClearIntFlag()                     (IFS1CLR = (1 << 21))
#define mDMA4GetIntFlag()                       (IFS1bits.DMA4IF)
#define mDMA4GetIntEnable()                     (IEC1bits.DMA4IE)
#define mDMA4IntEnable(enable)                  (IEC1CLR = (1 << 21), IEC1SET = (enable << 21))
#define mDMA4SetIntPriority(priority)           (IPC10CLR = (7 << 10), IPC10SET = (priority << 10))
#define mDMA4GetIntPriority()                   (IPC10bits.DMA4IP)
#define mDMA4SetIntSubPriority(subPriority)     (IPC10CLR = (3 << 8), IPC10SET = (subPriority << 8))
#define mDMA4GetIntSubPriority()                (IPC10bits.DMA4IS) 
/*********************************************************************
 * DMA 5
 ********************************************************************/
#define mDMA5ClearIntFlag()                     (IFS1CLR = (1 << 22))
#define mDMA5GetIntFlag()                       (IFS1bits.DMA5IF)
#define mDMA5GetIntEnable()                     (IEC1bits.DMA5IE)
#define mDMA5IntEnable(enable)                  (IEC1CLR = (1 << 22), IEC1SET = (enable << 22))
#define mDMA5SetIntPriority(priority)           (IPC10CLR = (7 << 18), IPC10SET = (priority << 18))
#define mDMA5GetIntPriority()                   (IPC10bits.DMA5IP)
#define mDMA5SetIntSubPriority(subPriority)     (IPC10CLR = (3 << 16), IPC10SET = (subPriority << 16))
#define mDMA5GetIntSubPriority()                (IPC10bits.DMA5IS) 
/*********************************************************************
 * DMA 6
 ********************************************************************/
#define mDMA6ClearIntFlag()                     (IFS1CLR = (1 << 23))
#define mDMA6GetIntFlag()                       (IFS1bits.DMA6IF)
#define mDMA6GetIntEnable()                     (IEC1bits.DMA6IE)
#define mDMA6IntEnable(enable)                  (IEC1CLR = (1 << 23), IEC1SET = (enable << 23))
#define mDMA6SetIntPriority(priority)           (IPC10CLR = (7 << 26), IPC10SET = (priority << 26))
#define mDMA6GetIntPriority()                   (IPC10bits.DMA6IP)
#define mDMA6SetIntSubPriority(subPriority)     (IPC10CLR = (3 << 24), IPC10SET = (subPriority << 24))
#define mDMA6GetIntSubPriority()                (IPC10bits.DMA6IS) 
/*********************************************************************
 * DMA 7
 ********************************************************************/
#define mDMA7ClearIntFlag()                     (IFS1CLR = (1 << 24))
#define mDMA7GetIntFlag()                       (IFS1bits.DMA7IF)
#define mDMA7GetIntEnable()                     (IEC1bits.DMA7IE)
#define mDMA7IntEnable(enable)                  (IEC1CLR = (1 << 24), IEC1SET = (enable << 24))
#define mDMA7SetIntPriority(priority)           (IPC11CLR = (7 << 2), IPC11SET = (priority << 2))
#define mDMA7GetIntPriority()                   (IPC11bits.DMA7IP)
#define mDMA7SetIntSubPriority(subPriority)     (IPC11CLR = (3), IPC11SET = (subPriority))
#define mDMA7GetIntSubPriority()                (IPC11bits.DMA7IS) 
/*********************************************************************
 * USB Interrupt Control Functions
 *
 * Function:        void    mUSBClearIntFlag(void)
 *					int32_t     mUSBGetIntFlag(void)
 *					void    mUSBSetIntEnable(void)
 *					int32_t     mUSBGetIntEnable(void)
 *                  void    mUSBClearIntEnable(void)
 *					void    mUSBSetIntPriority(int32_t priority)
 *					int32_t     mUSBGetIntPriority(void)
 *					void    mUSBSetIntSubPriority(subPriority)
 *					int32_t     mUSBGetIntSubPriority(void)
 *
 * PreCondition:    EBASE and IntCtl.VS set up
 *
 * Inputs:			priority:  		A value between 0 - 7 inclusive.
 *					subPriority:	A value between 0 - 3 inclusive
 *
 * Output:          For 'Get' functions, the returned values are of
 *					same type as 'Set' functions.
 *
 * Side Effects:    None
 *
 * Overview:        None
 ********************************************************************/
#define mUSBClearIntFlag()                  (IFS1CLR = (1 << 25))
#define mUSBGetIntFlag()                    (IFS1bits.USBIF)
#define mUSBGetIntEnable()                  (IEC1bits.USBIE)
#define mUSBIntEnable(enable)               (IEC1CLR = (1 << 25), IEC1SET = (enable << 25))
#define mUSBSetIntPriority(priority)        (IPC11CLR = (7 << 10), IPC11SET = (priority << 10))
#define mUSBGetIntPriority()                (IPC11bits.USBIP)
#define mUSBSetIntSubPriority(subPriority)  (IPC11CLR = (3 << 8), IPC11SET = (subPriority << 8))
#define mUSBGetIntSubPriority()             (IPC11bits.USBIS) 
/*********************************************************************
 * Clear IRQ flag and enable bits funcitons
 *
 * Function:        void    mClearIFSRegister(reg_num)
 *					void    mClearIECRegister(reg_num)
 *
 * Inputs:			reg_num:  		A value between 0 - 3 inclusive.
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 ********************************************************************/
#define mClearIFSRegister(reg_num)  (IFS##reg_num##CLR = 0xFFFFFFFF)
#define mClearIECRegister(reg_num)  (IEC##reg_num##CLR = 0xFFFFFFFF)
#define mClearAllIFSRegister()      (   IFS0CLR = 0xFFFFFFFF,   IFS1CLR = 0xFFFFFFFF,   IFS2CLR = 0xFFFFFFFF)
#define mClearAllIECRegister()      (   IEC0CLR = 0xFFFFFFFF,   IEC1CLR = 0xFFFFFFFF,   IEC2CLR = 0xFFFFFFFF)
/*********************************************************************
 * Function:       void mINTSetIFSx(flag)
 *
 * PreCondition:    None
 *
 * Input:           flag - value you would like to set
 *
 * Output:          None
 *
 * Side Effects:    IFSx - bit(s) will be set
 ********************************************************************/
#define mINTSetIFS0(flag)                    ( IFS0SET = (uint32_t)(flag)  )
#define mINTSetIFS1(flag)                    ( IFS1SET = (uint32_t)(flag)  )
#define mINTSetIFS2(flag)                    ( IFS2SET = (uint32_t)(flag)  )
/*********************************************************************
 * Function:       void mINTClearIFSx(flag)
 *
 * PreCondition:    None
 *
 * Input:           flag - value you would like to cleared
 *
 * Output:          None
 *
 * Side Effects:    IFSx - bit(s) will be cleared
 ********************************************************************/
#define mINTClearIFS0(flag)                  ( IFS0CLR = (uint32_t)(flag)  )
#define mINTClearIFS1(flag)                  ( IFS1CLR = (uint32_t)(flag)  )
#define mINTClearIFS2(flag)                  ( IFS2CLR = (uint32_t)(flag)  )
/*********************************************************************
 * Function:       void mINTGetIFSx(flag)
 *
 * PreCondition:    None
 *
 * Input:           flag - value you would like to cleared
 *
 * Output:          None
 *
 * Side Effects:    Will tell is the flag is set
 ********************************************************************/
#define mINTGetIFS0(flag)               (IFS0 & (flag))
#define mINTGetIFS1(flag)               (IFS1 & (flag))
#define mINTGetIFS2(flag)               (IFS2 & (flag))
/*********************************************************************
 * Function:       void mINTSetIECx(flag)
 *
 * PreCondition:    None
 *
 * Input:           flag - value you would like to set
 *
 * Output:          None
 *
 * Side Effects:    IECx - bit(s) will be set
 ********************************************************************/
#define mINTSetIEC0(flag)                    ( IEC0SET = (uint32_t)(flag)  )
#define mINTSetIEC1(flag)                    ( IEC1SET = (uint32_t)(flag)  )
#define mINTSetIEC2(flag)                    ( IEC2SET = (uint32_t)(flag)  ) 
/*********************************************************************
 * Function:       void mINTClearIECx(flag)
 *
 * PreCondition:    None
 *
 * Input:           flag - value you would like to cleared
 *
 * Output:          None
 *
 * Side Effects:    IECx - bit(s) will be cleared
 ********************************************************************/
#define mINTClearIEC0(flag)                  ( IEC0CLR = (uint32_t)(flag)  )
#define mINTClearIEC1(flag)                  ( IEC1CLR = (uint32_t)(flag)  )
#define mINTClearIEC2(flag)                  ( IEC2CLR = (uint32_t)(flag)  )

/*********************************************************************
 * Function:       void INTSetIntProximityTimerReload(WORD time)
 *
 * PreCondition:    None
 *
 * Input:           time - 16 bit value that will be used to reload the IP
 *                          time with tiggered
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Set the time that will be loaded into the IP timer
 ********************************************************************/
#define mINTSetIntProximityTimerReload(time)    (INTCONbits.IPTMR = time)
/*********************************************************************
 * Function:        WORD INTGetIntProximityTimer(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          Returns the current IP timer value
 *
 * Side Effects:    None
 *
 * Overview:        This function will return the current value of the
 *                  IP timer.
 ********************************************************************/
#define mINTGetIntProximityTimer()      (INTCONbits.IPTMR)
/*********************************************************************
 * Function:        void INTSetFreeze(void)
 *                  void INTClearFreeze(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    When emulator in Debug Mode, module freezes operation
 *
 * Overview:        Sets/Clears FRZ bit in INTCON
 ********************************************************************/
#define mINTSetFreeze()                 (INTCONSET = INTCON_FRZ_BIT)
#define mINTClearFreeze()               (INTCONCLR = INTCON_FRZ_BIT)
/*********************************************************************
 * Function:        void INTSetTemporalProximityControl(level)
 *                  void INTDisableTemporalProximityControl(void)
 * PreCondition:    None
 *
 * Input:           level   - The interrupt level group priority or lower
 *                              start the IP timer and are disabled while
 *                              the IP is counting
 *
 * Output:          None
 *
 * Side Effects:    TPC bits are modified
 *
 * Overview:        TPC bits is the INTCON are set
 ********************************************************************/
#define INT_TPC_DISABLE                 0x00000700
#define INT_TPC_GROUP_PRI_LEVEL_1       1
#define INT_TPC_GROUP_PRI_LEVEL_2       2
#define INT_TPC_GROUP_PRI_LEVEL_3       3
#define INT_TPC_GROUP_PRI_LEVEL_4       4
#define INT_TPC_GROUP_PRI_LEVEL_5       5
#define INT_TPC_GROUP_PRI_LEVEL_6       6
#define INT_TPC_GROUP_PRI_LEVEL_7       7

#define mINTSetTemporalProximityControl(level)  (INTCONbits.TPC = level)
#define mINTDisableTemporalProximityControl()   (INTCONCLR = INT_TPC_DISABLE)
/*********************************************************************
 * Function:        uint32_t INTGetPendingInterrupt(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          The pending interrupt or invalid data
 *
 * Side Effects:    None
 *
 * Overview:        Gets the pending interrupt
 ********************************************************************/
/**********************************************************
 * enums
 **********************************************************/
typedef enum
{
    INT_IRQ_CT = 0,            // Core Timer Interrupt
    INT_IRQ_CS0,               // Core Software Interrupt 0
    INT_IRQ_CS1,               // Core Software Interrupt 1
    INT_IRQ_INT0,              // External Interrupt 0
    INT_IRQ_T1,                // Timer 1
    INT_IRQ_IC1,               // Input Capture 1
    INT_IRQ_OC1,               // Output Capture 1
    INT_IRQ_INT1,              // External Interrupt 1
    INT_IRQ_T2,                // Timer 2
    INT_IRQ_IC2,               // Input Capture 2
    INT_IRQ_OC2,               // Output Capture 2
    INT_IRQ_INT2,              // External Interrrupt 2
    INT_IRQ_T3,                // TImer 3
    INT_IRQ_IC3,               // Input Capture 3
    INT_IRQ_OC3,               // Output Capture 3
    INT_IRQ_INT3,              // External Interrupt 3
    INT_IRQ_T4,                // Timer 4
    INT_IRQ_IC4,               // Input Capture 4
    INT_IRQ_OC4,               // Output Capture 4
    INT_IRQ_INT4,              // External Interupt 4
    INT_IRQ_T5,                // Timer 5
    INT_IRQ_IC5,               // Input Capture 5
    INT_IRQ_OC5,               // Input Capture 5
    INT_IRQ_CN,                // Input Change
    INT_IRQ_SPI1E,             // SPI 1 Fault
    INT_IRQ_SPI1TX,            // SPI 1 Transfer Done
    INT_IRQ_SPI1RX,            // SPI 1 Receive Done
    INT_IRQ_U1E,               // UART 1 Error
    INT_IRQ_U1RX,              // UART 1 Receiver
    INT_IRQ_U1TX,              // UART 1 Transmitter
    INT_IRQ_I2C1B,             // I2C 1 Bus Colision Event
    INT_IRQ_I2C1S,             // I2C 1 Slave Event
    INT_IRQ_I2C1M,             // I2C 1 Master Event
    INT_IRQ_AD1,               // ADC 1 Convert Done
    INT_IRQ_PMP,               // Parallel Master Port
    INT_IRQ_CMP1,              // Comparator 1 Interrupt
    INT_IRQ_CMP2,              // Comparator 2 Interrupt
    INT_IRQ_SPI2E,             // SPI 2 Fault
    INT_IRQ_SPI2TX,            // SPI 2 Transfer Done
    INT_IRQ_SPI2RX,            // SPI 2 Receive Done
    INT_IRQ_U2E,               // UART 2 Error
    INT_IRQ_U2RX,              // UART 2 Receiver
    INT_IRQ_U2TX,              // UART 2 Transmitter
    INT_IRQ_I2C2B,             // I2C 1 Bus Colision Event
    INT_IRQ_I2C2S,             // I2C 2 Slave Event
    INT_IRQ_I2C2M,             // I2C 2 Master Event
    INT_IRQ_FSCM,              // Fail-safe Monitor
    INT_IRQ_FCE,               // Flash Control Event 
    INT_IRQ_RTCC,              // Real Time Clock
    INT_IRQ_DMA0,              // DMA Channel 0
    INT_IRQ_DMA1,              // DMA Channel 1
    INT_IRQ_DMA2,              // DMA Channel 2
    INT_IRQ_DMA3,              // DMA Channel 3
    INT_IRQ_DMA4,              // DMA Channel 4
    INT_IRQ_DMA5,              // DMA Channel 5
    INT_IRQ_DMA6,              // DMA Channel 6
    INT_IRQ_DMA7,              // DMA Channel 7
    INT_IRQ_USB,               // USB
    INT_NUM_IRQ
}INT_IRQ;

typedef enum
{
    INT_IV_CT = 0,             // Core Timer Interrupt
    INT_IV_CS0,                // Core Software Interrupt 0
    INT_IV_CS1,                // Core Software Interrupt 1
    INT_IV_INT0,               // External Interrupt 0
    INT_IV_T1,                 // Timer 1
    INT_IV_IC1,                // Input Capture 1
    INT_IV_OC1,                // Output Capture 1
    INT_IV_INT1,               // External Interrupt 1
    INT_IV_T2,                 // Timer 2
    INT_IV_IC2,                // Input Capture 2
    INT_IV_OC2,                // Output Capture 2
    INT_IV_INT2,               // External Interrrupt 2
    INT_IV_T3,                 // TImer 3
    INT_IV_IC3,                // Input Capture 3
    INT_IV_OC3,                // Output Capture 3
    INT_IV_INT3,               // External Interrupt 3
    INT_IV_T4,                 // Timer 4
    INT_IV_IC4,                // Input Capture 4
    INT_IV_OC4,                // Output Capture 4
    INT_IV_INT4,               // External Interupt 4
    INT_IV_T5,                 // Timer 5
    INT_IV_IC5,                // Input Capture 5
    INT_IV_OC5,                // Input Capture 5
    INT_IV_CN,                 // Input Change
    INT_IV_SPI1,               // SPI 1
    INT_IV_UART1,              // UART 1
    INT_IV_I2C1,               // I2C 1
    INT_IV_AD1,                // ADC 1 Convert Done
    INT_IV_PMP,                // Parallel Master Port
    INT_IV_CMP1,               // Comparator 1
    INT_IV_CMP2,               // Comparator 2
    INT_IV_SPI2,               // SPI 2
    INT_IV_UART2,              // UART 2
    INT_IV_I2C2,               // I2C 2
    INT_IV_FSCM,               // Fail Safe Clock Monitor
    INT_IV_FCE,                // Flash Control Event
    INT_IV_RTCC,               // Real Time Clock
    INT_IV_DMA0,               // DMA Channel 0
    INT_IV_DMA1,               // DMA Channel 1
    INT_IV_DMA2,               // DMA Channel 2
    INT_IV_DMA3,               // DMA Channel 3
    INT_IV_DMA4,               // DMA Channel 4
    INT_IV_DMA5,               // DMA Channel 5
    INT_IV_DMA6,               // DMA Channel 6
    INT_IV_DMA7,               // DMA Channel 7
    INT_IV_USB,                // USB
    INT_NUM_IV
}INT_VECTOR;

uint32_t INTGetPendingInterrupt(void);

#endif
