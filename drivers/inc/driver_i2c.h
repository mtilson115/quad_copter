
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
#include <cstdbool>
#include <stdint.h>
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

    bool ReadRegByte( uint8_t dev_addr,
                      uint8_t reg_addr,
                      uint8_t* reg_byte );

    bool ReadRegBytes( 	uint8_t dev_addr,
                        uint8_t reg_addr,
                        uint8_t length,
                        uint8_t* buffer );

    bool ReadRegWord( uint8_t dev_addr,
                      uint8_t reg_addr,
                      uint16_t* reg_word );

    bool ReadRegWords(	uint8_t dev_addr,
                        uint8_t reg_addr,
                        uint8_t length,
			uint16_t* buffer );

    bool ReadRegBitB( uint8_t dev_addr,
                      uint8_t reg_addr,
                      uint8_t bit_num,
                      uint8_t* reg_bit );

    bool ReadRegBitW( uint8_t dev_addr,
                      uint8_t reg_addr,
                      uint8_t bit_num,
                      uint16_t* reg_bit );

    bool ReadRegBitsB( uint8_t dev_addr,
                       uint8_t reg_addr,
                       uint8_t bit_start,
                       uint8_t length,
                       uint8_t* reg_bits );

    bool ReadRegBitsW( uint8_t dev_addr,
                       uint8_t reg_addr,
                       uint8_t bit_start,
                       uint8_t length,
                       uint16_t* reg_bits );

    bool WriteRegByte( uint8_t dev_addr,
                       uint8_t reg_addr,
                       uint8_t reg_byte );

    bool WriteRegBytes(	uint8_t dev_addr,
			uint8_t reg_addr,
			uint8_t length,
			uint8_t* buffer );

    bool WriteRegWord( uint8_t dev_addr,
                       uint8_t reg_addr,
                       uint16_t reg_word );

    bool WriteRegWords(	uint8_t dev_addr,
			uint8_t reg_addr,
			uint8_t length,
			uint16_t* buffer );

    bool WriteRegBitB( uint8_t dev_addr,
                       uint8_t reg_addr,
                       uint8_t bit_num,
                       uint8_t reg_bit );

    bool WriteRegBitW( uint8_t dev_addr,
                       uint8_t reg_addr,
                       uint8_t bit_num,
                       uint8_t reg_bit );

    bool WriteRegBitsB( uint8_t dev_addr,
                        uint8_t reg_addr,
                        uint8_t bit_start,
                        uint8_t length,
                        uint8_t reg_bits );

    bool WriteRegBitsW( uint8_t dev_addr,
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
    bool    send_dev_address( I2C_MSG* msg, I2C_start_type start_type );
    bool    send_reg_address( I2C_MSG* msg );
    bool    send_byte( uint8_t byte );
    bool    send_word( uint16_t word );
    bool    read_byte( uint8_t* byte );
    void    wait_idle( void );

/*******************************************************************************
 * Private Data
 ******************************************************************************/
private:

    // Prevents functionality if not initialized
    bool    initialized_;

    // Prevents functionality if the physical interface is not enabled
    bool    enabled_;

    // Baud rate (can be 100kHz or 400kHz)
    float   baud_;

    // Registers for control
    I2C_REGS * regs_;

};

extern I2C_Class I2C1;
extern I2C_Class I2C2;

#endif	/* DRIVER_I2C_H*/

