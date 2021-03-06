/*******************************************************************************
 * File:    alg_stabilizer.h
 * Author:  Mitchell S. Tilson
 * Created: 02/12/2019
 *
 * Description: This file takes in accel data, computes roll and pitch, and
 *              outputs PWM values for the motor to keep the quad copter level.
 *
 ******************************************************************************/

#pragma once
#ifndef ALG_STABILIZER_H
#define	ALG_STABILIZER_H

#include <os.h>

void alg_stabilizer_init(void);

/*
 * Debug data
 */
// OS_TCB* alg_stabilizer_get_tcb(void);
// extern CPU_STK alg_stabilizer_stack[ALG_STABILIZER_STK_SIZE];

#endif
