
/*******************************************************************************
 * File:    driver_i2c.h
 * Author:  Mitchell S. Tilson
 * Created: 03/01/2014
 *
 * Description: This file provides a driver interface to the i2c periperial.
 ******************************************************************************/
#pragma once
#ifndef DRIVER_I2C_H
#define	DRIVER_I2C_H


/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "type_defs.h"
#include "perif_sfr_map.h"

/*******************************************************************************
 * Class definition
 ******************************************************************************/
class I2C_Class {

/*******************************************************************************
 * Enumerated constants
 ******************************************************************************/
private:

    typedef enum {
        kI2C_WRITE = 0,
        kI2C_READ,
    } I2C_read_type;

/*******************************************************************************
 * Structure type definitions
 ******************************************************************************/
public:

    typedef struct
    {
      volatile REG_SET    con;
      volatile REG_SET    stat;
      volatile REG_SET    add;
      volatile REG_SET    add_msk;
      volatile REG_SET    brg;
      volatile REG_SET    trn;
      volatile REG_SET    rcv;
    } I2C_REGS;

    typedef enum {
      I2C_1 = 0,
      I2C_2 = 1
    } I2C_ID;

private:

    typedef struct {
      uint8_t     dev_addr;
      uint8_t     reg_addr;
      uint8_t     dir;
      uint8_t*    data;
      uint32_t    count;
    } I2C_MSG;

    typedef enum {
      kSTART,
      kRESTART,
    } I2C_start_type;

/*******************************************************************************
 * Public Functions
 ******************************************************************************/
public:

    I2C_Class();

    void Init( I2C_ID id, float baud );

    BOOL ReadRegByte( uint8_t dev_addr,
                      uint8_t reg_addr,
                      uint8_t* reg_byte );

    BOOL ReadRegBytes( 	uint8_t dev_addr,
                        uint8_t reg_addr,
                        uint8_t length,
                        uint8_t* buffer );

    BOOL ReadRegWord( uint8_t dev_addr,
                      uint8_t reg_addr,
                      uint16_t* reg_word );

    BOOL ReadRegWords(	uint8_t dev_addr,
                        uint8_t reg_addr,
                        uint8_t length,
			uint16_t* buffer );

    BOOL ReadRegBitB( uint8_t dev_addr,
                      uint8_t reg_addr,
                      uint8_t bit_num,
                      uint8_t* reg_bit );

    BOOL ReadRegBitW( uint8_t dev_addr,
                      uint8_t reg_addr,
                      uint8_t bit_num,
                      uint16_t* reg_bit );

    BOOL ReadRegBitsB( uint8_t dev_addr,
                       uint8_t reg_addr,
                       uint8_t bit_start,
                       uint8_t length,
                       uint8_t* reg_bits );

    BOOL ReadRegBitsW( uint8_t dev_addr,
                       uint8_t reg_addr,
                       uint8_t bit_start,
                       uint8_t length,
                       uint16_t* reg_bits );

    BOOL WriteRegByte( uint8_t dev_addr,
                       uint8_t reg_addr,
                       uint8_t reg_byte );

    BOOL WriteRegBytes(	uint8_t dev_addr,
			uint8_t reg_addr,
			uint8_t length,
			uint8_t* buffer );

    BOOL WriteRegWord( uint8_t dev_addr,
                       uint8_t reg_addr,
                       uint16_t reg_word );

    BOOL WriteRegWords(	uint8_t dev_addr,
			uint8_t reg_addr,
			uint8_t length,
			uint16_t* buffer );

    BOOL WriteRegBitB( uint8_t dev_addr,
                       uint8_t reg_addr,
                       uint8_t bit_num,
                       uint8_t reg_bit );

    BOOL WriteRegBitW( uint8_t dev_addr,
                       uint8_t reg_addr,
                       uint8_t bit_num,
                       uint8_t reg_bit );

    BOOL WriteRegBitsB( uint8_t dev_addr,
                        uint8_t reg_addr,
                        uint8_t bit_start,
                        uint8_t length,
                        uint8_t reg_bits );

    BOOL WriteRegBitsW( uint8_t dev_addr,
                        uint8_t reg_addr,
                        uint8_t bit_start,
                        uint8_t length,
                        uint16_t reg_bits );

/*******************************************************************************
 * Private Functions
 ******************************************************************************/
private:

    // Setup
    void con_setup( void );
    void stat_setup( void );
    void addr_setup( void );
    void addr_msk_setup( void );
    void set_baud( void );

    // Utilities
    void    enable( void );
    void    disable( void );
    int32_t communicate( I2C_MSG* msg );
    BOOL    send_dev_address( I2C_MSG* msg, I2C_start_type start_type );
    BOOL    send_reg_address( I2C_MSG* msg );
    BOOL    send_byte( uint8_t byte );
    BOOL    send_word( uint16_t word );
    BOOL    read_byte( uint8_t* byte );
    void    wait_idle( void );

/*******************************************************************************
 * Private Data
 ******************************************************************************/
private:

    // Prevents functionality if not initialized
    BOOL    initialized_;

    // Prevents functionality if the physical interface is not enabled
    BOOL    enabled_;

    // Baud rate (can be 100kHz or 400kHz)
    float   baud_;

    // Registers for control
    I2C_REGS * regs_;

};

extern I2C_Class I2C1;
extern I2C_Class I2C2;

#endif	/* DRIVER_I2C_H*/

