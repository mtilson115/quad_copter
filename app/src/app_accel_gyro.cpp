/*******************************************************************************
 * File:    app_accel_gyro.cpp
 * Author:  Mitchell S. Tilson
 * Created: 03/26/2014
 *
 * Description: This file provies the application level interface to the accelerometer
 *							gyroscope chip.
 ******************************************************************************/

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "app_accel_gyro.h"
#include "type_defs.h"
#include "bsp_accel_gyro.h"
#include "driver_i2c.h"
#include "bsp_utils.h"
#include <string.h>
// Temp testing
#include <stdio.h>
#include "comms_xbee.h"

/*******************************************************************************
 * Public Object declaration
 ******************************************************************************/
AppAccelGyroClass AclGyro;

/*******************************************************************************
 * Public Method Section
 ******************************************************************************/

/*******************************************************************************
 * AppAccelGyroClass
 *
 * Description: Class constructor function
 *
 * Inputs:      None
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 03/26/2014 - Mitchell S. Tilson
 *
 * Constructor not required at this time.
 ******************************************************************************/

/*******************************************************************************
* Init
*
* Description: Initializes the accelerometer gyroscope board
*
* Inputs:      None
*
* Returns:     None
*
* Revision:    Initial Creation 03/26/2014 - Mitchell S. Tilson
*
******************************************************************************/
void AppAccelGyroClass::Init( void )
{
	// Initialize the accelerometer gyroscope ( the address pin is tied low )
	AccelGyro.SetI2CAddress( MPU6050_ADDRESS_AD0_LOW );

    // Setup the microchip interrupts
    AccelGyro.IntEnable();

	// Intialize the chip
	if( false == AccelGyro.Init() ) {
		BSP_Printf("Failed to initialize the gyro\n\r");
		return;
	}

    Calibrate();
    // Creat the main thread
    //
}

/*******************************************************************************
 * Calibrate
 *
 * Description: Calibrates the Chip offsets.
 *							The function works by output of all 6 motion indicators
 *							and then adjusting the offsets correctly until they read 0.
 *							(Z is calibrated to be the ideal value -1g).
 *
 * Inputs:      None
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 03/26/2014 - Mitchell S. Tilson
 *
 ******************************************************************************/
void AppAccelGyroClass::Calibrate( void )
{
	memset(&offsets,0x00,sizeof(offsets));
    offsets.ax = 300;
    offsets.gx = -300;
    offsets.gy = -100;
    offsets.gz = 200;
    // GetMotion6Data( &offsets );
}

/*******************************************************************************
 * PrintOffsets
 *
 * Description: Prints the offsets for the 6 motion data
 *
 * Inputs:      None
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 03/26/2014 - Mitchell S. Tilson
 *
 ******************************************************************************/
void AppAccelGyroClass::PrintOffsets( void )
{
	BSP_Printf("Offset values (not the actual readings)\n\r");
	BSP_Printf("AX %d\n\rAY %d\n\rAZ %d\n\r",offsets.ax,offsets.ay,offsets.az);
	BSP_Printf("GX %d\n\rGY %d\n\rGZ %d\n\r",offsets.gx,offsets.gy,offsets.gz);
}

/*******************************************************************************
 * GetMotion6Data
 *
 * Description: Gets the data from the sensor
 *
 * Inputs:      motion6_data_type* data - Where to store the data.
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 03/26/2014 - Mitchell S. Tilson
 *
 ******************************************************************************/
void AppAccelGyroClass::GetMotion6Data( motion6_data_type* data )
{
	memset(data,0x00,sizeof(data));
	AccelGyro.GetMotion6( &data->ax, &data->ay, &data->az, &data->gx, &data->gy, &data->gz );
}

/*******************************************************************************
 * PrintMotion6Data
 *
 * Description: Prints the data from the sensor
 *
 * Inputs:      None
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 03/26/2014 - Mitchell S. Tilson
 *
 ******************************************************************************/
void AppAccelGyroClass::PrintMotion6Data( void )
{
    // Read the status
    motion6_data_type data;
	memset(&data,0x00,sizeof(data));

    GetMotion6Data( &data );
    data.ax -= offsets.ax;
    data.ay -= offsets.ay;
    data.az -= offsets.az;
    data.gx -= offsets.gx;
    data.gy -= offsets.gy;
    data.gz -= offsets.gz;
    // uint8_t status = AccelGyro.GetIntStatus();
    // BSP_Printf("Status: 0x%X",status);

    // BSP_Printf("%d,%d,%d,%d,%d,%d",data.ax,data.ay,data.az,data.gx,data.gy,data.gz);
    char buffer[100];
    int cnt = snprintf(buffer,sizeof(buffer),"%d,%d,%d,%d,%d,%d",data.ax,data.ay,data.az,data.gx,data.gy,data.gz);
    if( cnt > sizeof(buffer) )
    {
        while(1);
    }
    comms_xbee_msg_t msg;
    msg.data = buffer;
    msg.len = len;
    COMMS_xbee_send(msg);
}

/*******************************************************************************
 * Private Method Section
 ******************************************************************************/

/*******************************************************************************
 * readOffsets
 *
 * Description: Reads the offsets of the sensor
 *
 * Inputs:      motion6_data_type* data - the pointer to the address to read the data into
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 03/26/2014 - Mitchell S. Tilson
 *
 ******************************************************************************/
void AppAccelGyroClass::readOffsets( motion6_data_type* data )
{
	data->ax = AccelGyro.GetXAccelOffset();
	data->ay = AccelGyro.GetYAccelOffset();
	data->az = AccelGyro.GetZAccelOffset();

	data->gx = AccelGyro.GetXGyroOffset();
	data->gy = AccelGyro.GetYGyroOffset();
	data->gz = AccelGyro.GetZGyroOffset();
}

/*******************************************************************************
 * setOffsets
 *
 * Description: Sets the offsets of the sensor
 *
 * Inputs:      motion6_data_type* data - pointer to the data to set the offsets to
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 03/26/2014 - Mitchell S. Tilson
 *
 ******************************************************************************/
void AppAccelGyroClass::setOffsets( motion6_data_type* data )
{
	AccelGyro.SetXAccelOffset( data->ax );
	AccelGyro.SetYAccelOffset( data->ay );
	AccelGyro.SetZAccelOffset( data->az );

	AccelGyro.SetXGyroOffset( data->gx );
	AccelGyro.SetYGyroOffset( data->gy );
	AccelGyro.SetZGyroOffset( data->gz );
}

