/*******************************************************************************
 * File:    type_defs.h
 * Author:  Mitchell S. Tilson
 * Created: 03/24/2014
 *
 * Description: This file provides common type definitions
 ******************************************************************************/

#ifndef TYPE_DEFS_H
#define	TYPE_DEFS_H

#ifdef	__cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Boolean
 ******************************************************************************/
typedef enum {
	FALSE,
  TRUE,
} BOOL;

/*******************************************************************************
 * Integers
 ******************************************************************************/
 typedef char               int8_t;
 typedef unsigned char      uint8_t;
 typedef short              int16_t;
 typedef unsigned short     uint16_t;
 typedef int                int32_t;
 typedef unsigned int       uint32_t;

#ifdef	__cplusplus
}
#endif

#endif	/* TYPE_DEFS_H */

