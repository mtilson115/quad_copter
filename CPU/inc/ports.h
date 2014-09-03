/******************************************************************************
 *
 *                  IO PORT Library definitions
 *
 ******************************************************************************
 * FileName:        ports.h
 * Dependencies:
 * Processor:       PIC32 family
 *
 * Complier:        MPLAB Cxx
 *                  MPLAB IDE xx
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the “Company”) for its PIC32 Microcontroller is intended
 * and supplied to you, the Company’s customer, for use solely and
 * exclusively on Microchip PIC32 Microcontroller products.
 * The software is owned by the Company and/or its supplier, and is
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
 *
 * $Id: ports.h,v 1.8 2006/10/10 16:51:48 C11842 Exp $
 *
 * $Name:  $
 *
 *****************************************************************************/

#ifndef _PORTS_H_
#define _PORTS_H_

#if defined(DAYTONA)
	#include <p32xxxx.h>
	#include "Int.h"
#endif


/******************************************************************************
 * Parameter values to be used with ConfigINTx() & SetPriorityINTx()
 *****************************************************************************/
// Interrupt enable/disable values
#define EXT_INT_ENABLE			(1 << 15)
#define EXT_INT_DISABLE			(0)

// Interrupt edge modes - must be any one value only
#define RISING_EDGE_INT			(1 << 3)
#define FALLING_EDGE_INT		(0)

// Priority values - must be any one value only
#define EXT_INT_PRI_7         (7)
#define EXT_INT_PRI_6         (6)
#define EXT_INT_PRI_5         (5)
#define EXT_INT_PRI_4         (4)
#define EXT_INT_PRI_3         (3)
#define EXT_INT_PRI_2         (2)
#define EXT_INT_PRI_1         (1)
#define EXT_INT_PRI_0         (0)

/******************************************************************************
 * External INTx Control Function
 *
 * Function:        void ConfigINTx(int32_t config)
 *
 * Description:		Configures an external interrupt
 *
 * PreCondition:    None
 *
 * Inputs:			config: Bit-wise ORed of Priority, Interrupt Edge
 *							Mode and Interrupt enable/disable value.
 *
 *					Note: An absent symbol assumes corresponding bit(s)
 *					are disabled, or default value, and will be set = 0.
 *
 * Output:          None
 *
 * Example:			ConfigINT0(EXT_INT_PRI_1 | RISING_EDGE_INT | EXT_INT_ENABLE)
 *
 *****************************************************************************/
#define ConfigINT0(config)  (mINT0ClearIntFlag(), mINT0SetIntPriority(config & 7),\
        mINT0SetEdgeMode((config >> 3) & 1), mINT0IntEnable(config >> 15))

#define ConfigINT1(config)  (mINT1ClearIntFlag(), mINT1SetIntPriority(config & 7),\
	    mINT1SetEdgeMode((config >> 3) & 1), mINT1IntEnable(config >> 15))

#define ConfigINT2(config)  (mINT2ClearIntFlag(), mINT2SetIntPriority(config & 7),\
	    mINT2SetEdgeMode((config >> 3) & 1), mINT2IntEnable(config >> 15))

#define ConfigINT3(config)  (mINT3ClearIntFlag(), mINT3SetIntPriority(config & 7),\
	    mINT3SetEdgeMode((config >> 3) & 1), mINT3IntEnable(config >> 15))

#define ConfigINT4(config)  (mINT4ClearIntFlag(), mINT4SetIntPriority(config & 7),\
	    mINT4SetEdgeMode((config >> 3) & 1), mINT4IntEnable(config >> 15))


/******************************************************************************
 * External INTx Control Function
 *
 * Function:        void SetPriorityIntx(int32_t priority)
 *
 * Description:		Sets the priority for an External interrupt
 *
 * PreCondition:    None
 *
 * Inputs:			A Priority value
 *
 * Output:          None
 *
 * Example:			SetPriorityInt0(EXT_INT_PRI_5)
 *
 *****************************************************************************/
#define SetPriorityInt0(priority)	mINT0SetIntPriority(priority)
#define SetPriorityInt1(priority)	mINT1SetIntPriority(priority)
#define SetPriorityInt2(priority)	mINT2SetIntPriority(priority)
#define SetPriorityInt3(priority)	mINT3SetIntPriority(priority)
#define SetPriorityInt4(priority)	mINT4SetIntPriority(priority)


/******************************************************************************
 * Sub-priority alues to be used with SetSubPriorityIntx
 *****************************************************************************/
#define EXT_INT_SUB_PRI_3			(3)
#define EXT_INT_SUB_PRI_2			(2)
#define EXT_INT_SUB_PRI_1			(1)
#define EXT_INT_SUB_PRI_0			(0)

/******************************************************************************
 * External INTx Control Function
 *
 * Function:        void SetSubPriorityIntx(int32_t subPriority)
 *
 * Description:		Sets the sub-priority for an External interrupt
 *
 * PreCondition:    None
 *
 * Inputs:			A Sub-priority value
 *
 * Output:          None
 *
 * Example:			SetSubPriorityInt0(EXT_INT_SUB_PRI_2)
 *
 *****************************************************************************/
#define SetSubPriorityInt0(subpriority) mINT0SetIntSubPriority(subpriority)
#define SetSubPriorityInt1(subpriority) mINT1SetIntSubPriority(subpriority)
#define SetSubPriorityInt2(subpriority) mINT2SetIntSubPriority(subpriority)
#define SetSubPriorityInt3(subpriority) mINT3SetIntSubPriority(subpriority)
#define SetSubPriorityInt4(subpriority) mINT4SetIntSubPriority(subpriority)


/******************************************************************************
 * External INTx Control Function
 *
 * Function:        void CloseINTx(void)
 *
 * Description:		Disables an external interrupt and clears corresponding flag.
 *
 * PreCondition:    None
 *
 * Inputs:			None
 *
 * Output:          None
 *
 * Example:			CloseINT0()
 *
 *****************************************************************************/
#define CloseINT0()                 (mINT0IntEnable(0), mINT0ClearIntFlag())
#define CloseINT1()					(mINT1IntEnable(0), mINT1ClearIntFlag())
#define CloseINT2()					(mINT2IntEnable(0), mINT2ClearIntFlag())
#define CloseINT3()					(mINT3IntEnable(0), mINT3ClearIntFlag())
#define CloseINT4()					(mINT4IntEnable(0), mINT4ClearIntFlag())

/******************************************************************************
 * External INTx Control Function
 *
 * Function:        void EnableINTx(void)
 *
 * Description:		Enables an external interrupt.
 *
 * PreCondition:    None
 *
 * Inputs:			None
 *
 * Output:          None
 *
 * Example:			EnableINT4()
 *
 *****************************************************************************/
#define EnableINT0              mINT0IntEnable(1)
#define EnableINT1              mINT1IntEnable(1)
#define EnableINT2              mINT2IntEnable(1)
#define EnableINT3              mINT3IntEnable(1)
#define EnableINT4              mINT4IntEnable(1)

/******************************************************************************
 * External INTx Control Function
 *
 * Function:        void DisableINTx(void)
 *
 * Description:		Disables an external interrupt.
 *
 * PreCondition:    None
 *
 * Inputs:			None
 *
 * Output:          None
 *
 * Example:			DisableINT0()
 *
 *****************************************************************************/
#define DisableINT0             mINT0IntEnable(0)
#define DisableINT1             mINT0IntEnable(0)
#define DisableINT2             mINT0IntEnable(0)
#define DisableINT3             mINT0IntEnable(0)
#define DisableINT4             mINT0IntEnable(0)


/******************************************************************************
 * Parameter values to be used with ConfigINTCN()
 *****************************************************************************/
// Change Interrupt ON/OFF values.
#define CHANGE_INT_ON           	(1 << 15)
#define CHANGE_INT_OFF          	(0)

// Change Interrupt Priority Values
#define CHANGE_INT_PRI_7        	(7)
#define CHANGE_INT_PRI_6        	(6)
#define CHANGE_INT_PRI_5        	(5)
#define CHANGE_INT_PRI_4        	(4)
#define CHANGE_INT_PRI_3        	(3)
#define CHANGE_INT_PRI_2        	(2)
#define CHANGE_INT_PRI_1        	(1)
#define CHANGE_INT_PRI_0        	(0)


/******************************************************************************
 * Change Notice Interrupt Control Function
 *
 * Function:        void ConfigIntCN(int32_t config)
 *
 * Description:		Configures Change Notice interrupts
 *
 * PreCondition:    None
 *
 * Inputs:			Bit-wise OR value of CHANGE_INT_PRIx and
 *					CHANGE_INT_XXX sets
 *
 * Output:          None
 *
 * Example:			ConfigIntCN(CHANGE_INT_PRI_3 |
 *								  CHANGE_INT_ON)
 *
 *****************************************************************************/
#define ConfigIntCN(config) (mCNClearIntFlag(), mCNSetIntPriority((config & 0x7)),\
	mCNIntEnable(config >> 15))


/******************************************************************************
 * Parameter values to be used with ConfigCNPullups()
 *****************************************************************************/
#define CN21_PULLUP_ENABLE			(1 << 21)
#define CN20_PULLUP_ENABLE			(1 << 20)
#define CN19_PULLUP_ENABLE			(1 << 19)
#define CN18_PULLUP_ENABLE			(1 << 18)
#define CN17_PULLUP_ENABLE			(1 << 17)
#define CN16_PULLUP_ENABLE			(1 << 16)
#define CN15_PULLUP_ENABLE			(1 << 15)
#define CN14_PULLUP_ENABLE			(1 << 14)
#define CN13_PULLUP_ENABLE			(1 << 13)
#define CN12_PULLUP_ENABLE			(1 << 12)
#define CN11_PULLUP_ENABLE			(1 << 11)
#define CN10_PULLUP_ENABLE			(1 << 10)
#define CN9_PULLUP_ENABLE			(1 << 9)
#define CN8_PULLUP_ENABLE			(1 << 8)
#define CN7_PULLUP_ENABLE			(1 << 7)
#define CN6_PULLUP_ENABLE			(1 << 6)
#define CN5_PULLUP_ENABLE			(1 << 5)
#define CN4_PULLUP_ENABLE			(1 << 4)
#define CN3_PULLUP_ENABLE			(1 << 3)
#define CN2_PULLUP_ENABLE			(1 << 2)
#define CN1_PULLUP_ENABLE			(1 << 1)
#define CN0_PULLUP_ENABLE			(1 << 0)

/******************************************************************************
 * Change Notice Interrupt Control Function
 *
 * Function:        void ConfigCNPullups(int32_t config)
 *
 * Description:		Configures Change Notice pull-ups
 *
 * PreCondition:    None
 *
 * Inputs:			Bit-wise OR value of CNx_PULLUP_ENABLE set
 *
 *					Note: An absent symbol assumes corresponding bit(s)
 *					are disabled, or default value, and will be set = 0.
 *
 * Output:          None
 *
 * Example:			To enable CN0 & CN1 pullups and disable all others
 *					ConfigCNPullups(CN0_PULLUP_ENABLE | CN1_PULLUP_ENABLE)
 *
 *****************************************************************************/
#define ConfigCNPullups(config) 	CNPUE = config


/******************************************************************************
 * Change Notice Interrupt Control Function
 *
 * Function:        void EnableCNx(void)
 *
 * Description:		Enables individual Change Notice interrupt
 *
 * PreCondition:    None
 *
 * Inputs:			None
 *
 * Output:          None
 *
 * Example:			EnableCN0()
 *
 *****************************************************************************/
#define EnableCN21()             	CNENSET = (1 << 21)
#define EnableCN20()              	CNENSET = (1 << 20)
#define EnableCN19()              	CNENSET = (1 << 19)
#define EnableCN18()              	CNENSET = (1 << 18)
#define EnableCN17()              	CNENSET = (1 << 17)
#define EnableCN16()              	CNENSET = (1 << 16)
#define EnableCN15()              	CNENSET = (1 << 15)
#define EnableCN14()              	CNENSET = (1 << 14)
#define EnableCN13()              	CNENSET = (1 << 13)
#define EnableCN12()              	CNENSET = (1 << 12)
#define EnableCN11()              	CNENSET = (1 << 11)
#define EnableCN10()              	CNENSET = (1 << 10)
#define EnableCN9()               	CNENSET = (1 << 9)
#define EnableCN8()               	CNENSET = (1 << 8)
#define EnableCN7()               	CNENSET = (1 << 7)
#define EnableCN6()               	CNENSET = (1 << 6)
#define EnableCN5()               	CNENSET = (1 << 5)
#define EnableCN4()               	CNENSET = (1 << 4)
#define EnableCN3()               	CNENSET = (1 << 3)
#define EnableCN2()               	CNENSET = (1 << 2)
#define EnableCN1()               	CNENSET = (1 << 1)
#define EnableCN0()               	CNENSET = (1 << 0)

/******************************************************************************
 * Change Notice Interrupt Control Function
 *
 * Function:        void DisableCNx(void)
 *
 * Description:		Disables individual Change Notice interrupt
 *
 * PreCondition:    None
 *
 * Inputs:			None
 *
 * Output:          None
 *
 * Example:			DisableCN0()
 *
 *****************************************************************************/
#define DisableCN21()              	CNENCLR = (1 << 21)
#define DisableCN20()              	CNENCLR = (1 << 20)
#define DisableCN19()              	CNENCLR = (1 << 19)
#define DisableCN18()              	CNENCLR = (1 << 18)
#define DisableCN17()              	CNENCLR = (1 << 17)
#define DisableCN16()             	CNENCLR = (1 << 16)
#define DisableCN15()              	CNENCLR = (1 << 15)
#define DisableCN14()              	CNENCLR = (1 << 14)
#define DisableCN13()              	CNENCLR = (1 << 13)
#define DisableCN12()              	CNENCLR = (1 << 12)
#define DisableCN11()              	CNENCLR = (1 << 11)
#define DisableCN10()              	CNENCLR = (1 << 10)
#define DisableCN9()               	CNENCLR = (1 << 9)
#define DisableCN8()               	CNENCLR = (1 << 8)
#define DisableCN7()               	CNENCLR = (1 << 7)
#define DisableCN6()               	CNENCLR = (1 << 6)
#define DisableCN5()               	CNENCLR = (1 << 5)
#define DisableCN4()               	CNENCLR = (1 << 4)
#define DisableCN3()               	CNENCLR = (1 << 3)
#define DisableCN2()               	CNENCLR = (1 << 2)
#define DisableCN1()               	CNENCLR = (1 << 1)
#define DisableCN0()               	CNENCLR = (1 << 0)

/******************************************************************************
 * Parameter values to be used with mPORTxxx macros
 *****************************************************************************/
#define IOPORT_BIT_31                       (1 << 31)
#define IOPORT_BIT_30                       (1 << 30)
#define IOPORT_BIT_29                       (1 << 29)
#define IOPORT_BIT_28                       (1 << 28)
#define IOPORT_BIT_27                       (1 << 27)
#define IOPORT_BIT_26                       (1 << 26)
#define IOPORT_BIT_25                       (1 << 25)
#define IOPORT_BIT_24                       (1 << 24)
#define IOPORT_BIT_23                       (1 << 23)
#define IOPORT_BIT_22                       (1 << 22)
#define IOPORT_BIT_21                       (1 << 21)
#define IOPORT_BIT_20                       (1 << 20)
#define IOPORT_BIT_19                       (1 << 19)
#define IOPORT_BIT_18                       (1 << 18)
#define IOPORT_BIT_17                       (1 << 17)
#define IOPORT_BIT_16                       (1 << 16)
#define IOPORT_BIT_15                       (1 << 15)
#define IOPORT_BIT_14                       (1 << 14)
#define IOPORT_BIT_13                       (1 << 13)
#define IOPORT_BIT_12                       (1 << 12)
#define IOPORT_BIT_11                       (1 << 11)
#define IOPORT_BIT_10                       (1 << 10)
#define IOPORT_BIT_9                        (1 << 9)
#define IOPORT_BIT_8                        (1 << 8)
#define IOPORT_BIT_7                        (1 << 7)
#define IOPORT_BIT_6                        (1 << 6)
#define IOPORT_BIT_5                        (1 << 5)
#define IOPORT_BIT_4                        (1 << 4)
#define IOPORT_BIT_3                        (1 << 3)
#define IOPORT_BIT_2                        (1 << 2)
#define IOPORT_BIT_1                        (1 << 1)
#define IOPORT_BIT_0                        (1 << 0)

/******************************************************************************
 * GPIO Control Macros
 *
 * Function:
 *			To set I/O pin directions for a PORTx
 *					void mPORTxConfig(int32_t _direction)
 *					int32_t mPORTxGetConfig(void)
 *					int32_t mPORTxReadConfigBit(int32_t _bits)
 *
 *			To read an input pin
 *					int32_t mPORTxRead(void)
 *					int32_t mPORTxReadBit(int32_t _bits)
 *
 *			To write to an output pin
 *					void mPORTxWrite(int32_t _value)
 *					int32_t mPORTxReadLatch(void)
 *					int32_t mPORTxReadLatchBit(int32_t _bits)
 *
 *			To configure select pins in a PORT without affecting other pins
 *                  void mPORTxSetPinsDigitalInput(int32_t _inputs)
 *                  void mPORTxSetPinsDigitalOutput(int32_t _outputs) 
 *                  void mPORTxSetPinsAnalogInput(int32_t _inputs)
 *                  void mPORTxSetPinsAnalgoOutput(int32_t _outputs)
 *					void mPORTxOutputConfig(int32_t _outputs)
 *					void mPORTxInputConfig(int32_t _inputs)
 *
 *			To manipulate select pins in a PORT without affecting other pins
 *					void mPORTxSetbits(int32_t _bits)
 *					void mPORTxClearBits(int32_t _bits)
 *					void mPORTxToggleBits(int32_t _bits)
 *
 *			To make a PORT all input and clear latch values.
 *					void mPORTxCloseAll(void)
 *					void mPORTxCloseBits(int32_t _bits)
 *
 *			To configure open drain I/Os in a PORT.
 *					void mPORTxOpenDrainOpen(int32_t _bits)
 *					void mPORTxOpenDrainClose(int32_t _bits)
 *
 * Description:		None
 *
 * PreCondition:    None
 *
 * Inputs:			Depends
 *
 * Output:          None
 *
 * Example:			mPORTCSetPinsDigitalOutput(IO_PORT_BIT_4 | IO_PORT_BIT_3)
 *
 *****************************************************************************/

#if defined _PORTA
/******************************************************************************
 * PORTA macros
 *****************************************************************************/
#define mPORTAConfig(_tris)                 TRISA = (uint32_t)_tris
#define mPORTARead()                        PORTA
#define mPORTAWrite(_lat)                   LATA = (uint32_t)_lat
#define mPORTAReadLatch()                   LATA
#define mPORTAGetConfig()                   TRISA
#define mPORTAReadBit(_bits)                (PORTA & (uint32_t)_bits)
#define mPORTAReadLatchBit(_bits)           (LATA & (uint32_t)_bits)
#define mPORTAReadConfigBit(_bits)          (TRISA & (uint32_t)_bits)

#define mPORTASetPinsDigitalIn(_inputs)     TRISASET = (uint32_t)_inputs
#define mPORTASetPinsDigitalOut(_outputs)   TRISACLR = (uint32_t)_outputs
#define mPORTAOutputConfig(_outputs)         TRISACLR = (uint32_t)_outputs
#define mPORTAInputConfig(_inputs)           TRISASET = (uint32_t)_inputs
#define mPORTASetBits(_bits)                LATASET = (uint32_t)_bits
#define mPORTAClearBits(_bits)              LATACLR = (uint32_t)_bits
#define mPORTAToggleBits(_bits)             LATAINV = (uint32_t)_bits
#define mPORTACloseAll()                    (TRISASET = 0xFFFFFFFF,\
                                             LATACLR = 0xFFFFFFFF)
#define mPORTACloseBits(_bits)              (TRISASET = (uint32_t)_bits,\
                                             LATACLR = (uint32_t)_bits)
#define mPORTAOpenDrainOpen(_bits)          (ODCFGASET = (uint32_t)_bits,\
                                             TRISACLR = (uint32_t)_bits)
#define mPORTAOpenDrainClose(_bits)         (ODCFGACLR = (uint32_t)_bits,\
                                             TRISASET = (uint32_t)_bits)
#endif  //end PORTA


#if defined _PORTB
/******************************************************************************
 * PORTB macros
 *****************************************************************************/
#define mPORTBConfig(_tris)                 TRISB = (uint32_t)_tris
#define mPORTBRead()                        PORTB
#define mPORTBWrite(_lat)                   LATB = (uint32_t)_lat
#define mPORTBReadLatch()                   LATB
#define mPORTBGetConfig()                   TRISB
#define mPORTBReadBit(_bits)                (PORTB & (uint32_t)_bits)
#define mPORTBReadLatchBit(_bits)           (LATB & (uint32_t)_bits)
#define mPORTBReadConfigBit(_bits)          (TRISB & (uint32_t)_bits)

#define mPORTBSetPinsAnalogOut(_outputs)    (TRISBSET = (uint32_t)_outputs,\
                                            AD1PCFGCLR = (uint32_t)_outputs)
#define mPORTBSetPinsAnalogIn(_inputs)      (TRISBSET = (uint32_t)_inputs,\
                                            AD1PCFGCLR = (uint32_t)_inputs)
#define mPORTBSetPinsDigitalOut(_bits)      (TRISBCLR = (uint32_t)_inputs,\
                                            AD1PCFGSET = (uint32_t)_outputs)
#define mPORTBSetPinsDigitalIn(_bits)       (TRISBSET = (uint32_t)_bits,\
                                            AD1PCFGSET = (uint32_t)_bits)
#define mPORTBOutputConfig(_outputs)        (AD1PCFGSET = (uint32_t) _outputs,\
                                            TRISBCLR = (uint32_t)_outputs)
#define mPORTBInputConfig(_inputs)          (AD1PCFGSET = (uint32_t) _inputs,\
                                            TRISBSET = (uint32_t)_inputs)
#define mPORTBSetBits(_bits)                LATBSET = (uint32_t)_bits
#define mPORTBClearBits(_bits)              LATBCLR = (uint32_t)_bits
#define mPORTBToggleBits(_bits)             LATBINV = (uint32_t)_bits
#define mPORTBCloseAll()                    (TRISBSET = 0xFFFFFFFF,\
                                             LATBCLR = 0xFFFFFFFF)
#define mPORTBCloseBits(_bits)              (TRISBSET = (uint32_t)_bits,\
                                             LATBCLR = (uint32_t)_bits)
#define mPORTBOpenDrainOpen(_bits)          (ODCFGBSET = (uint32_t)_bits,\
                                             TRISBCLR = (uint32_t)_bits)
#define mPORTBOpenDrainClose(_bits)         (ODCFGBCLR = (uint32_t)_bits,\
                                             TRISBSET = (uint32_t)_bits)
#endif  //end PORTB

#if defined _PORTC
/******************************************************************************
 * PORTC macros
 *****************************************************************************/
#define mPORTCConfig(_tris)                 TRISC = (uint32_t)_tris
#define mPORTCRead()                        PORTC
#define mPORTCWrite(_lat)                   LATC = (uint32_t)_lat
#define mPORTCReadLatch()                   LATC
#define mPORTCGetConfig()                   TRISC
#define mPORTCReadBit(_bits)                (PORTC & (uint32_t)_bits)
#define mPORTCReadLatchBit(_bits)           (LATC & (uint32_t)_bits)
#define mPORTCReadConfigBit(_bits)          (TRISC & (uint32_t)_bits)

#define mPORTCSetPinsDigitalIn(_inputs)     TRISCSET = (uint32_t)_inputs
#define mPORTCSetPinsDigitalOut(_outputs)   TRISCCLR = (uint32_t)_outpust
#define mPORTCOutputConfig(_outputs)        TRISCCLR = (uint32_t)_outputs
#define mPORTCInputConfig(_inputs)          TRISCSET = (uint32_t)_inputs
#define mPORTCSetBits(_bits)                LATCSET = (uint32_t)_bits
#define mPORTCClearBits(_bits)              LATCCLR = (uint32_t)_bits
#define mPORTCToggleBits(_bits)             LATCINV = (uint32_t)_bits
#define mPORTCCloseAll()                    (TRISCSET = 0xFFFFFFFF,\
                                             LATCCLR = 0xFFFFFFFF)
#define mPORTCCloseBits(_bits)              (TRISCSET = (uint32_t)_bits,\
                                             LATCCLR = (uint32_t)_bits)
#define mPORTCOpenDrainOpen(_bits)          (ODCFGCSET = (uint32_t)_bits,\
                                             TRISCCLR = (uint32_t)_bits)
#define mPORTCOpenDrainClose(_bits)         (ODCFGCCLR = (uint32_t)_bits,\
                                             TRISCSET = (uint32_t)_bits)
#endif  // end PORTC

#if defined _PORTD
/******************************************************************************
 * PORTD macros
 *****************************************************************************/
#define mPORTDConfig(_tris)                 TRISD = (uint32_t)_tris
#define mPORTDRead()                        PORTD
#define mPORTDWrite(_lat)                   LATD = (uint32_t)_lat
#define mPORTDReadLatch()                   LATD
#define mPORTDGetConfig()                   TRISD
#define mPORTDReadBit(_bits)                (PORTD & (uint32_t)_bits)
#define mPORTDReadLatchBit(_bits)           (LATD & (uint32_t)_bits)
#define mPORTDReadConfigBit(_bits)          (TRISD & (uint32_t)_bits)

#define mPORTDSetPinsDigitalIn(_inputs)     TRISDSET = (uint32_t)_inputs
#define mPORTDSetPinsDigitalOut(_outputs)   TRISDCLR = (uint32_t)_outputs
#define mPORTDOutputConfig(_outputs)        TRISDCLR = (uint32_t)_outputs
#define mPORTDInputConfig(_inputs)          TRISDSET = (uint32_t)_inputs
#define mPORTDSetBits(_bits)                LATDSET = (uint32_t)_bits
#define mPORTDClearBits(_bits)              LATDCLR = (uint32_t)_bits
#define mPORTDToggleBits(_bits)             LATDINV = (uint32_t)_bits
#define mPORTDCloseAll()                    (TRISDSET = 0xFFFFFFFF,\
                                             LATDCLR = 0xFFFFFFFF)
#define mPORTDCloseBits(_bits)              (TRISDSET = (uint32_t)_bits,\
                                             LATDCLR = (uint32_t)_bits)
#define mPORTDOpenDrainOpen(_bits)          (TRISDCLR = (uint32_t)_bits,\
                                             ODCFGDSET = (uint32_t)_bits)
#define mPORTDOpenDrainClose(_bits)         (TRISDSET = (uint32_t)_bits,\
                                             ODCFGDCLR = (uint32_t)_bits)
#endif  // end PORTD

#if defined _PORTE
/******************************************************************************
 * PORTE macros
 *****************************************************************************/
#define mPORTEConfig(_tris)                 TRISE = (uint32_t)_tris
#define mPORTERead()                        PORTE
#define mPORTEWrite(_lat)                   LATE = (uint32_t)_lat
#define mPORTEReadLatch()                   LATE
#define mPORTEGetConfig()                   TRISE
#define mPORTEReadBit(_bits)                (PORTE & (uint32_t)_bits)
#define mPORTEReadLatchBit(_bits)           (LATE & (uint32_t)_bits)
#define mPORTEReadConfigBit(_bits)          (TRISE & (uint32_t)_bits)

#define mPORTESetPinsDigitalIn(_inputs)     TRISESET = (uint32_t)_inputs
#define mPORTESetPinsDigitalOut(_outputs)   TRISECLR = (uint32_t)_outputs
#define mPORTEOutputConfig(_outputs)        TRISECLR = (uint32_t)_outputs
#define mPORTEInputConfig(_inputs)          TRISESET = (uint32_t)_inputs
#define mPORTESetBits(_bits)                LATESET = (uint32_t)_bits
#define mPORTEClearBits(_bits)              LATECLR = (uint32_t)_bits
#define mPORTEToggleBits(_bits)             LATEINV = (uint32_t)_bits
#define mPORTECloseAll()                    (TRISESET = 0xFFFFFFFF,\
                                             LATECLR = 0xFFFFFFFF)
#define mPORTECloseBits(_bits)              (TRISESET = (uint32_t)_bits,\
                                             LATECLR = (uint32_t)_bits)
#define mPORTEOpenDrainOpen(_bits)          (ODCFGESET = (uint32_t)_bits,\
                                             TRISECLR = (uint32_t)_bits)
#define mPORTEOpenDrainClose(_bits)         (ODCFGECLR = (uint32_t)_bits,\
                                             TRISESET = (uint32_t)_bits)
#endif  // end PORTE

#if defined _PORTF
/******************************************************************************
 * PORTF macros
 *****************************************************************************/
#define mPORTFConfig(_tris)                 TRISF = (uint32_t)_tris
#define mPORTFRead()                        PORTF
#define mPORTFWrite(_lat)                   LATF = (uint32_t)_lat
#define mPORTFReadLatch()                   LATF
#define mPORTFGetConfig()                   TRISF
#define mPORTFReadBit(_bits)                (PORTF & (uint32_t)_bits)
#define mPORTFReadLatchBit(_bits)           (LATF & (uint32_t)_bits)
#define mPORTFReadConfigBit(_bits)          (TRISF & (uint32_t)_bits)

#define mPORTFSetPinsDigitalIn(_inputs)     TRISFSET = (uint32_t)_inputs
#define mPORTFSetPinsDigitalOut(_outputs)   TRISFCLR = (uint32_t)_outputs
#define mPORTFOutputConfig(_outputs)        TRISFCLR = (uint32_t)_outputs
#define mPORTFInputConfig(_inputs)          TRISFSET = (uint32_t)_inputs
#define mPORTFSetBits(_bits)                LATFSET = (uint32_t)_bits
#define mPORTFClearBits(_bits)              LATFCLR = (uint32_t)_bits
#define mPORTFToggleBits(_bits)             LATFINV = (uint32_t)_bits
#define mPORTFCloseAll()                    (TRISFSET = 0xFFFFFFFF,\
                                             LATFCLR = 0xFFFFFFFF)
#define mPORTFCloseBits(_bits)              (TRISFSET = (uint32_t)_bits,\
                                             LATFCLR = (uint32_t)_bits)
#define mPORTFOpenDrainOpen(_bits)          (ODCFGFSET = (uint32_t)_bits,\
                                             TRISFCLR = (uint32_t)_bits)
#define mPORTFOpenDrainClose(_bits)         (ODCFGFCLR = (uint32_t)_bits,\
                                             TRISFSET = (uint32_t)_bits)
#endif  // end PORTF

#if defined _PORTG
/******************************************************************************
 * PORTG macros
 *****************************************************************************/
#define mPORTGConfig(_tris)                 TRISG = (uint32_t)_tris
#define mPORTGSetPinsDigitalIn(_bits)       TRISGSET = (uint32_t)_bits
#define mPORTGSetPinsDigitalOut(_bits)      TRISGCLR = (uint32_t)_bits
#define mPORTGRead()                        PORTG
#define mPORTGWrite(_lat)                   LATG = (uint32_t)_lat
#define mPORTGReadLatch()                   LATG
#define mPORTGGetConfig()                   TRISG
#define mPORTGReadBit(_bits)                (PORTG & (uint32_t)_bits)
#define mPORTGReadLatchBit(_bits)           (LATG & (uint32_t)_bits)
#define mPORTGReadConfigBit(_bits)          (TRISG & (uint32_t)_bits)

#define mPORTGOutputConfig(_outputs)        TRISGCLR = (uint32_t)_outputs
#define mPORTGInputConfig(_inputs)          TRISGSET = (uint32_t)_inputs
#define mPORTGSetBits(_bits)                LATGSET = (uint32_t)_bits
#define mPORTGClearBits(_bits)              LATGCLR = (uint32_t)_bits
#define mPORTGToggleBits(_bits)             LATGINV = (uint32_t)_bits
#define mPORTGCloseAll()                    (TRISGSET = 0xFFFFFFFF,\
                                             LATGCLR = 0xFFFFFFFF)
#define mPORTGCloseBits(_bits)              (TRISGSET = (uint32_t)_bits,\
                                             LATGCLR = (uint32_t)_bits)
#define mPORTGOpenDrainOpen(_bits)          (ODCFGGSET = (uint32_t)_bits,\
                                             TRISGCLR = (uint32_t)_bits)
#define mPORTGOpenDrainClose(_bits)         (ODCFGGCLR = (uint32_t)_bits,\
                                             TRISGSET = (uint32_t)_bits)
#endif  // end PORTG


/******************************************************************************
 * Parameter values to be used with mJTAGEnable()
 *****************************************************************************/
#define DEBUG_JTAG_ON                       (1)
#define DEBUG_JTAG_OFF                      (0)

/******************************************************************************
 * JTAG macro       void mJTAGEnable(uint32_t enable)
 *
 * Function:
 *			        To enable/disable JTAG module
 *					mJTAGEnable(int32_t _enable)
 *
 * Description:		Enables or disables JTAG port.
 *
 * PreCondition:    None
 *
 * Inputs:			0 = disable, 1 = enable
 *
 * Output:          None
 *
 * Example:			mJTAGEnable(1)
 *
 *****************************************************************************/
// disabled until we have an address for ORBCFG0
//#define mJTAGEnable(_enable)                   (ORBCFG0 &= ~(1 << 4),\
                                                ORBCFG0 |= _enable)
#endif  // _PORTS_H_
