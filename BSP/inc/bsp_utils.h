/*******************************************************************************
 * File:    driver_uart.h
 * Author:  Mitchell S. Tilson
 * Created: 03/24/2014
 *
 * Description: This file provides common utilities
 ******************************************************************************/

#ifndef BSP_UTILS_H
#define	BSP_UTILS_H

#ifdef	__cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Includes
 ******************************************************************************/

/*******************************************************************************
 * Defines
 ******************************************************************************/

/*******************************************************************************
 * Type Defs
 ******************************************************************************/

/*******************************************************************************
 * Enumerations
 ******************************************************************************/

/*******************************************************************************
 * Public Data
 ******************************************************************************/

/*******************************************************************************
 * Public Functions
 ******************************************************************************/

void BSP_PrintfInit( void );

void BSP_Printf( const char* format, ... );

#ifdef	__cplusplus
}
#endif

#endif	/* BSP_UTILS_H */

