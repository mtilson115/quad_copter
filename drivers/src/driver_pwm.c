/*******************************************************************************
 * File:    driver_pwm.c
 * Author:  Mitchell S. Tilson
 * Created: 08/01/2019
 *
 * Description: This file contians the driver interface for the PWM module.
 ******************************************************************************/

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "driver_pwm.h"
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
    volatile uint32_t* OCxRS;
    volatile uint32_t* OCxR;
    bool initialized;
} pwm_config_t;

/*******************************************************************************
 * Local Data
 ******************************************************************************/
pwm_config_t PWM1_cfg = {
    &OC1CONbits,
    &OC1RS,
    &OC1R,
    false,
};

pwm_config_t PWM2_cfg = {
    /*
     * This cast seems dangerous, but is ok given the registers
     * are all defined the same way.
     */
    (__OC1CONbits_t*)&OC2CONbits,
    &OC2RS,
    &OC2R,
    false,
};

pwm_config_t PWM3_cfg = {
    (__OC1CONbits_t*)&OC3CONbits,
    &OC3RS,
    &OC3R,
    false,
};

pwm_config_t PWM4_cfg = {
    (__OC1CONbits_t*)&OC4CONbits,
    &OC4RS,
    &OC4R,
    false,
};

static bool pwm_tmr_initialized = false;

pwm_config_t* curr_pwm_cfg_p;

/*******************************************************************************
 * Local Functions
 ******************************************************************************/
static int32_t pwm_set_cfg_pointer( pwm_num_e pwm );

/*******************************************************************************
 * Public function section
 ******************************************************************************/

/*******************************************************************************
 * PWM_init_tmr
 *
 * Description: Initializes the PWM timer
 *
 * Inputs:      uint32_t period - periodicity of the pulse
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 02/16/2019 - Mitchell S. Tilson
 *
 * Notes:       The clock is set up for 80MHz and the prescale value is set to 256.
 *
 ******************************************************************************/
void PWM_init_tmr( uint32_t period )
{
    // Set up the period
    PR2 = period;

    // Set the timer prescale value
    // TCKPS<2:0>: Timer Input Clock Prescale Select bits
    // 111 = 1:256 prescale value
    // 110 = 1:64 prescale value
    // 101 = 1:32 prescale value
    // 100 = 1:16 prescale value
    // 011 = 1:8 prescale value
    // 010 = 1:4 prescale value
    // 001 = 1:2 prescale value
    T2CONbits.TCKPS = 6;

    // Clear timer 2
    TMR2 = 0;

    pwm_tmr_initialized = true;
}

/*******************************************************************************
 * PWM_init (call PWM_init_tmr first)
 *
 * Description: Initializes the PWM identified by the pwm_num_e using the passed
 *              pwm_init_t settings. (call PWM_init_tmr first)
 *
 * Inputs:      pwm_num_e pwm - the pwm to initialize
 *              pwm_init_t init_settings:
 *                  uint32_t period - periodicity of the pulse
 *                  uint32_t duty - duty cycle
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 08/01/2019 - Mitchell S. Tilson
 *
 * Notes:       The clock is set up for 80MHz and the prescale value is set to 256.
 *              For a 16bit timer, this means the slowest period is (1/80e6)*65535*64 = 0.052428
 *              The period can be calculated as perod_s = (1/80e6)*(init_settings.period)*256.
 *              (call PWM_init_tmr first)
 *
 ******************************************************************************/
void PWM_init( pwm_num_e pwm, pwm_init_t init_settings )
{
    if( -1 == pwm_set_cfg_pointer( pwm ) )
    {
        return;
    }

    if( pwm_tmr_initialized == false )
    {
        return;
    }

    // Set the intialized flag
    curr_pwm_cfg_p->initialized = false;

    // Can't be greater than 2^16 - 1
    if( init_settings.period > 65535 )
    {
        return;
    }

    // Disable the PWM module
    curr_pwm_cfg_p->OCxCON->ON = 0;
    __asm__("nop");

    // Set up the duty cycle
    float percentage = (float)init_settings.duty/100.0;
    float clk_val = ((float)init_settings.period)*percentage;
    *curr_pwm_cfg_p->OCxRS = (uint32_t)(clk_val + 0.5);
    *curr_pwm_cfg_p->OCxR = *curr_pwm_cfg_p->OCxRS;
    curr_pwm_cfg_p->OCxCON->OCM = PWM_MODE_NO_FAULT_DETECTION;

    // Set the intialized flag
    curr_pwm_cfg_p->initialized = true;
}

/*******************************************************************************
 * PWM_tmr_en
 *
 * Description: Enables or disables Timer 2
 *
 * Inputs:      bool en - enable (true) or disable (false) timer 2
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 02/16/2019 - Mitchell S. Tilson
 *
 * Notes:
 *
 ******************************************************************************/
void PWM_tmr_en( bool en )
{
    if( pwm_tmr_initialized )
    {
        T2CONbits.ON = en;
        __asm__("nop");
    }
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
 *              02/14/2016 - chech intialization.
 *              02/16/2019 - Timer 2 is used for all PWMs
 *
 * Notes:
 *
 ******************************************************************************/
void PWM_start( pwm_num_e pwm )
{
    if( -1 == pwm_set_cfg_pointer( pwm ) )
    {
        return;
    }

    if( !curr_pwm_cfg_p->initialized )
    {
        return;
    }

    // Enable the pwm
    curr_pwm_cfg_p->OCxCON->ON = 1;
    __asm__("nop");
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
 *              02/14/2016 - chech intialization.
 *
 * Notes:
 *
 ******************************************************************************/
void PWM_stop( pwm_num_e pwm )
{
    if( -1 == pwm_set_cfg_pointer( pwm ) )
    {
        return;
    }

    if( !curr_pwm_cfg_p->initialized )
    {
        return;
    }

    // Disable the pwm
    curr_pwm_cfg_p->OCxCON->ON = 0;
    __asm__("nop");
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
 *              02/14/2016 - chech intialization.
 *
 * Notes:
 *
 ******************************************************************************/
void PWM_chg_duty( pwm_num_e pwm, float duty_cycle )
{
    if( -1 == pwm_set_cfg_pointer( pwm ) )
    {
        return;
    }

    if( !curr_pwm_cfg_p->initialized )
    {
        return;
    }

    float percentage = (float)duty_cycle/100.0;
    float clk_val = (float)(PR2)*percentage;
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
static int32_t pwm_set_cfg_pointer( pwm_num_e pwm )
{
    int32_t ret = 0;

    // Get the appropriate configuration
    // to modify
    switch( pwm )
    {
        case PWM1:
            curr_pwm_cfg_p = &PWM1_cfg;
            break;

        case PWM2:
            curr_pwm_cfg_p = &PWM2_cfg;
            break;

        case PWM3:
            curr_pwm_cfg_p = &PWM3_cfg;
            break;

        case PWM4:
            curr_pwm_cfg_p = &PWM4_cfg;
            break;

        default:
            curr_pwm_cfg_p = NULL;
            ret = -1;
            break;
    }

    return ret;
}
