/*******************************************************************************
 * File:    bsp_cal.h
 * Author:  Mitchell S. Tilson
 * Created: 09/30/2019
 *
 * Description: Provides utilities for writing calibration to the flash.
 ******************************************************************************/
#pragma once
#ifndef BSP_CAL_H
#define	BSP_CAL_H

#ifdef	__cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdint.h>

/*******************************************************************************
 * Public Functions
 ******************************************************************************/
void bsp_cal_init( void );
const uint32_t* bsp_cal_request_space( uint32_t size );

#ifdef	__cplusplus
}
#endif

#endif	/* BSP_CAL_H */
