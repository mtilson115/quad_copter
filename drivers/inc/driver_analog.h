
/*******************************************************************************
 * File:    driver_analog.h
 * Author:  Mitchell S. Tilson
 * Created: 06/01/2019
 *
 * Description: This file provides a driver interface to the analog to digital
 * periperial.
 ******************************************************************************/
#pragma once
#ifndef DRIVER_ANALOG_H
#define	DRIVER_ANALOG_H

#ifdef	__cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Includes
 ******************************************************************************/
    #include "type_defs.h"

/*******************************************************************************
 * Public Functions
 ******************************************************************************/

    void driver_analog_init( void );

    void driver_analog_init_pin( unsigned int port, unsigned int pin );

    int16_t driver_analog_get_10bit( unsigned int pin );

#ifdef	__cplusplus
}
#endif

#endif	/* DRIVER_ANALOG_H*/
