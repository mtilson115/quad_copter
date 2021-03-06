/*******************************************************************************
 * File:    driver_flash.h
 * Author:  Mitchell S. Tilson
 * Created: 06/08/2019
 *
 * Description: This file provides a driver interface to the flash programming
 * periperial.
 ******************************************************************************/
#pragma once
#ifndef DRIVER_FLASH_H
#define	DRIVER_FLASH_H

#ifdef	__cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Includes
 ******************************************************************************/

/*******************************************************************************
 * Public Functions
 ******************************************************************************/
unsigned int driver_flash_unlock( unsigned int nvmop );

unsigned int driver_flash_write_word( unsigned int address, unsigned int data );

unsigned int driver_flash_erase_page( unsigned int address );


#ifdef	__cplusplus
}
#endif

#endif	/* DRIVER_FLASH_H*/
