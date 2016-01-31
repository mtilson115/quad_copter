/*******************************************************************************
 * File:    driver_i2c.c
 * Author:  Mitchell S. Tilson
 * Created: 02/27/2014
 *
 * Description: This file provides a driver interface to the i2c periperial.
 * 		The driver only works with i2c in master mode.
 *		Credit to Arduino developers for some of the algorithms to read
 *		bits from an I2C register.
 ******************************************************************************/

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "driver_i2c.h"
#include "type_defs.h"
#include "bsp_utils.h"
#include <p32xxxx.h>

/*******************************************************************************
 * Local Type Defs
 ******************************************************************************/

/*******************************************************************************
 * Constants
 ******************************************************************************/

/*******************************************************************************
 * Local Data
 ******************************************************************************/

/*******************************************************************************
 * Local Functions
 ******************************************************************************/

I2C_Class::I2C_REGS * const I2C_REGISTERS[2] = {
    (I2C_Class::I2C_REGS *)(I2C1_BASE),
    (I2C_Class::I2C_REGS *)(I2C2_BASE)
};

I2C_Class I2C1;
I2C_Class I2C2;

/*******************************************************************************
 * Public Method Section
 ******************************************************************************/

/*******************************************************************************
 * I2C_Class
 *
 * Description: Class constructor function
 *
 * Inputs:      I2C_ID id - i2c number (1 or 2)
 *              I2C_REGS * const reg_base - the register base
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 03/18/2014 - Mitchell S. Tilson
 *
 ******************************************************************************/
I2C_Class::I2C_Class()
{
}

/*******************************************************************************
 * Init
 *
 * Description: Initializes the i2c identified by the ID (only 2 i2c
 *              periferals are supported)
 *
 * Inputs:      float buad - buad rate
 *
 *
 * Revision: Initial Creation 2/27/2014 - Mitchell S. Tilson
 *           Update to c++ 3/18/2014 - Mitchell S. Tilson
 *
 ******************************************************************************/
void I2C_Class::Init( I2C_ID id, float baud )
{
    // Intialize the register base for the I2C peripheral
    regs_ = I2C_REGISTERS[id];

    // set the initialized flag
    initialized_ = FALSE;

    // set the enabled flag
    enabled_ = FALSE;

    // Set up the buad rate
    baud_ = baud;

    // Disable the peripherial for now
    // disable();

    // set up the control register
    con_setup();

    // set up the status register
    stat_setup();

    // set up the address register
    addr_setup();

    // set up the address register
    addr_msk_setup();

    // set up the baud rate
    set_baud();

    // set the initialized flag
    initialized_ = TRUE;

    // Enable the I2C
    enable();
}

/*******************************************************************************
 * ReadRegByte
 *
 * Description:	Reads a register that is 8 bits wide
 *
 * Inputs: I2C_MSG* - the message to send and get data with
 *
 * Revision: Initial Creation 3/15/2014 - Mitchell S. Tilson
 *           Update to c++ 3/18/2014 - Mitchell S. Tilson
 *
 * Notes: I2C_Init must be called first
 ******************************************************************************/
BOOL I2C_Class::ReadRegByte( uint8_t dev_addr, uint8_t reg_addr, uint8_t* reg_byte )
{
	// Build the message
	I2C_MSG msg;
	msg.dev_addr    = dev_addr;
	msg.reg_addr    = reg_addr;
	msg.dir         = kI2C_READ;
	msg.data 	= reg_byte;
	msg.count 	= 1;

	// Communicate with the device to get the register
	if( 1 == communicate(&msg) ) {
            return TRUE;
	} else {
            return FALSE;
	}
}

/*******************************************************************************
 * ReadRegBytes
 *
 * Description:	Reads multiple bytes from a register that is 8 bits wide
 *
 * Inputs: dev_addr - the device address
 *         reg_addr - the device register address
 *         length   - the number of bytes to read
 *         buffer   - the pointer where the bytes will be stored
 *
 * Revision: Initial Creation 3/25/2014
 *
 * Notes: I2C_Init must be called first
 ******************************************************************************/
BOOL I2C_Class::ReadRegBytes( uint8_t dev_addr, uint8_t reg_addr, uint8_t length, uint8_t* buffer )
{
    // Build the message
    I2C_MSG msg;
    uint8_t byte_count = 0;
    msg.dev_addr    = dev_addr;
    msg.reg_addr    = reg_addr;
    msg.dir         = kI2C_READ;
    msg.data        = &buffer[0];
    msg.count       = length;

    // Communicate with the device to get the register
    if( length == communicate(&msg) ) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/*******************************************************************************
 * ReadRegWord
 *
 * Description:	Reads a register that is 16 bits wide
 *
 * Inputs: dev_addr - the device address
 *         reg_addr - the register address
 *         reg_word - pointer to store the word in
 *
 * Revision: Initial Creation 3/15/2014 - Mitchell S. Tilson
 *           Update to c++ 3/18/2014 - Mitchell S. Tilson
 *
 * Notes: I2C_Init must be called first
 ******************************************************************************/
BOOL I2C_Class::ReadRegWord( uint8_t dev_addr, uint8_t reg_addr, uint16_t* reg_word )
{
	// Build the message
	I2C_MSG msg;
	uint8_t data[2];
	msg.dev_addr    = dev_addr;
	msg.reg_addr    = reg_addr;
	msg.dir 	= kI2C_READ;
	msg.data 	= &data[0];
	msg.count 	= 2;

	// Communicate with the device to get the register
	if( 2 == communicate(&msg) ) {
		*reg_word = (((uint16_t)(data[0])) << 8) | ((uint16_t)(data[1]));
		return TRUE;
	} else {
		return FALSE;
	}
}

/*******************************************************************************
 * ReadRegWords
 *
 * Description:	Reads multiple words from a register that is 16 bits wide
 *
 * Inputs: dev_addr - the device address
 *         reg_addr - the register address
 *         length   - the number of words to read
 *         buffer   - pointer to store the words in
 *
 * Revision: Initial Creation 3/25/2014 - Mitchell S. Tilson
 *           Update to c++ 3/18/2014 - Mitchell S. Tilson
 *
 * Notes: I2C_Init must be called first
 ******************************************************************************/
BOOL I2C_Class::ReadRegWords( uint8_t dev_addr, uint8_t reg_addr, uint8_t length, uint16_t* buffer )
{
	// Build the message
	I2C_MSG msg;
	uint8_t data[length*2];
	msg.dev_addr    = dev_addr;
	msg.reg_addr    = reg_addr;
	msg.dir 	= kI2C_READ;
	msg.data 	= &data[0];
	msg.count 	= length*2;

	// Communicate with the device to get the register
	if( length*2 == communicate(&msg) ) {
        for( uint32_t i = 0; i < length; i++ ) {
            buffer[i] = (((uint16_t)(data[2*i])) << 8) | ((uint16_t)(data[2*i+1]));
        }
		return TRUE;
	} else {
		return FALSE;
	}
}

/*******************************************************************************
 * ReadRegBitB
 *
 * Description:	Reads a bit from an 8 bit register
 *
 * Inputs: dev_addr - the device address
 *				 reg_addr - the register to read address
 *				 bit_num - the bit number to read
 *				 reg_bit - the register bit read
 *
 * Returns: TRUE if the read was successful FALSE otherwise
 *
 * Revision: Initial Creation 3/24/2014 - Mitchell S. Tilson
 *
 ******************************************************************************/
BOOL I2C_Class::ReadRegBitB( uint8_t dev_addr, uint8_t reg_addr, uint8_t bit_num, uint8_t* reg_bit )
{
	uint8_t reg_byte;
	reg_byte = 0;

	if( TRUE == ReadRegByte( dev_addr, reg_addr, &reg_byte ) ) {
		*reg_bit = reg_byte & (1 << bit_num);
		return TRUE;
	}
	return FALSE;
}

/*******************************************************************************
 * ReadRegBitW
 *
 * Description:	Reads a bit from a 16 bit register
 *
 * Inputs: dev_addr - the device address
 *				 reg_addr - the register to read address
 *				 bit_num - the bit number to read
 *				 reg_bit - the register bit read
 *
 * Returns: TRUE if the read was successful FALSE otherwise
 *
 * Revision: Initial Creation 3/24/2014 - Mitchell S. Tilson
 *
 ******************************************************************************/
BOOL I2C_Class::ReadRegBitW( uint8_t dev_addr, uint8_t reg_addr, uint8_t bit_num, uint16_t* reg_bit )
{
	uint16_t reg_word;
	reg_word = 0;

	if( TRUE == ReadRegWord( dev_addr, reg_addr, &reg_word ) ) {
		*reg_bit = reg_word & (1 << bit_num);
		return TRUE;
	}
	return FALSE;
}

/*******************************************************************************
 * ReadRegBitsB
 *
 * Description: Read multiple bits from an 8 bit device register.
 *
 * Inputs: dev_addr - I2C slave device address
 * 				 reg_addr - Register address to read from
 * 				 bit_start - First bit position to read (0-7)
 * 				 length - Number of bits to read (not more than 8)
 * 				 reg_bits - data for right-aligned value (i.e. '101' read from any bitStart position will equal 0x05)
 *
 * Returns: Status of read operation (TRUE = success)
 *
 * Revision: Initial Creation 3/24/2014 - Mitchell S. Tilson (Credit to Arduino I2Cdev.cpp)
 *
 * Notes (example):
 *				01101001 read byte
 *				76543210 bit numbers
 *				xxx   args: bitStart=4, length=3
 *				010   masked
 *				-> 010 shifted
 ******************************************************************************/
BOOL I2C_Class::ReadRegBitsB( uint8_t dev_addr, uint8_t reg_addr, uint8_t bit_start, uint8_t length, uint8_t* reg_bits )
{
    uint8_t reg_byte;
    if( TRUE == ReadRegByte(dev_addr, reg_addr, &reg_byte) ) {
        uint8_t mask = ((1 << length) - 1) << (bit_start - length + 1);
        reg_byte &= mask;
        reg_byte >>= (bit_start - length + 1);
        *reg_bits = reg_byte;
        return TRUE;
    }
    return FALSE;
}

/*******************************************************************************
 * ReadRegBitsW
 *
 * Description: Read multiple bits from a 16 bit device register.
 *
 * Inputs: dev_addr - I2C slave device address
 * 				 reg_addr - Register address to read from
 * 				 bit_start - First bit position to read (0-7)
 * 				 length - Number of bits to read (not more than 8)
 * 				 reg_bits - data for right-aligned value (i.e. '101' read from any bitStart position will equal 0x05)
 *
 * Returns: Status of read operation (TRUE = success)
 *
 * Revision: Initial Creation 3/24/2014 - Mitchell S. Tilson (Credit to Arduino I2Cdev.cpp)
 *
 * Notes (example):
 *				1101011001101001 read byte
 *				fedcba9876543210 bit numbers
 *				   xxx           args: bitStart=12, length=3
 *				   010           masked
 *				       -> 010 shifted
 ******************************************************************************/
BOOL I2C_Class::ReadRegBitsW( uint8_t dev_addr, uint8_t reg_addr, uint8_t bit_start, uint8_t length, uint16_t* reg_bits )
{
    uint16_t reg_word;
    if( TRUE == ReadRegWord(dev_addr, reg_addr, &reg_word) ) {
        uint16_t mask = ((1 << length) - 1) << (bit_start - length + 1);
        reg_word &= mask;
        reg_word >>= (bit_start - length + 1);
        *reg_bits = reg_word;
	return TRUE;
    }
    return FALSE;
}

/*******************************************************************************
 * WriteRegByte
 *
 * Description:	Writes a register that is 8 bits wide
 *
 * Inputs: dev_addr - the I2C device address
 *				 reg_addr - the I2C device's register address
 *				 reg_byte - the byte to write to the register
 *
 * Revision: Initial Creation 3/24/2014 - Mitchell S. Tilson
 *
 * Notes: I2C_Init must be called first
 ******************************************************************************/
BOOL I2C_Class::WriteRegByte( uint8_t dev_addr, uint8_t reg_addr, uint8_t reg_byte )
{
	// Build the message
	I2C_MSG msg;
	msg.dev_addr    = dev_addr;
	msg.reg_addr    = reg_addr;
	msg.dir         = kI2C_WRITE;
	msg.data        = &reg_byte;
	msg.count 	= 1;

	// Communicate with the device to get the register
	if( 1 == communicate(&msg) ) {
            return TRUE;
	} else {
            return FALSE;
	}
}

/*******************************************************************************
 * WriteRegBytes
 *
 * Description:	Writes multiple bytes to a register that is 8 bits wide
 *
 * Inputs: dev_addr - the I2C device address
 *				 reg_addr - the I2C device's register address
 *               length   - the number of bytes to write
 *				 buffer  -  pointer to the buffer of bytes to write
 *
 * Revision: Initial Creation 3/25/2014 - Mitchell S. Tilson
 *
 * Notes: I2C_Init must be called first
 ******************************************************************************/
BOOL I2C_Class::WriteRegBytes( uint8_t dev_addr, uint8_t reg_addr, uint8_t length, uint8_t* buffer )
{
	// Build the message
	I2C_MSG msg;

	msg.dev_addr  = dev_addr;
	msg.reg_addr 	= reg_addr;
	msg.dir 			= kI2C_WRITE;
	msg.data 			= buffer;
	msg.count 		= length;

	// Communicate with the device to get the register
	if( length == communicate(&msg) ) {
		return TRUE;
	} else {
        return FALSE;
	}
}

/*******************************************************************************
 * WriteRegWord
 *
 * Description:	Writes a register that is 16 bits wide
 *
 * Inputs: dev_addr - the I2C device address
 *				 reg_addr - the I2C device's register address
 *				 reg_word - the word to write to the register
 *
 * Revision: Initial Creation 3/24/2014 - Mitchell S. Tilson
 *
 * Notes: I2C_Init must be called first
 ******************************************************************************/
BOOL I2C_Class::WriteRegWord( uint8_t dev_addr, uint8_t reg_addr, uint16_t reg_word )
{
	// Build the message
	I2C_MSG msg;
	uint8_t data[2];
	data[0] = (uint8_t)(reg_word >> 8);
	data[1] = (uint8_t)(reg_word);
	msg.dev_addr  = dev_addr;
	msg.reg_addr 	= reg_addr;
	msg.dir 			= kI2C_WRITE;
	msg.data 			= data;
	msg.count 		= 2;

	// Communicate with the device to get the register
	if( 2 == communicate(&msg) ) {
		return TRUE;
	} else {
    return FALSE;
	}
}

/*******************************************************************************
 * WriteRegWords
 *
 * Description:	Writes multiple words a register that is 16 bits wide
 *
 * Inputs: dev_addr - the I2C device address
 *				 reg_addr - the I2C device's register address
 *               length - the number of words to write
 *				 buffer - pointer to the buffer containing the words to write
 *
 * Revision: Initial Creation 3/25/2014 - Mitchell S. Tilson
 *
 * Notes: I2C_Init must be called first
 ******************************************************************************/
BOOL I2C_Class::WriteRegWords( uint8_t dev_addr, uint8_t reg_addr, uint8_t length, uint16_t* buffer )
{
	// Build the message
	I2C_MSG msg;
	uint8_t data[2*length];
    for(uint32_t i = 0; i < length; i++ ) {
        data[2*i] = (uint8_t)(buffer[i] >> 8);
        data[2*i+1] = (uint8_t)(buffer[i]);
    }
	msg.dev_addr  = dev_addr;
	msg.reg_addr 	= reg_addr;
	msg.dir 			= kI2C_WRITE;
	msg.data 			= data;
	msg.count 		= length*2;

	// Communicate with the device to get the register
	if( length*2 == communicate(&msg) ) {
		return TRUE;
	} else {
        return FALSE;
	}
}

/*******************************************************************************
 * WriteRegBitB
 *
 * Description:	Write a bit to an 8 bit register
 *
 * Inputs: dev_addr - the device address
 *				 reg_addr - the register to write address
 *				 bit_num - the bit number to write
 *				 reg_bit - the register bit write
 *
 * Returns: TRUE if the write was successful FALSE otherwise
 *
 * Revision: Initial Creation 3/24/2014 - Mitchell S. Tilson
 *
 ******************************************************************************/
BOOL I2C_Class::WriteRegBitB( uint8_t dev_addr, uint8_t reg_addr, uint8_t bit_num, uint8_t reg_bit )
{
	uint8_t reg_byte;
  if( TRUE == ReadRegByte(dev_addr, reg_addr, &reg_byte ) ) {
    reg_byte = (reg_bit != 0) ? (reg_byte | (1 << bit_num)) : (reg_byte & ~(1 << bit_num ));
	} else {
		return FALSE;
	}

	if( TRUE == WriteRegByte( dev_addr, reg_addr, reg_byte ) ) {
		return TRUE;
	}

	return FALSE;
}

/*******************************************************************************
 * WriteRegBitW
 *
 * Description:	Write a bit to a 16 bit register
 *
 * Inputs: dev_addr - the device address
 *				 reg_addr - the register to write address
 *				 bit_num - the bit number to write
 *				 reg_bit - the register bit write
 *
 * Returns: TRUE if the write was successful FALSE otherwise
 *
 * Revision: Initial Creation 3/24/2014 - Mitchell S. Tilson
 *
 ******************************************************************************/
BOOL I2C_Class::WriteRegBitW( uint8_t dev_addr, uint8_t reg_addr, uint8_t bit_num, uint8_t reg_bit )
{
	uint16_t reg_word;
  if( TRUE == ReadRegWord(dev_addr, reg_addr, &reg_word ) ) {
    reg_word = (reg_bit != 0) ? (reg_word | (1 << bit_num)) : (reg_word & ~(1 << bit_num ));
	} else {
		return FALSE;
	}

	if( TRUE == WriteRegWord( dev_addr, reg_addr, reg_word ) ) {
		return TRUE;
	}

	return FALSE;
}

/*******************************************************************************
 * WriteRegBitsB
 *
 * Description:	Write a bit patten to an 8 bit register
 *
 * Inputs: dev_addr - the device address
 *				 reg_addr - the register to write address
 *				 bit_start - starting location of the bit patter to write
 *				 length - the length of the bit pattern to write
 *				 reg_bits - the bit pattern to write
 *
 * Returns: TRUE if the write was successful FALSE otherwise
 *
 * Revision: Initial Creation 3/24/2014 - Mitchell S. Tilson
 *
 * Example:
 *
 *      010 value to write
 * 76543210 bit numbers
 *    xxx   args: bitStart=4, length=3
 * 00011100 mask byte
 * 10101111 original value (sample)
 * 10100011 original & ~mask
 * 10101011 masked | value
 ******************************************************************************/
BOOL I2C_Class::WriteRegBitsB( uint8_t dev_addr, uint8_t reg_addr, uint8_t bit_start, uint8_t length, uint8_t reg_bits )
{
	uint8_t reg_byte;
  if( TRUE == ReadRegByte(dev_addr, reg_addr, &reg_byte ) ) {
    uint8_t mask = ((1 << length) - 1) << (bit_start - length + 1);
    reg_bits <<= (bit_start - length + 1); // shift reg_bits into correct position
    reg_bits &= mask; // zero all non-important bits in reg_bits
    reg_byte &= ~(mask); // zero all important bits in existing byte
    reg_byte |= reg_bits; // combine reg_bits with existing byte
	} else {
		return FALSE;
	}

	if( TRUE == WriteRegByte( dev_addr, reg_addr, reg_byte ) ) {
		return TRUE;
	}

	return FALSE;
}

/*******************************************************************************
 * WriteRegBitsW
 *
 * Description:	Write a bit patten to a 16 bit register
 *
 * Inputs: dev_addr - the device address
 *				 reg_addr - the register to write address
 *				 bit_start - starting location of the bit pattern to write
 *				 length - the length of the bit pattern to write
 *				 reg_bits - the bit pattern to write
 *
 * Returns: TRUE if the write was successful FALSE otherwise
 *
 * Revision: Initial Creation 3/24/2014 - Mitchell S. Tilson
 *
 * Example:
 *
 *              010 value to write
 * fedcba9876543210 bit numbers
 *   xxx            args: bitStart=12, length=3
 * 0001110000000000 mask word
 * 1010111110010110 original value (sample)
 * 1010001110010110 original & ~mask
 * 1010101110010110 masked | value
 ******************************************************************************/
BOOL I2C_Class::WriteRegBitsW( uint8_t dev_addr, uint8_t reg_addr, uint8_t bit_start, uint8_t length, uint16_t reg_bits )
{
	uint16_t reg_word;
  if( TRUE == ReadRegWord(dev_addr, reg_addr, &reg_word ) ) {
    uint16_t mask = ((1 << length) - 1) << (bit_start - length + 1);
    reg_bits <<= (bit_start - length + 1); // shift reg_bits into correct position
    reg_bits &= mask; // zero all non-important bits in reg_bits
    reg_word &= ~(mask); // zero all important bits in existing byte
    reg_word |= reg_bits; // combine reg_bits with existing byte
	} else {
		return FALSE;
	}

	if( TRUE == WriteRegWord( dev_addr, reg_addr, reg_word ) ) {
		return TRUE;
	}

	return FALSE;
}

/*******************************************************************************
 * Private Method Section
 ******************************************************************************/

/*******************************************************************************
 * enable
 *
 * Description: enable enables the specified the i2c port.  Init must
 *              be called first.
 *
 * Inputs: none
 *
 * Returns: none
 *
 * Revision: Initial Creation 1/1/2014 - Mitchell S. Tilson
 *           Update to c++ 3/18/2014 - Mitchell S. Tilson
 *
 ******************************************************************************/
void I2C_Class::enable( void )
{
    // Check if not intialized or already enabled
    if( FALSE == initialized_ || TRUE == enabled_ ) {
        return;
    }

    // Enable the I2C
    regs_->con.set = I2C_CON_ON(1);

    // set the enabled flag
    enabled_ = TRUE;
}

/*******************************************************************************
 * disable
 *
 * Description:     Disables the I2C
 *
 * Inputs: none
 *
 * Revision: Initial Creation 2/27/2014 - Mitchell S. Tilson
 *           Update to c++ 3/18/2014 - Mitchell S. Tilson
 *
 * Note:	This funciton can only be called after I2C_Init
 *
 ******************************************************************************/
void I2C_Class::disable( void )
{
    // Check if not initialized or not enabled
    if( FALSE == initialized_ || FALSE == enabled_ ) {
        return;
    }

    // Disable the I2C
    regs_->con.clr = I2C_CON_ON(1);

    // Delay to avoid anything accessing the SFRs after clearin the ON bit
    __asm__ volatile("nop");
    __asm__ volatile("nop");

    // set the enabled flag
    enabled_ = FALSE;
}

/*******************************************************************************
 * communicate
 *
 * Description: communicates with the I2C device
 *
 * Inputs: I2C_MSG* - the message to send and get data with
 *
 * Revision: Initial Creation 3/15/2014 - Mitchell S. Tilson
 *           Update to c++ 3/18/2014 - Mitchell S. Tilson
 *
 * Notes: Init must be called first
 ******************************************************************************/
int I2C_Class::communicate( I2C_MSG* msg )
{
    uint32_t byte_count = 0;
    if( FALSE == initialized_ ) {
    	return 0;
    }

    // enable the I2C peripheral
    wait_idle();

    // Send the device address
    if( TRUE == send_dev_address(msg,kSTART) ) {
        wait_idle();

        // Send the register address
        if( TRUE == send_reg_address(msg) ) {
            wait_idle();

            // Send the restart
            if( msg->dir == kI2C_READ ) {
                if( TRUE == send_dev_address(msg,kRESTART) ) {
                    wait_idle();
                }
            }

            // Write data
            if( msg->dir == kI2C_WRITE ) {
                // Send the data
                while( msg->count-- != 0 ) {

                    // Send the data
                    if( FALSE == send_byte(msg->data[byte_count]) ) {
                        break;
                    } else {
                        byte_count++;
                        wait_idle();
                    }
                }

                // Read data
            } else if( msg->dir == kI2C_READ ) {

                while( msg->count != 0 ) {

                    // Read the data
                    if( FALSE == read_byte(&msg->data[byte_count]) ) {
                        break;
                    } else if( 1 != msg->count ) {

                        // Send ack
                        regs_->con.clr = I2C_CON_ACKDT(1);
                        regs_->con.set = I2C_CON_ACKEN(1);
                        wait_idle();
                    } else {
                        // Send NACK on last byte
                        regs_->con.set = I2C_CON_ACKDT(1);
                        regs_->con.set = I2C_CON_ACKEN(1);
                        wait_idle();
                    }
                    msg->count--;
                    byte_count++;
                }
            }
        }
    }

    // Assert a stop condition
    regs_->con.set = I2C_CON_PEN(1);

    // Wait for stop condition to be released
    while( (regs_->con.reg & I2C_CON_PEN(1)) ){};

    // Disable the I2C
    // disable();
    return byte_count;
}

/*******************************************************************************
 * con_setup
 *
 * Description: Sets up the con register for the i2c peripherial
 *
 * Inputs: none
 *
 * Revision: Initial Creation 3/02/2014 - Mitchell S. Tilson
 *           Update to c++ 3/18/2014 - Mitchell S. Tilson
 *
 ******************************************************************************/
void I2C_Class::con_setup( void )
{
    regs_->con.reg = 0;
    regs_->con.clr = I2C_CON_PCIE(1);	// stop detection interrupts
    regs_->con.clr = I2C_CON_SCIE(1);	// start detection interrupts disabled
    regs_->con.clr = I2C_CON_BOEN(1);	// only recieve data if i2conv is clear
    regs_->con.clr = I2C_CON_SDAHT(1);	// 100 ns hold time
    regs_->con.clr = I2C_CON_SBCDE(1);	// slave bus collision interrupts are disabled
    regs_->con.clr = I2C_CON_AHEN(1);	// Address hold disabled
    regs_->con.clr = I2C_CON_DHEN(1);	// Data hold disabled
    regs_->con.clr = I2C_CON_ON(1);	// This bit should not be modified here
    //regs_->con.set = I2C_CON_SIDL(1);	// Disable operation while the device is in idle mode
    regs_->con.clr = I2C_CON_SIDL(1);
    regs_->con.set = I2C_CON_SCLREL(1);	// Leave as is because I don't completey understand what this does for now
    regs_->con.clr = I2C_CON_STRICT(1);	// strict addressing not enforced
    regs_->con.clr = I2C_CON_A10M(1);	// 7 bit addressing
    regs_->con.clr = I2C_CON_DISSLW(1);	// slew rate control enabled (going to use 400kHz transmissions)
    regs_->con.clr = I2C_CON_SMEN(1);	// No SMB bus compatability for now
    regs_->con.clr = I2C_CON_GCEN(1);	// Don't respond to general call address
    regs_->con.clr = I2C_CON_STREN(1);	// Clock stretching disabled (slave mode only option)
    regs_->con.clr = I2C_CON_ACKDT(1);	// Normal ACK sent
    regs_->con.clr = I2C_CON_ACKEN(1);	// May need to change this later (ACK disabled for now as it is the default)
    regs_->con.clr = I2C_CON_RCEN(1);	// Reciever disabled for now.  Will enable it during operation
    regs_->con.clr = I2C_CON_PEN(1);	// Stop enable bit not set until we are transmitting
    regs_->con.clr = I2C_CON_RSEN(1);	// Restart enable bit not set until we are transmitting
    regs_->con.clr = I2C_CON_SEN(1);	// Start enable bit not set until we are transmitting
}

/*******************************************************************************
 * stat_setup
 *
 * Description: Sets up the STAT register
 *
 * Inputs: none
 *
 * Revision: Initial Creation 3/02/2014 - Mitchell S. Tilson
 *           Update to c++ 3/18/2014 - Mitchell S. Tilson
 *
 * Note: Right now, this funciton makes no changes to the stat register
 *
 ******************************************************************************/
void I2C_Class::stat_setup( void )
{

    //ACKSTAT   // read only bit showing wether an ACK was recieved or no
    //TRSTAT	// read only bit indicating if a transmission is in progress
    //ACKTIM	// read only bit indicating bus is in an ACK sequence (slave mode only)
    //BCL	// r/w bit indicating a collision on the bus has occured (probably needs to be cleared after a collision)
    //GCSTAT	// indicates a general call address was recieved (this driver doesn't support general call addresses)
    //ADD10	// read only 10 bit address match (this driver doesn't support 10 bit addressing)
    //IWCOL	// r/w bit that indicates a collision occured trying to write the TRN register (probably needs to be cleared)
    //I2COV	// r/w bit indicating receive buffer overflow (must be cleared in software)
    //D/A	// In slave mode, last byte was data or last byte was an address
    //P		// If 1, a stop bit was detected last. 0, a stop bit was not detected last
    //S		// Indicates a start bit was detected last if 1.  Opposite if 0.
    //R/W	// Read or write.  Last transaction was a read if 1, write otherwise.
    //RBF	// Receive buffer full if 1.
    //TBF	// Transmit buffer full if 1.
}

/*******************************************************************************
 * addr_setup
 *
 * Description: Sets the slave address.  It is 0 for now because its not
 *              planned to use the i2c in slave mode.
 *
 * Inputs: none
 *
 * Revision: Initial Creation 3/02/2014 - Mitchell S. Tilson
 *           Update to c++ 3/18/2014 - Mitchell S. Tilson
 *
 ******************************************************************************/
void I2C_Class::addr_setup( void )
{
    regs_->add.reg = 0x00;
}

/*******************************************************************************
 * add_msk_setup
 *
 * Description: Sets up a mask of don't cares that allowed for addresses that we can receive from.  This will
 *		always be a zero as we only service one address.
 *
 * Inputs: none
 *
 * Revision: Initial Creation 3/02/2014 - Mitchell S. Tilson
 *           Update to c++ 3/18/2014 - Mitchell S. Tilson
 *
 ******************************************************************************/
void I2C_Class::addr_msk_setup( void )
{
    regs_->add_msk.reg = 0x00;
}

/*******************************************************************************
 * set_baud
 *
 * Description: Sets the buad rate.  Can either be 400kHz or 100kHz.
 *
 * Inputs: none
 *
 * Revision: Initial Creation 3/02/2014 - Mitchell S. Tilson
 *           Update to c++ 3/18/2014 - Mitchell S. Tilson
 *
 ******************************************************************************/
void I2C_Class::set_baud( void )
{
    float brg;
    if( baud_ == 400000 || baud_ == 100000 )
    {
        brg = (((1/(2*baud_))-0.000000104)*80000000.0)-2 + 0.5;
    }
    else
    {
        brg = 0;
    }
    regs_->brg.reg = (uint32_t)brg;
}

/*******************************************************************************
 * send_dev_address
 *
 * Description: Sends the address to the peripherial
 *
 * Inputs: I2C_MSG*    - the message with the address and the direction
 *
 *         I2C_start_type - start or restart
 *
 * Revision: Initial Creation 3/15/2014 - Mitchell S. Tilson
 *           Update to c++ 3/18/2014 - Mitchell S. Tilson
 *
 ******************************************************************************/
BOOL I2C_Class::send_dev_address( I2C_MSG* msg, I2C_start_type start_type )
{

    // address byte
    uint8_t addr_byte = 0;

    // Return if peripherial is not enabled
    if( FALSE == enabled_ ) {
        return FALSE;
    }

    if( start_type == kSTART ) {

        // Assert the start condition
        regs_->con.set = I2C_CON_SEN(1);

        // Wait for start condition to be released
        while( (regs_->con.reg & I2C_CON_SEN(1)) ){};

        // Always a write bit when writting the address the first time
        addr_byte = (msg->dev_addr << 1) | kI2C_WRITE;

    } else if( start_type == kRESTART ) {

        // Assert the restart condition
        regs_->con.set = I2C_CON_RSEN(1);

        // Wait for start condition to be released
        while( (regs_->con.reg & I2C_CON_RSEN(1)) ){};

        // Send the direction based on the next operation
        addr_byte = (msg->dev_addr << 1) | msg->dir;
    }

    // Send the address and return if we got an ACK
    return send_byte( addr_byte );

}

/*******************************************************************************
 * send_reg_address
 *
 * Description: Sends the address to the peripherial
 *
 * Inputs: I2C_MSG* - the message with register address to write or read
 *
 * Revision: Initial Creation 3/15/2014 - Mitchell S. Tilson
 *           Update to c++ 3/18/2014 - Mitchell S. Tilson
 *
 ******************************************************************************/
BOOL I2C_Class::send_reg_address( I2C_MSG* msg )
{
    // Return if peripherial is not enabled
    if( FALSE == enabled_ ) {
        return FALSE;
    }

    //Send the register address
    return send_byte( (uint8_t)msg->reg_addr );

}

/*******************************************************************************
 * send_byte
 *
 * Description: Sends a byte (any byte) to the bus
 *
 * Inputs: uint8_t byte - the byte to send
 *
 * Revision: Initial Creation 3/15/2014 - Mitchell S. Tilson
 *           Update to c++ 3/18/2014 - Mitchell S. Tilson
 *
 ******************************************************************************/
BOOL I2C_Class::send_byte( uint8_t byte )
{
    // Return if peripherial is not enabled
    if( FALSE == enabled_ ) {
        return FALSE;
    }

    // Write the byte to the transmitt register
    regs_->trn.reg = byte;

    // Wait for transmit to be empty
    while( I2C_STAT_TRSTAT(1) == (regs_->stat.reg & I2C_STAT_TRSTAT(1)) ){};

    // Return if nak or no ack
    if( I2C_STAT_ACKSTAT(1) == (regs_->stat.reg & I2C_STAT_ACKSTAT(1)) ) {
        return FALSE;
    } else {
        return TRUE;
    }
}

/*******************************************************************************
 * send_word
 *
 * Description: Sends two bytes to the bus
 *
 * Inputs: uint16_t word - the word to send
 *
 * Revision: Initial Creation 3/15/2014 - Mitchell S. Tilson
 *           Update to c++ 3/18/2014 - Mitchell S. Tilson
 *
 ******************************************************************************/
BOOL I2C_Class::send_word( uint16_t word )
{
    if( FALSE == send_byte( (uint8_t)(word >> 8)) ) {
        return FALSE;
    } else if( FALSE == send_byte( (uint8_t)(word)) ) {
        return FALSE;
    }
    return TRUE;
}

/*******************************************************************************
 * read_byte
 *
 * Description: Reads a byte from the I2C bus
 *
 * Inputs: uint8_t* - location to store the byte
 *
 * Revision: Initial Creation 3/15/2014 - Mitchell S. Tilson
 *           Update to c++ 3/18/2014 - Mitchell S. Tilson
 *
 ******************************************************************************/
BOOL I2C_Class::read_byte( uint8_t* byte )
{
    // time out (this is very simple version for now
    uint32_t time_out = 3000;

    // Return if peripherial is not enabled
    if( FALSE == enabled_ ) {
        return FALSE;
    }

    // Set the master receive bit
    regs_->con.set = I2C_CON_RCEN(1);

    // Wait for the byte to come in
    while( (regs_->con.reg & I2C_CON_RCEN(1)) && 0 != time_out ) {
        time_out--;
    }

    // If we timed out return false
    if( 0 == time_out ) {
        *byte = 0x00;
        regs_->con.set = I2C_CON_ACKDT(1);
        regs_->con.set = I2C_CON_ACKEN(1);
        return FALSE;

    // Get the byte
    } else {
        *byte = regs_->rcv.reg;
        return TRUE;
    }

}

/*******************************************************************************
 * wait_idle
 *
 * Description: Waits for the I2C bus to be idle.
 *
 * Inputs: none
 *
 * Returns: none
 *
 * Revision: Initial Creation 4/9/2014 - Mitchell S. Tilson
 *
 ******************************************************************************/
void I2C_Class::wait_idle( void )
{
    //Wait for Acken, Rcen, Pen, Rsen and Sen to clear
    while((regs_->con.reg&0x001F)!=0);
}
