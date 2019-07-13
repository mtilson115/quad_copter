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
#include "driver_flash.h"
#include "bsp_utils.h"
#include "comms_xbee.h"
#include <string.h>
#include <math.h>
#include <sys/kmem.h>

/*******************************************************************************
 * Private data
 ******************************************************************************/
const uint32_t AppAccelGyroClass::CAL_SUM_CNT = 400;
const uint16_t AppAccelGyroClass::ONE_G = 16384;
#define M_PI (3.14159)
#define ACCEL_GYRO_CAL_HDR (0xA5A5A5A5)

/*******************************************************************************
 * Public data
 ******************************************************************************/
unsigned char accel_gyro_cal[1024] __attribute__((section (".cal_space"),space(prog))) = {1};

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

	// Intialize the chip
	if( false == AccelGyro.Init() ) {
		return;
	}

    /*
     * Get the full range divisor for calculations, calibrate the zero values, and enable
     * interrupts.
     */
    AccelGyro.GetFullRangeDivisor(&accel_full_range_divisor,&gyro_full_range_divisor);
}

/*******************************************************************************
 * Start
 *
 * Description: Enables the interrupts from the accel gyro
 *
 * Inputs:      None
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 06/17/2019 - Mitchell S. Tilson
 *
 ******************************************************************************/
void AppAccelGyroClass::Start( void )
{
    AccelGyro.IntEn();
}

/*******************************************************************************
 * Start
 *
 * Description: Enables the interrupts from the accel gyro
 *
 * Inputs:      None
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 06/17/2019 - Mitchell S. Tilson
 *
 ******************************************************************************/
void AppAccelGyroClass::Stop( void )
{
    AccelGyro.IntDisable();
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
    OS_ERR err;
    motion6_data_type offsets;
    memset(&offsets,0x00,sizeof(offsets));
    setOffsets(&offsets);
    int32_t ax_s = 0;
    int32_t ay_s = 0;
    int32_t az_s = 0;
    int32_t gx_s = 0;
    int32_t gy_s = 0;
    int32_t gz_s = 0;
    motion6_data_type data;
    for( uint32_t cal_idx = 0; cal_idx < CAL_SUM_CNT; cal_idx++ )
    {
        GetMotion6Data( &data );
        ax_s += data.ax;
        ay_s += data.ay;
        az_s += data.az;
        gx_s += data.gx;
        gy_s += data.gy;
        gz_s += data.gz;
        OSTimeDlyHMSM(0u, 0u, 0u, 20u,OS_OPT_TIME_HMSM_STRICT,&err);
    }
    offsets.ax = int16_t(((float)ax_s)/((float)CAL_SUM_CNT));
    offsets.ay = int16_t(((float)ay_s)/((float)CAL_SUM_CNT));
    offsets.az = int16_t(((float)az_s)/((float)CAL_SUM_CNT)) - ONE_G;
    offsets.gx = int16_t(((float)gx_s)/((float)CAL_SUM_CNT));
    offsets.gy = int16_t(((float)gy_s)/((float)CAL_SUM_CNT));
    offsets.gz = int16_t(((float)gz_s)/((float)CAL_SUM_CNT));

    setOffsets(&offsets);

    // Build the message
    uint8_t msg_hdr = COMMS_CALIBRATION;
    uint8_t data_buff[sizeof(msg_hdr)+6*sizeof(int16_t)] = {0};
    data_buff[0] = msg_hdr;
    memcpy(&data_buff[sizeof(msg_hdr)],&offsets.ax,sizeof(int16_t));
    memcpy(&data_buff[sizeof(int16_t)+sizeof(msg_hdr)],&offsets.ay,sizeof(int16_t));
    memcpy(&data_buff[2*sizeof(int16_t)+sizeof(msg_hdr)],&offsets.az,sizeof(int16_t));
    memcpy(&data_buff[3*sizeof(int16_t)+sizeof(msg_hdr)],&offsets.gx,sizeof(int16_t));
    memcpy(&data_buff[4*sizeof(int16_t)+sizeof(msg_hdr)],&offsets.gy,sizeof(int16_t));
    memcpy(&data_buff[5*sizeof(int16_t)+sizeof(msg_hdr)],&offsets.gz,sizeof(int16_t));

    // Send the message
    comms_xbee_msg_t msg;
    msg.data = data_buff;
    msg.len = sizeof(data_buff);
    COMMS_xbee_send(msg);

    PrintOffsets();
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
    // Get the offsets
    motion6_data_type offsets;
    readOffsets( &offsets );

    // Build the message
    uint8_t msg_hdr = COMMS_CALIBRATION;
    uint8_t data_buff[sizeof(msg_hdr)+6*sizeof(int16_t)] = {0};
    data_buff[0] = msg_hdr;
    memcpy(&data_buff[sizeof(msg_hdr)],&offsets.ax,sizeof(int16_t));
    memcpy(&data_buff[sizeof(int16_t)+sizeof(msg_hdr)],&offsets.ay,sizeof(int16_t));
    memcpy(&data_buff[2*sizeof(int16_t)+sizeof(msg_hdr)],&offsets.az,sizeof(int16_t));
    memcpy(&data_buff[3*sizeof(int16_t)+sizeof(msg_hdr)],&offsets.gx,sizeof(int16_t));
    memcpy(&data_buff[4*sizeof(int16_t)+sizeof(msg_hdr)],&offsets.gy,sizeof(int16_t));
    memcpy(&data_buff[5*sizeof(int16_t)+sizeof(msg_hdr)],&offsets.gz,sizeof(int16_t));

    // Send the message
    comms_xbee_msg_t msg;
    msg.data = data_buff;
    msg.len = sizeof(data_buff);
    COMMS_xbee_send(msg);
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
	AccelGyro.GetMotion6( &data->ax, &data->ay, &data->az, &data->gx, &data->gy, &data->gz );
    motion6_data_type offsets;
    readOffsets( &offsets );
    data->ax -= offsets.ax;
    data->ay -= offsets.ay;
    data->az -= offsets.az;
    data->gx -= offsets.gx;
    data->gy -= offsets.gy;
    data->gz -= offsets.gz;
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

    // Calculate the pitch and roll
    float accel_pitch = atan2(data.ax,data.az);
    float accel_roll = atan2(data.ay,data.az);

    // Convert to degrees
    accel_pitch = accel_pitch*180.0/M_PI;
    accel_roll = accel_roll*180.0/M_PI;

    // Allocate buffer and copy in the data
    uint8_t data_buff[2*sizeof(float)] = {0};
    memcpy(data_buff,&accel_pitch,sizeof(accel_pitch));
    memcpy(&data_buff[sizeof(accel_pitch)],&accel_roll,sizeof(accel_roll));

    // Send the message
    comms_xbee_msg_t msg;
    msg.data = data_buff;
    msg.len = sizeof(data_buff);
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
    uint32_t hdr = 0;
    memcpy(&hdr,&accel_gyro_cal[0],sizeof(hdr));
    if( hdr == ACCEL_GYRO_CAL_HDR )
    {
        memcpy(&data->ax,&accel_gyro_cal[4],sizeof(data->ax));
        memcpy(&data->ay,&accel_gyro_cal[8],sizeof(data->ay));
        memcpy(&data->az,&accel_gyro_cal[12],sizeof(data->az));
        memcpy(&data->gx,&accel_gyro_cal[16],sizeof(data->gx));
        memcpy(&data->gy,&accel_gyro_cal[20],sizeof(data->gy));
        memcpy(&data->gz,&accel_gyro_cal[24],sizeof(data->gz));
    }
    else
    {
        memset(data,0x00,sizeof(motion6_data_type));
    }
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
    driver_flash_erase_page(KVA_TO_PA(accel_gyro_cal));
    driver_flash_write_word(KVA_TO_PA(&accel_gyro_cal[0]),ACCEL_GYRO_CAL_HDR);
    driver_flash_write_word(KVA_TO_PA(&accel_gyro_cal[4]),data->ax);
    driver_flash_write_word(KVA_TO_PA(&accel_gyro_cal[8]),data->ay);
    driver_flash_write_word(KVA_TO_PA(&accel_gyro_cal[12]),data->az);
    driver_flash_write_word(KVA_TO_PA(&accel_gyro_cal[16]),data->gx);
    driver_flash_write_word(KVA_TO_PA(&accel_gyro_cal[20]),data->gy);
    driver_flash_write_word(KVA_TO_PA(&accel_gyro_cal[24]),data->gz);
}

