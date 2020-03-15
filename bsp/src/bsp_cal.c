/*******************************************************************************
 * File:    bsp_cal.c
 * Author:  Mitchell S. Tilson
 * Created: 09/30/2019
 *
 * Description: This file provides an interface for writing calibration to the
 * flash.
 ******************************************************************************/

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "bsp_cal.h"
#include "driver_flash.h"
#include "comms_xbee.h"
#include <assert.h>
#include <sys/kmem.h>
#include <string.h>

/*******************************************************************************
 * Local definitions
 ******************************************************************************/
#define CAL_HDR 0xA5A5A5A5

/*******************************************************************************
 * Local static function definitions
 ******************************************************************************/
static void bsp_cal_erase_cb( uint8_t* data, uint16_t len );

/*******************************************************************************
 * Local static data
 ******************************************************************************/
const uint32_t bsp_cal_space[256] __attribute__((section (".cal_space"),space(prog))) = {1};
uint32_t bsp_cal_words_used = 1; // The header for the calibration space uses 1 byte

/*******************************************************************************
 * Public function section
 ******************************************************************************/

/*******************************************************************************
 * bsp_cal_init
 *
 * Description: Initializes the motor cal bsp
 *
 * Inputs:      none
 *
 * Returns:     none
 *
 * Revision:    Initial Creation 09/29/2019 - Mitchell S. Tilson
 *
 ******************************************************************************/
void bsp_cal_init( void )
{
    comms_xbee_rx_cb_t bsp_cal_erase_rx_cb = {
        .cb = bsp_cal_erase_cb,
        .msg_id = COMMS_ERASE_CAL,
    };
    ret_t ret = comms_xbee_register_rx_cb(bsp_cal_erase_rx_cb);
    assert(ret==rSUCCESS);
}

/*******************************************************************************
 * bsp_cal_request_space
 *
 * Description: Allocates flash space to a client
 *
 * Inputs:      none
 *
 * Returns:     none
 *
 * Revision:    Initial Creation 09/29/2019 - Mitchell S. Tilson
 *
 ******************************************************************************/
const uint32_t* bsp_cal_request_space( uint32_t size )
{
    const uint32_t* mem_ptr = &bsp_cal_space[bsp_cal_words_used];
    bsp_cal_words_used += size;
    return mem_ptr;
}

/*******************************************************************************
 * bsp_cal_erase_cb
 *
 * Description: Erases the calibration flash space when called from the comms
 *              layer.
 *
 * Inputs:      none
 *
 * Returns:     none
 *
 * Revision:    Initial Creation 09/29/2019 - Mitchell S. Tilson
 *
 * Notes: Calibration is read for all motors at once
 *
 ******************************************************************************/
static void bsp_cal_erase_cb( uint8_t* data, uint16_t len )
{
    (void)len;
    if( data[0] == COMMS_ERASE_CAL )
    {
        driver_flash_erase_page(KVA_TO_PA(bsp_cal_space));
        driver_flash_write_word(KVA_TO_PA(&bsp_cal_space[0]),CAL_HDR);
    }
}
