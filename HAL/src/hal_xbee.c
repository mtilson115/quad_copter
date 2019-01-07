/*******************************************************************************
 * File:    hal_xbee.c
 * Author:  Mitchell S. Tilson
 * Created: 01/07/2019
 *
 * Description: This file contains the hardware abstraction layer for the XBEE
 *              module.
 ******************************************************************************/

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "hal_xbee.h"
#include <p32xxxx.h>

/*******************************************************************************
 * Public function section
 ******************************************************************************/

/*******************************************************************************
 * HAL_xbee_init
 *
 * Description: Initialzies the xbee module.
 *
 *
 * Inputs:      none
 *
 * Returns:     ret_t - rSUCCESS
 *
 * Revision:    Initial Creation 01/07/2019 - Mitchell S. Tilson
 *
 * Notes:       This function does the following:
 *              1. Registers an interrupt callback function with the OS for the SPI_ATTN line on the XBEE
 *              2. Places the XBEE in reset
 *              3. Holds the DOUT low
 *              4. Releases reset
 *
 ******************************************************************************/
