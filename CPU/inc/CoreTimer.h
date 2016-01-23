/********************************************************************
 * FileName:		CoreTimer.h
 * Dependencies:
 * Processor:		PICxx
 * Hardware:		N/A
 * Assembler:		N/A
 * Linker:		    N/A
 * Company:		    Microchip Technology, Inc.
 *
 * Software License Agreement:
 * The software supplied herewith by Microchip Technology Incorporated
 * (the �Company�) for its PICmicro� Microcontroller is intended and
 * supplied to you, the Company�s customer, for use solely and
 * exclusively on Microchip PICmicro Microcontroller products. The
 * software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN �AS IS� CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *********************************************************************
 * File Description:
 *
 * Core Timer Library
 *
 * Change History:
 * Name         Date            Changes
 * S Justice    11/02/06        Initial Version
 *
 * $Id: CoreTimer.h,v 1.1 2006/11/02 21:50:50 C12923 Exp $
 * $Name:  $

 ********************************************************************/

#ifndef _CORE_TIMER_LIB_HEADER
#define _CORE_TIMER_LIB_HEADER

#include "p32xxxx.h"
#include "INT.h"

/*********************************************************************
 * Core Timer Control Interrupt Functions
 *
 * Function:        void OpenCoreTimer(uint32_t period)
 *
 * PreCondition:    None
 *
 * Inputs:			period - 32 bit value for the timer
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 ********************************************************************/
extern inline void __attribute__((always_inline)) OpenCoreTimer(uint32_t period)
{
    // clear the count reg
    __asm__ volatile("mtc0   $0,$9");
    // set up the period in the compare reg
    __asm__ volatile("mtc0   %0,$11" : "+r"(period));
}
/*********************************************************************
 * Core Timer
 *
 * Function:        void UpdateCoreTimer(uint32_t period)
 *
 * PreCondition:    None
 *
 * Inputs:			period - 32 bit value for the timer
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 ********************************************************************/
extern inline void __attribute__((always_inline)) UpdateCoreTimer(uint32_t period)
{
    uint32_t old_period;

    // get the old compare time
    __asm__ volatile("mfc0   %0, $9" : "=r"(old_period));
    period += old_period;
    // set up the period in the compare reg
    __asm__ volatile("mtc0   %0,$11" : "+r"(period));
}
/******************************************************************************
 * Parameter values to be used with ConfigIntCoreTimer()
 *****************************************************************************/
#define CT_INT_ON		            	(1 << 15)
#define CT_INT_OFF						(0)

#define CT_INT_PRIOR_7                  (7)
#define CT_INT_PRIOR_6                  (6)
#define CT_INT_PRIOR_5                  (5)
#define CT_INT_PRIOR_4                  (4)
#define CT_INT_PRIOR_3                  (3)
#define CT_INT_PRIOR_2                  (2)
#define CT_INT_PRIOR_1                  (1)
#define CT_INT_PRIOR_0                  (0)

#define CT_INT_SUB_PRIOR_3              (3 << 4)
#define CT_INT_SUB_PRIOR_2              (2 << 4)
#define CT_INT_SUB_PRIOR_1              (1 << 4)
#define CT_INT_SUB_PRIOR_0              (0 << 4)
/******************************************************************************
 * Function:        void ConfigIntCoreTimer(int32_t config)
 *
 * Description:		Configures Core Timer  interrupt
 *
 * PreCondition:    None
 *
 * Inputs:			config: Bit-wise OR value of CT_INT_XXX, CT_INT_PRIOR_XXX
 *							  and CT_INT_SUB_PRIOR_X
 *
 *					Note: An absent symbol assumes corresponding bit(s)
 *					are disabled, or default value, and will be set = 0.
 *
 * Output:          None
 *
 * Example:			ConfigIntCoreTimer(CT_INT_ON | CT_INT_PRIOR_3 | CT_INT_SUB_PRIOR_1)
 *
 *****************************************************************************/
#define mConfigIntCoreTimer(config)      (mCTClearIntFlag(), mCTSetIntPriority(((config) & 7)),\
        mCTSetIntSubPriority((((config) >> 4) & 3)), mCTIntEnable(((config) >> 15)))
/******************************************************************************
 * Function:        EnableIntCoreTimer(void)
 *
 * Description:		Enables Core Timer interrupt
 *
 * PreCondition:    None
 *
 * Inputs:			None
 *
 * Output:          None
 *
 * Example:			EnableIntCoreTimer()
 *****************************************************************************/
#define mEnableIntCoreTimer()            mCTIntEnable(1)


/******************************************************************************
 * Function:        DisableIntCoreTimer(void)
 *
 * Description:		Disables Core Timer interrupt
 *
 * PreCondition:    None
 *
 * Inputs:			None
 *
 * Output:          None
 *
 * Example:			DisableIntCoreTimer()
 *****************************************************************************/
#define mDisableIntCoreTimer()           mCTIntEnable(0)

/******************************************************************************
 * Function:        void SetPriorityIntCoreTimer(int32_t priority)
 *
 * Description:		Sets the priority for Core Timer interrupt
 *					The ConfigIntCoreTimer() sets the priority. Use this
 *					macro to change the previously set priority
 *					without any need to respecify other parameters.
 *
 * PreCondition:    None
 *
 * Inputs:			priority : A value of CT_INT_PRIOR_XXX
 *
 * Output:          None
 *
 *
 * Example:			SetPriorityIntCoreTimer(CT_INT_PRIORITY_3)
 *****************************************************************************/
#define mSetPriorityIntCoreTimer(priority)	mCTSetIntPriority(priority)

/******************************************************************************
 * Function:        uint32_t ReadCoreTimer(void)
 *
 * Description:		Returns the current Core Timer value.
 *
 * PreCondition:    None
 *
 * Inputs:			None
 *
 * Output:          Count value.
 *
 * Example:			ReadCoreTimer()
 *
 *****************************************************************************/
extern inline uint32_t __attribute__((always_inline)) ReadCoreTimer(void)
{
    uint32_t timer;

    // get the count reg
    __asm__ volatile("mfc0   %0, $9" : "=r"(timer));

    return timer;
}


/******************************************************************************
 * Function:        void WriteTimer1(uint32_t value)
 *
 * Description:		Loads given value into Timer1 register.
 *
 * PreCondition:    None
 *
 * Inputs:			value: The value to be loaded.
 *
 * Output:          None
 *
 * Example:			WriteTimer1(1234)
 *
 *****************************************************************************/
extern inline void __attribute__((always_inline)) WriteCoreTimer(uint32_t timer)
{
    // get the count reg
    __asm__ volatile("mtc0   %0, $9": "+r"(timer));

}

#endif
