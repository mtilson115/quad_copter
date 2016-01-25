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
    uint32_t duty;
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

/*******************************************************************************
 * PWM_init
 * 
 * Description: Initializes the PWM identified by the pwm_num_e using the passed
 *              pwm_init_t settings.
 * 
 * Inputs:      pwm_num_e pwm - the pwm to initialize
 *              pwm_init_t init_settings:
 *                  uint32_t period - periodicity of the pulse
 *                  uint32_t duty - duty cycle
 * 
 * Returns:     None
 * 
 * Revision:    Initial Creation 01/24/2016 - Mitchell S. Tilson
 * Revision:    01/25/2016 - Mitchell S. Tilson - Update to use pwm_set_cfg_pointer,
 *              rearrange items, and to disable the peripheral first.
 * 
 * Notes:       None
 *          
 ******************************************************************************/
void PWM_init( pwm_num_e pwm, pwm_init_t init_settings );

/*******************************************************************************
 * PWM_start
 * 
 * Description: Starts the selected PWM
 * 
 * Inputs:      pwm_num_e pwm - the pwm start
 * 
 * Returns:     None
 * 
 * Revision:    Initial Creation 01/25/2016 - Mitchell S. Tilson
 * 
 * Notes:       Need to add check to see if it is initialized first
 *          
 ******************************************************************************/
void PWM_start( pwm_num_e pwm );

/*******************************************************************************
 * PWM_stop
 * 
 * Description: Stop the selected PWM
 * 
 * Inputs:      pwm_num_e pwm - the pwm start
 * 
 * Returns:     None
 * 
 * Revision:    Initial Creation 01/25/2016 - Mitchell S. Tilson
 * 
 * Notes:       Need to add check to see if it is initialized first
 *          
 ******************************************************************************/
void PWM_stop( pwm_num_e pwm );

#ifdef	__cplusplus
}
#endif

#endif	/* DRIVER_PWM_H */
