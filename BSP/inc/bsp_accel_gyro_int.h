
/*******************************************************************************
 * File:    bsp_accel_gyro_int.s
 * Author:  Mitchell S. Tilson
 * Created: 01/31/2019
 *
 * Description: This file provides the interrupt service routine for the accel/gyro
 * chip.
 ******************************************************************************/

#pragma once
#ifndef bsp_accel_gyro_int_h
#define	bsp_accel_gyro_int_h

#ifdef	__cplusplus
extern "C" {
#endif

#include <os.h>

void bsp_accel_gyro_int( void );
void bsp_accel_gyro_int_en( void );
void bsp_accel_gyro_int_register( OS_TCB* tcb );

#ifdef	__cplusplus
}
#endif

#endif
