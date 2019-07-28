/*******************************************************************************
 * File:    type_defs.h
 * Author:  Mitchell S. Tilson
 * Created: 03/24/2014
 *
 * Description: This file provides common type definitions
 ******************************************************************************/

#ifndef TYPE_DEFS_H
#define	TYPE_DEFS_H

#include <stdint.h>

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
 * Definitions for uintX_t
 ******************************************************************************/
/*
#ifdef CC_VERS1p00
    typedef unsigned int uint32_t;
    typedef unsigned char uint8_t;
    typedef unsigned short uint16_t;
    typedef unsigned long uint64_t;
    typedef int int32_t;
    typedef char int8_t;
    typedef short int16_t;
    typedef long int64_t;
#endif
*/

/*******************************************************************************
 * Common return types
 ******************************************************************************/
typedef enum {
    rSUCCESS,
    rFAILURE,
    rSPI_SUCCESS,
    rSPI_BUFF_FULL,
    rSPI_ERROR,
} ret_t;

typedef __attribute__((aligned(1))) uint8_t uint8_ua_t;
typedef __attribute__((aligned(1))) uint16_t uint16_ua_t;

/*******************************************************************************
 * Macros
 ******************************************************************************/
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

#ifdef	__cplusplus
}
#endif

#endif	/* TYPE_DEFS_H */

