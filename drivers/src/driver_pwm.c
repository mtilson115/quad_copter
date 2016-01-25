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
    volatile __T1CONbits_t* TxCON;
} pwm_config_t;

/*******************************************************************************
 * Local Data
 ******************************************************************************/
pwm_config_t PWM0_cfg = {
    &OC1CONbits,
    &PR1,
    &OC1RS,
    &OC1R,
    &TMR1,
    &T1CONbits,
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
 * 
 * Notes:       None
 *          
 ******************************************************************************/
void PWM_init( pwm_num_e pwm, pwm_init_t init_settings )
{
    if( pwm != PWM0 )
    {
        return;
    }

    // Can't be greater than 2^16 - 1
    if( init_settings.period > 65535 ) 
    {
        return;
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

    // Set up the period
    *curr_pwm_cfg_p->PRy = init_settings.period;

    // Set up the duty cycle
    float percentage = (float)init_settings.duty/100.0;
    float clk_val = ((float)init_settings.period)*percentage;
    *curr_pwm_cfg_p->OCxRS = (uint32_t)(clk_val + 0.5);
    *curr_pwm_cfg_p->OCxR = *curr_pwm_cfg_p->OCxRS;

    // Set up the mode to be PWM and no fault detection
    curr_pwm_cfg_p->OCxCON->OCM = PWM_MODE_NO_FAULT_DETECTION;

    // Set the timer prescale value
    // TCKPS<1:0>: Timer Input Clock Prescale Select bits
    // 11 = 1:256 prescale value
    // 10 = 1:64 prescale value
    // 01 = 1:8 prescale value
    // 00 = 1:1 prescale value
    // Current setting is 64
    curr_pwm_cfg_p->TxCON->TCKPS0 = 0;
    curr_pwm_cfg_p->TxCON->TCKPS1 = 1;
} 
