/*******************************************************************************
 * File:    driver_flash.c
 * Author:  Mitchell S. Tilson
 * Created: 06/08/2019
 *
 * Description: This file provides a driver interface to the flash programming
 * periperial.
 ******************************************************************************/

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "driver_flash.h"
#include <p32xxxx.h>
#include <os.h>

/*******************************************************************************
 * Public function section
 ******************************************************************************/

/*******************************************************************************
 * driver_flash_unlock
 *
 * Description: Unlocks the flash for writting.
 *
 * Inputs:      unsigned int nvmop (least significant nibble) - The options for the programming sequence
 *              1111 = Reserved
 *              •
 *              •
 *              •
 *              0111 = Reserved
 *              0110 = No operation
 *              0101 = Program Flash Memory (PFM) Erase Operation: Erases PFM if all pages are not write-protected 0100 = Page Erase Operation: Erases page selected by NVMADDR(4)
 *              0011 = Row Program Operation: Programs row selected by NVMADDR(4)
 *              0010 = Double-Word Program Operation: Programs word selected by NVMADDR(3,4)
 *              0001 = Word Program Operation: Programs word selected by NVMADDR(3,4)
 *              0000 = No operation
 *
 * Returns:     Return WRERR and LVDERR Error Status Bits
 *
 * Revision:    Initial Creation 06/08/2019 - Mitchell S. Tilson
 *
 * Notes:       None
 *
 ******************************************************************************/
unsigned int driver_flash_unlock( unsigned int nvmop )
{
    // Suspend or Disable all Interrupts
    CPU_SR_ALLOC();
    CPU_CRITICAL_ENTER();

    // Enable Flash Write/Erase Operations and Select // Flash operation to perform
    NVMCON = nvmop;

    // Write Keys
    NVMKEY = 0xAA996655;
    NVMKEY = 0x556699AA;

    // Start the operation using the Set Register
    NVMCONSET = 0x8000;

    // Wait for operation to complete
    while (NVMCON & 0x8000);

    // Restore Interrupts
    CPU_CRITICAL_EXIT();

    // Disable NVM write enable
    NVMCONCLR = 0x0004000;

    // Return WRERR and LVDERR Error Status Bits
    return (NVMCON & 0x3000);
}

/*******************************************************************************
 * driver_flash_write_word
 *
 * Description: Writes an unsigned int value to flash.
 *
 * Inputs:      None
 *
 * Returns:     WRERR and LVDERR Error Status Bits
 *
 * Revision:    Initial Creation 06/08/2019 - Mitchell S. Tilson
 *
 * Notes:       None
 *
 ******************************************************************************/
unsigned int driver_flash_write_word( unsigned int address, unsigned int data )
{
    unsigned int res;

    // Load data into NVMDATA register
    NVMDATA = data;

    // Load address to program into NVMADDR register
    NVMADDR = (unsigned int) address;

    // Unlock and Write Word
    res = driver_flash_unlock(0x4001);

    // Return WRERR and LVDERR Error Status Bits
    return res;
}

/*******************************************************************************
 * driver_flash_erase_page
 *
 * Description: Erases a page of flash memory
 *
 * Inputs:      None
 *
 * Returns:     WRERR and LVDERR Error Status Bits
 *
 * Revision:    Initial Creation 06/08/2019 - Mitchell S. Tilson
 *
 * Notes:       The passed address should be the start of a page
 *
 ******************************************************************************/
unsigned int driver_flash_erase_page( unsigned int address )
{

    unsigned int res;

    // Set NVMADDR to the Start Address of page to erase
    NVMADDR = (unsigned int) address;

    // Unlock and Erase Page
    res = driver_flash_unlock(0x4004);

    // Return Result
    return res;
}
