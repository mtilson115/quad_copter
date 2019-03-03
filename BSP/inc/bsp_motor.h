/*******************************************************************************
 * File:    bsp_motor.h
 * Author:  Mitchell S. Tilson
 * Created: 03/01/2019
 *
 * Description: This file provides the motor control class
 *
 ******************************************************************************/

#pragma once
#ifndef BSP_MOTOR_H
#define	BSP_MOTOR_H

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "driver_pwm.h"

/*******************************************************************************
 * Class definition
 ******************************************************************************/
class BSPMotor {

    /*******************************************************************************
     * Public Functions
     ******************************************************************************/
    public:
        /*
         * Constructor (does nothing right now)
         */
        BSPMotor( void );

        /*
         * Class init for the timer
         */
        static void InitTmr( void );

        /*
         * Object initialize PWM set up
         */
        void Init( pwm_num_e pwm );

        /*
         * Class enabling of Timer2
         */
        static void TmrEn( void );

        /*
         * Start the motor (starts at 0%)
         */
        void Start( void );

        /*
         * Set the speed between 0% and 100%
         */
        void SetSpeedPercent( float speed );

    /*******************************************************************************
     * private data
     ******************************************************************************/
    private:

        /*
         * PWM output associated with this object
         */
        pwm_num_e _pwm;

        /*
         * PWM initialization data
         */
        static const pwm_init_t pwm_init_data;

};

#endif
