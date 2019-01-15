/*******************************************************************************
 * File:    hal_xbee.h
 * Author:  Mitchell S. Tilson
 * Created: 01/07/2019
 *
 * Description: This file contains the hardware abstraction layer for the XBEE
 *              module.
 ******************************************************************************/
#pragma once
#ifndef HAL_XBEE_H
#define	HAL_XBEE_H

#ifdef	__cplusplus
extern "C" {
#endif

void HAL_xbee_init( void );
void HAL_xbee_test( void );

#ifdef	__cplusplus
}
#endif

#endif
