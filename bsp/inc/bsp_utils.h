/*******************************************************************************
 * File:    bsp_utils.h
 * Author:  Mitchell S. Tilson
 * Created: 08/09/2019
 *
 * Description: This file provides the bsp_utils.h API
 ******************************************************************************/
#pragma once
#ifndef BSP_UTILS_H
#define	BSP_UTILS_H

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

void BSP_PrintfInit( void );

void BSP_Printf( const char* format, ... );

#ifdef	__cplusplus
}
#endif

#endif	/* BSP_UTILS_H */
