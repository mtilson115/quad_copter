/********************************************************************
 * FileName:		System.h
 * Dependencies:
 * Processor:		PIC32
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
 * $Id: CoreTimer.h,v 1.2 2006/11/03 15:07:21 C12923 Exp $
 * $Name:  $
 ********************************************************************/

#ifndef _SYSTEM_HEADER_FILE
#define _SYSTEM_HEADER_FILE

#include <p32xxxx.h>
#include <peripheral/int.h>
#include <peripheral/pcache.h>
#include <peripheral/osc.h>
#include <peripheral/dma.h>
#include <peripheral/bmx.h>
#include <peripheral/lock.h>

#define FLASH_ACCESS_TIME_HZ    20000000
#define PB_BUS_ACCESS_TIME_HZ   50000000

/*********************************************************************
 * Function:       void SYSTEMConfigPB(uint32_t sys_clock)
 *
 * PreCondition:    None
 *
 * Input:           sys_clock - system clock in Hz
 *
 * Output:          the PB divider value
 *
 * Side Effects:    Sets the PB divider to the optimum value
 ********************************************************************/
extern inline uint32_t __attribute__((always_inline)) SYSTEMConfigPB(uint32_t sys_clock)
{
    uint32_t dma_status;
    uint32_t int_status;
    uint32_t pb_clock;

    pb_clock = sys_clock;
    
    mSYSTEMUnlock(int_status, dma_status);

    // set the PB Div
    if(sys_clock > PB_BUS_ACCESS_TIME_HZ)
    {
        mOSCSetPBDIV(OSC_PB_DIV_2);
        pb_clock >>= 1;
    }
    else
    {
        mOSCSetPBDIV(OSC_PB_DIV_1);
    }

    mSYSTEMLock(int_status, dma_status);
    
    return pb_clock;
}



/*********************************************************************
 * Function:       void SYSTEMConfigWaitStatesAndPB(uint32_t sys_clock)
 *
 * PreCondition:    None
 *
 * Input:           sys_clock - system clock in Hz
 *
 * Output:          None
 *
 * Side Effects:    Sets the PB and Flash Wait states
 ********************************************************************/
extern inline uint32_t __attribute__((always_inline)) SYSTEMConfigWaitStatesAndPB(uint32_t sys_clock)
{
#ifdef _PCACHE
    uint32_t wait_states;
#endif
    uint32_t pb_clock;
    uint32_t int_status;
    
    pb_clock = SYSTEMConfigPB(sys_clock);
    

    // set the flash wait states based on 1 wait state
    // for every 25 MHz
#ifdef _PCACHE
    wait_states = 0;

    while(sys_clock >= FLASH_ACCESS_TIME_HZ)
    {
        wait_states++;
        sys_clock -= FLASH_ACCESS_TIME_HZ;
    }
    
    int_status=INTDisableInterrupts();	
    mCheConfigure(wait_states);
    INTRestoreInterrupts(int_status);

#endif
	
    return pb_clock;
}
/*********************************************************************
 * Function:       void SYSTEMConfigPerformance(uint32_t sys_clock, uint32_t flag)
 *
 * PreCondition:    None
 *
 * Input:           sys_clock - system clock in Hz
 *
 * Output:          None
 *
 * Side Effects:    Sets the PB and Flash Wait states
 ********************************************************************/
extern inline uint32_t __attribute__((always_inline)) SYSTEMConfigPerformance(uint32_t sys_clock)
{
    // set up the wait states
    uint32_t pb_clk;
#ifdef _PCACHE
    uint32_t cache_status;
#endif    
    uint32_t int_status;

    pb_clk = SYSTEMConfigWaitStatesAndPB(sys_clock);
    
    int_status=INTDisableInterrupts();
    
    mBMXDisableDRMWaitState();

#ifdef _PCACHE
    cache_status = mCheGetCon();
    cache_status |= CHE_CONF_PF_ALL;
    mCheConfigure(cache_status);
    CheKseg0CacheOn();
#endif

    INTRestoreInterrupts(int_status);
    
    return pb_clk;
    
}
#endif

