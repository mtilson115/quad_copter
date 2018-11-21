/*******************************************************************************
 * File:    driver_pwm.c
 * Author:  Mitchell S. Tilson
 * Created: 01/24/2016
 *
 * Description: This file contians the driver interface for the PWM module.
 ******************************************************************************/

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "driver_pwm.h"
#include "perif_sfr_map.h"
#include <p32xxxx.h>

/*******************************************************************************
 * Local Defines
 ******************************************************************************/
#define PWM_MODE_NO_FAULT_DETECTION (6)

/*******************************************************************************
 * Local Type Defs
 ******************************************************************************/
typedef struct {
    volatile __OC1CONbits_t* OCxCON;
    volatile uint32_t* PRy;
    volatile uint32_t* OCxRS;
    volatile uint32_t* OCxR;
    volatile uint32_t* TMRy;
    volatile __T2CONbits_t* TxCON;
} pwm_config_t;

/*******************************************************************************
 * Local Data
 ******************************************************************************/
pwm_config_t PWM0_cfg = {
    &OC1CONbits,
    &PR2,
    &OC1RS,
    &OC1R,
    &TMR2,
    &T2CONbits,
};

pwm_config_t* curr_pwm_cfg_p;

/* These are not supported right now
pwm_config_t PWM1_cfg;
pwm_config_t PWM2_cfg;
pwm_config_t PWM3_cfg;
pwm_config_t PWM4_cfg;
*/

/*******************************************************************************
 * Local Functions
 ******************************************************************************/
int32_t pwm_set_cfg_pointer( pwm_num_e pwm );

/*******************************************************************************
 * Public function section
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
void PWM_init( pwm_num_e pwm, pwm_init_t init_settings )
{
    if( -1 == pwm_set_cfg_pointer( pwm ) )
    {
        return;
    }

    // Can't be greater than 2^16 - 1
    if( init_settings.period > 65535 )
    {
        return;
    }

    // Disable the PWM module
    curr_pwm_cfg_p->OCxCON->ON = 0;

    // Set up the duty cycle
    float percentage = (float)init_settings.duty/100.0;
    float clk_val = ((float)init_settings.period)*percentage;
    *curr_pwm_cfg_p->OCxRS = (uint32_t)(clk_val + 0.5);
    *curr_pwm_cfg_p->OCxR = *curr_pwm_cfg_p->OCxRS;

    // Set up the period
    *curr_pwm_cfg_p->PRy = init_settings.period;

    // Set up the mode to be PWM and no fault detection
    curr_pwm_cfg_p->OCxCON->OCM = PWM_MODE_NO_FAULT_DETECTION;

    // Set the timer prescale value
    // TCKPS<2:0>: Timer Input Clock Prescale Select bits
    // 111 = 1:256 prescale value
    // 110 = 1:64 prescale value
    // 101 = 1:32 prescale value
    // 100 = 1:16 prescale value
    // 011 = 1:8 prescale value
    // 010 = 1:4 prescale value
    // 001 = 1:2 prescale value
    // Current setting is 64
    curr_pwm_cfg_p->TxCON->TCKPS0 = 0;
    curr_pwm_cfg_p->TxCON->TCKPS1 = 1;
    curr_pwm_cfg_p->TxCON->TCKPS2 = 1;
}

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
void PWM_start( pwm_num_e pwm )
{
    if( -1 == pwm_set_cfg_pointer( pwm ) )
    {
        return;
    }

    // Enable the timer
    curr_pwm_cfg_p->TxCON->ON = 1;

    // Enable the pwm
    curr_pwm_cfg_p->OCxCON->ON = 1;
}

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
void PWM_stop( pwm_num_e pwm )
{
    if( -1 == pwm_set_cfg_pointer( pwm ) )
    {
        return;
    }

    // Disable the pwm
    curr_pwm_cfg_p->OCxCON->ON = 0;

    // Disable the timer
    curr_pwm_cfg_p->TxCON->ON = 0;
}

/*******************************************************************************
 * PWM_chg_duty
 *
 * Description: changes the duty cycle
 *
 * Inputs:      pwm_num_e pwm - the pwm start
 *              float duty - duty cycle (0 to 100)
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 01/25/2016 - Mitchell S. Tilson
 *
 * Notes:       Need to add check to see if it is initialized first
 *
 ******************************************************************************/
void PWM_chg_duty( pwm_num_e pwm, float duty_cycle )
{
    if( -1 == pwm_set_cfg_pointer( pwm ) )
    {
        return;
    }

    float percentage = (float)duty_cycle/100.0;
    float clk_val = (float)(*curr_pwm_cfg_p->PRy)*percentage;
    *curr_pwm_cfg_p->OCxRS = (uint32_t)(clk_val + 0.5);
}

/*******************************************************************************
 * pwm_set_cfg_pointer
 *
 * Description: Sets the configuration pointer
 *
 * Inputs:      pwm_num_e pwm - the pwm to set the configuration for
 *
 * Returns:     int32_t - -1 if the init fails
 *
 * Revision:    Initial Creation 01/25/2016 - Mitchell S. Tilson
 *
 * Notes:       None
 *
 ******************************************************************************/
int32_t pwm_set_cfg_pointer( pwm_num_e pwm )
{
    if( pwm != PWM0 )
    {
        return -1;
    }

    // Get the appropriate configuration
    // to modify
    switch( pwm )
    {
        case PWM0:
            curr_pwm_cfg_p = &PWM0_cfg;
            break;

        default:
            curr_pwm_cfg_p = &PWM0_cfg;
            break;
    }
}

