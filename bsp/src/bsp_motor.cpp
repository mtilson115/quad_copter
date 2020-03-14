/*******************************************************************************
 * File:    bsp_motor.cpp
 * Author:  Mitchell S. Tilson
 * Created: 03/01/2019
 *
 * Description: This file provides the motor control class
 *
 ******************************************************************************/

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "bsp_motor.h"

/*******************************************************************************
 * Private data
 ******************************************************************************/
#define LOW_PWM_DUTY_CYCLE (50.0)
#define MAX_PWM_DUTY_CYCLE (95.0)

const pwm_init_t BSPMotor::pwm_init_data = {
    // (1/80e6)*313*64 = 0.0002504s ~250us
    .period = 313,
#if DO_THROTTLE_CAL == 1
    .duty = MAX_PWM_DUTY_CYCLE,
#else
    .duty = LOW_PWM_DUTY_CYCLE,
#endif
};

/*******************************************************************************
 * Public Method Section
 ******************************************************************************/

/*******************************************************************************
 * BSPMotor
 *
 * Description: Class constructor function
 *
 * Inputs:      None
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 03/01/2019 - Mitchell S. Tilson
 *
 ******************************************************************************/
BSPMotor::BSPMotor( void )
{
}

/*******************************************************************************
 * static InitTmr
 *
 * Description: Initializes the timer
 *
 * Inputs:      None
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 03/01/2019 - Mitchell S. Tilson
 *
 ******************************************************************************/
void BSPMotor::InitTmr( void )
{
    // Initialize the timer
    PWM_init_tmr( pwm_init_data.period );
}

/*******************************************************************************
 * Init
 *
 * Description: Initializes a motor
 *
 * Inputs:      pwm_num_e pwm - The PWM assigned to this object
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 03/01/2019 - Mitchell S. Tilson
 *
 ******************************************************************************/
void BSPMotor::Init( pwm_num_e pwm )
{
    pwm_ = pwm;
    PWM_init( pwm_, pwm_init_data );
}

/*******************************************************************************
 * TmrEn
 *
 * Description: Enables Timer2
 *
 * Inputs:      None
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 03/01/2019 - Mitchell S. Tilson
 *
 ******************************************************************************/
void BSPMotor::TmrEn( void )
{
    PWM_tmr_en( true );
}

/*******************************************************************************
 * Start
 *
 * Description: Starts a given motor
 *
 * Inputs:      None
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 03/01/2019 - Mitchell S. Tilson
 *
 ******************************************************************************/
void BSPMotor::Start( void )
{
    PWM_start( _pwm );
}

/*******************************************************************************
 * SetSpeedPercent
 *
 * Description: Sets the speed of a motor
 *
 * Inputs:      float speed the speed in percentage (0 to 100%)
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 03/01/2019 - Mitchell S. Tilson
 *
 ******************************************************************************/
void BSPMotor::SetSpeedPercent( float speed )
{
    float pwm_duty_cycle = (MAX_PWM_DUTY_CYCLE - LOW_PWM_DUTY_CYCLE)*speed + LOW_PWM_DUTY_CYCLE;
    PWM_chg_duty( _pwm, pwm_duty_cycle );
}
