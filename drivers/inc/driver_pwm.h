/*******************************************************************************
 * File:    driver_pwm.h
 * Author:  Mitchell S. Tilson
 * Created: 01/24/2016
 *
 * Description: This file provides a driver interface to the PWM periperial.
 ******************************************************************************/
#pragma once
#ifndef DRIVER_PWM_H
#define	DRIVER_PWM_H

#ifdef	__cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "type_defs.h"

/*******************************************************************************
 * Type Defs
 ******************************************************************************/
typedef struct {
    uint32_t period;
    float duty;
} pwm_init_t;

/*******************************************************************************
 * Enumerations
 ******************************************************************************/
typedef enum {
    PWM0 = 0,
    PWM1 = 1,
    PWM2 = 2,
    PWM3 = 3,
    PWM4 = 4,
    PWM_INVALID,
} pwm_num_e;

/*******************************************************************************
 * Public Functions
 ******************************************************************************/

void PWM_init_tmr( uint32_t period );

void PWM_init( pwm_num_e pwm, pwm_init_t init_settings );

void PWM_tmr_en( BOOL en );

void PWM_start( pwm_num_e pwm );

void PWM_stop( pwm_num_e pwm );

void PWM_chg_duty( pwm_num_e pwm, float duty_cycle );

#ifdef	__cplusplus
}
#endif

#endif	/* DRIVER_PWM_H */
