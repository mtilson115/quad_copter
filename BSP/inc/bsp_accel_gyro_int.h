
/*******************************************************************************
 * File:    bsp_accel_gyro_int.s
 * Author:  Mitchell S. Tilson
 * Created: 01/31/2019
 *
 * Description: This file provides the interrupt service routine for the accel/gyro
 * chip.
 *
 * IN THE CURRENT HW DESIGN, INT1 INTERFERES WITH INT2.  I BELIEVE THE SIGNAL
 * FROM THE MPU6050 IS TOO NOISY.  THIS FILE IS UNUSED BUT LEFT JUST IN CASE.
 ******************************************************************************/

#pragma once
#ifndef BSP_ACCEL_GYRO_INT_H
#define	BSP_ACCEL_GYRO_INT_H

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
