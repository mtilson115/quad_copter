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
#include <os.h>
#include <p32xxxx.h>
#include <xc.h>
#include <sys/attribs.h>

/*******************************************************************************
 * Local Defines
 ******************************************************************************/
#define PWM_MODE_NO_FAULT_DETECTION (6)
#define SINGLE_COMPARE_MODE_TOGGLE  (3)

/*******************************************************************************
 * Special Pragma for the interrupt handler to use a shadow register set (SRS)
 ******************************************************************************/
#pragma config FSRSSEL = PRIORITY_6

/*******************************************************************************
 * Local Type Defs
 ******************************************************************************/
typedef struct {
    volatile __OC1CONbits_t* OCxCON;
    volatile uint32_t* OCxRS;
    volatile uint32_t* OCxR;
    BOOL initialized;
} pwm_config_t;

/*******************************************************************************
 * Local Data
 ******************************************************************************/
pwm_config_t PWM1_cfg = {
    &OC1CONbits,
    &OC1RS,
    &OC1R,
    FALSE,
};

pwm_config_t PWM2_cfg = {
    /*
     * This cast seems dangerous, but is ok given the registers
     * are all defined the same way.
     */
    (__OC1CONbits_t*)&OC2CONbits,
    &OC2RS,
    &OC2R,
    FALSE,
};

pwm_config_t PWM3_cfg = {
    (__OC1CONbits_t*)&OC3CONbits,
    &OC3RS,
    &OC3R,
    FALSE,
};

pwm_config_t PWM4_cfg = {
    (__OC1CONbits_t*)&OC4CONbits,
    &OC4RS,
    &OC4R,
    FALSE,
};

static BOOL pwm_tmr_initialized = FALSE;

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
    // Current setting is 256
    T2CONbits.TCKPS = 7;

    // Clear timer 2
    TMR2 = 0;

    pwm_tmr_initialized = TRUE;
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
 * Revision:    Initial Creation 01/24/2016 - Mitchell S. Tilson
 * Revision:    01/25/2016 - Mitchell S. Tilson - Update to use pwm_set_cfg_pointer,
 *              rearrange items, and to disable the peripheral first.
 *
 *              02/14/2016 - Update to add in and set the initialized flag.
 *              02/16/2019 - Timer 2 is used for all PWMs
 *
 * Notes:       The clock is set up for 80MHz and the prescale value is set to 256.
 *              For a 16bit timer, this means the slowest period is (1/80e6)*65535*256 = 0.209712.
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

    if( pwm_tmr_initialized == FALSE )
    {
        return;
    }

    // Set the intialized flag
    curr_pwm_cfg_p->initialized = FALSE;

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

    // Set up the mode to be PWM and no fault detection
    if( pwm == PWM3 )
    {
        curr_pwm_cfg_p->OCxCON->OCM = SINGLE_COMPARE_MODE_TOGGLE;
    }
    else
    {
        curr_pwm_cfg_p->OCxCON->OCM = PWM_MODE_NO_FAULT_DETECTION;
    }

    // Set the intialized flag
    curr_pwm_cfg_p->initialized = TRUE;
}

/*******************************************************************************
 * PWM_oc3_work_around_init
 *
 * Description: I blew out OC3 while plugging in an ESC.  This enables the interrupts
 *              for it so that another pin can be used.
 *
 * Inputs:      none
 *
 * Returns:     none
 *
 * Revision:    Initial Creation 02/17/2016 - Mitchell S. Tilson
 *
 * Notes:
 *
 ******************************************************************************/
void PWM_oc3_work_around_init( void )
{
    /*
     * Set up RE6
     */
    TRISEbits.TRISE6 = 0;   // output
    ODCEbits.ODCE6 = 0;     // CMOS outout

    /*
     * Enale TMR2 interrupts
     */
    IFS0bits.T2IF = 0;
    IPC2bits.T2IP = 6;
    IPC2bits.T2IS = 2;
    IEC0bits.T2IE = 1;

    /*
     * Enable OC3 interrupts
     */
    IFS0bits.OC3IF = 0;
    IPC3bits.OC3IP = 6;
    IPC3bits.OC3IS = 2;
    IEC0bits.OC3IE = 1;
}

/*******************************************************************************
 * PWM_tmr2_work_around_int
 *
 * Description: Handles Timer2 interrupts by inverting
 *              RE6.
 *
 * Inputs:      none
 *
 * Returns:     none
 *
 * Revision:    Initial Creation 02/17/2016 - Mitchell S. Tilson
 *
 * Notes:       By definition, these two interrupts can't happen at the same time.
 *
 *              This function should use the microchip ISR creation and comment out
 *              the vector_X code in bsp_a.S as it doesn't involve the OS.
 *
 ******************************************************************************/
void __ISR(_TIMER_2_VECTOR,IPL6SRS) PWM_tmr2_work_around_int( void )
{
    PORTESET = (1<<6);
    IFS0bits.T2IF = 0;
}

/*******************************************************************************
 * PWM_oc3_work_around_int
 *
 * Description: Handles OC3 interrupts by inverting
 *              RE6.
 *
 * Inputs:      none
 *
 * Returns:     none
 *
 * Revision:    Initial Creation 02/17/2016 - Mitchell S. Tilson
 *
 * Notes:       By definition, these two interrupts can't happen at the same time.
 *
 *              This function should use the microchip ISR creation and comment out
 *              the vector_X code in bsp_a.S as it doesn't involve the OS.
 *
 ******************************************************************************/
void __ISR(_OUTPUT_COMPARE_3_VECTOR,IPL6SRS) PWM_oc3_work_around_int( void )
{
    PORTECLR = (1<<6);
    IFS0bits.OC3IF = 0;
}

/*******************************************************************************
 * PWM_tmr_en
 *
 * Description: Enables or disables Timer 2
 *
 * Inputs:      BOOL en - enable (TRUE) or disable (FALSE) timer 2
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 02/16/2019 - Mitchell S. Tilson
 *
 * Notes:
 *
 ******************************************************************************/
void PWM_tmr_en( BOOL en )
{
    if( pwm_tmr_initialized )
    {
        T2CONbits.ON = en;
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
    if( pwm == PWM3 )
    {
        *curr_pwm_cfg_p->OCxR = (uint32_t)(clk_val + 0.5);
    }
    else
    {
        *curr_pwm_cfg_p->OCxRS = (uint32_t)(clk_val + 0.5);
    }
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

