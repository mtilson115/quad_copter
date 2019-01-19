/*******************************************************************************
 * File:    bsp_xbee.h
 * Author:  Mitchell S. Tilson
 * Created: 01/17/2019
 *
 * Description: This file contains the hardware abstraction layer for the XBEE
 *              module.
 ******************************************************************************/
#pragma once
#ifndef BSP_XBEE_H
#define	BSP_XBEE_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <os.h>

void BSP_xbee_init( void );
void BSP_xbee_register_sem(OS_SEM* sem);
void BSP_xbee_test( void );

#ifdef	__cplusplus
}
#endif

#endif
