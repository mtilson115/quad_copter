/*******************************************************************************
 * File:    app_accel_gyro.h
 * Author:  Mitchell S. Tilson
 * Created: 03/26/2014
 *
 * Description: This file provides the application interface to the gyro accelerometer
 *							board.
 *
 ******************************************************************************/

#pragma once
#ifndef APP_ACCELL_GYRO_H
#define	APP_ACCELL_GYRO_H

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "type_defs.h"

/*******************************************************************************
 * Class definition
 ******************************************************************************/
class AppAccelGyroClass {

/*******************************************************************************
 * Structure type definitions
 ******************************************************************************/
public:

	typedef struct {
		int16_t		ax;
		int16_t		ay;
		int16_t		az;
	} accel_data_type;

	typedef struct {
		int16_t		gx;
		int16_t		gy;
		int16_t		gz;
	} gyro_data_type;

	typedef struct {
		int16_t		ax;
		int16_t		ay;
		int16_t		az;
		int16_t		gx;
		int16_t		gy;
		int16_t		gz;
	} motion6_data_type;

/*******************************************************************************
 * Public Functions
 ******************************************************************************/
public:

	AppAccelGyroClass(){};

	void Init( void );

	void Start( void );

	void Stop( void );

	void Calibrate( void );

	void PrintOffsets( void );

	void GetMotion6Data( motion6_data_type* data  );

	void PrintMotion6Data( void );

/*******************************************************************************
 * Private Functions
 ******************************************************************************/
private:

	void readOffsets( motion6_data_type* data );

	void setOffsets( motion6_data_type* data );

/*******************************************************************************
 * Private Data
 ******************************************************************************/
private:

    static const uint32_t CAL_SUM_CNT;

    static const uint16_t ONE_G;

    float accel_full_range_divisor; // In G (gravity)

    float gyro_full_range_divisor;  // In degree/sec
};

extern AppAccelGyroClass AclGyro;

#endif
