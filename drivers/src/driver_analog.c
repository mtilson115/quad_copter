
/*******************************************************************************
 * File:    driver_analog.c
 * Author:  Mitchell S. Tilson
 * Created: 06/01/2019
 *
 * Description: This file provides a driver interface to the analog to digital
 * periperial.
 ******************************************************************************/

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "driver_analog.h"
#include <p32xxxx.h>

/*******************************************************************************
 * Macros/definitions
 ******************************************************************************/
#define DRIVER_ANALOG_NUM_SAMPS (5) // Value is zero indexed so the its actually DRIVER_ANALOG_NUM_SAMPS + 1

/*******************************************************************************
 * Public function section
 ******************************************************************************/

/*******************************************************************************
 * driver_analog_init
 *
 * Description: Initializes the analog parameters that are common
 *
 * Inputs:      None
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 06/01/2019 - Mitchell S. Tilson
 *
 * Notes:       None
 *
 ******************************************************************************/
void driver_analog_init( void )
{
    // Disable the module
    AD1CON1bits.ON = 0;

    // The next instruction can't write an SFR for the peripheral
    __asm__ volatile("nop");

    // Set the format to signed integer 16 bit
    AD1CON1bits.FORM = 1;

    // Set the conversion for auto convert (internal counter)
    AD1CON1bits.SSRC = 7;

    // Set the voltage reference to be AVDD, AVSS
    AD1CON2bits.VCFG = 0;

    // Disable scanning inputs
    AD1CON2bits.CSCNA = 0;

    // Indicate done after DRIVER_ANALOG_NUM_SAMPS
    AD1CON2bits.SMPI = DRIVER_ANALOG_NUM_SAMPS;

    // Set the buffer fill mode as a single 16-word buffer (all ADC samples taken before reading)
    AD1CON2bits.BUFM = 0;

    // Always use muxA settings
    AD1CON2bits.ALTS = 0;

    // Select the ADC clock source (PBCLK 80MHz)
    AD1CON3bits.ADRC = 0;

    // Set the sample time 31 TADs
    AD1CON3bits.SAMC = 0x1F;

    // Set the ADC clock pre-scale 512 PBCLK = TAD
    AD1CON3bits.ADCS = 0xFF;

    // Clear the ASAM when conversion is done
    AD1CON1bits.CLRASAM = 1;
}

/*******************************************************************************
 * driver_analog_init_pin
 *
 * Description: Initializes a pin to be analog
 *
 * Inputs:      unsigned int port - the port the pin is on (0 == PORTA, 1 == PORTB, etc.)
 *              unsigned int pin - the pin to configure
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 06/01/2019 - Mitchell S. Tilson
 *
 * Notes:       None
 *
 ******************************************************************************/
void driver_analog_init_pin( unsigned int port, unsigned int pin )
{
    // Configure pin
    AD1PCFG &= ~(1 << pin);

    // Configure the pin as an input
    switch (port)
    {
        case 0:
            TRISA |= (1 << pin);
            break;
        case 1:
            TRISB |= (1 << pin);
            break;
        case 2:
            TRISC |= (1 << pin);
            break;
        case 3:
            TRISD |= (1 << pin);
            break;
        case 4:
            TRISE |= (1 << pin);
            break;
        default:
            break;
    }
}

/*******************************************************************************
 * driver_analog_get_10bit
 *
 * Description: Takes a reading for a given pin
 *
 * Inputs:      unsigned int pin
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 06/01/2019 - Mitchell S. Tilson
 *
 * Notes:       None
 *
 ******************************************************************************/
int16_t driver_analog_get_10bit( unsigned int pin )
{

    // Set up pin in mux for reading
    AD1CHSbits.CH0SA = pin;

    // Start the reading using auto sampling mode
    AD1CON1bits.ON = 1;
    IFS1bits.AD1IF = 0;
    AD1CON1bits.ASAM = 1;

    // Wait for the result
    for( uint32_t i = 0; i < 10000; i++ ) 
    {
        if( IFS1bits.AD1IF )
        {
            break;
        }
    }
    if( !IFS1bits.AD1IF )
    {
        AD1CON1bits.ON = 0;
        return 0.0;
    }

    // Get the reading
    int32_t tmp_result = 0;
    volatile uint32_t* buff_ptr = &ADC1BUF0;
    for( uint32_t i = 0; i <= DRIVER_ANALOG_NUM_SAMPS; i++ )
    {
        tmp_result += *buff_ptr;
        // For some reason, the analog registers are separated
        // by 4 addresses.
        buff_ptr += 0x04;
    }
    float result = (float)tmp_result/(DRIVER_ANALOG_NUM_SAMPS+1);

    // Disable the module
    AD1CON1bits.ON = 0;

    // The next instruction can't write an SFR for the peripheral
    __asm__ volatile("nop");

    // Add .5 for rounding
    return (int16_t)(result+.5);
}
