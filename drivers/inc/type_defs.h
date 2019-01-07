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
 * Common return types
 ******************************************************************************/
typedef enum {
    rSUCCESS,
    rFAILURE,
    rSPI_SUCCESS,
    rSPI_BUFF_FULL,
    rSPI_ERROR,
} ret_t;

#ifdef	__cplusplus
}
#endif

#endif	/* TYPE_DEFS_H */

