/*******************************************************************************
 * File:    alg_stabilizer.c
 * Author:  Mitchell S. Tilson
 * Created: 02/12/2019
 *
 * Description: This file takes in accel data, computes roll and pitch, and
 *              outputs PWM values for the motor to keep the quad copter level.
 *
 ******************************************************************************/

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "alg_stabilizer.h"
#include "app_accel_gyro.h"
#include "comms_xbee.h"
#include "bsp_accel_gyro_int.h"
#include "bsp_utils.h"
#include "bsp_motor.h"
#include "bsp_accel_gyro.h"
#include <math.h>
#include <string.h>
#include <assert.h>
#include <p32xxxx.h>
#include <os.h>

/*******************************************************************************
 * Constants
 ******************************************************************************/
#define ALG_STB_TX_Q_DEPTH (0)
#define M_PI (3.14159)

#define SEND_DEBUG 1
#define DO_THROTTLE_CAL 0

/*******************************************************************************
 * Local Data
 ******************************************************************************/

// Task data
CPU_STK alg_stabilizer_stack[ALG_STABILIZER_STK_SIZE];
OS_TCB alg_stabilizer_TCB;
OS_MUTEX alg_stabilizer_throttle_mutex;
OS_MUTEX alg_stabilizer_PID_mutex;
OS_MUTEX alg_stabilizer_calibrate_mutex;
OS_MUTEX alg_stabilizer_pitch_roll_yaw_mutex;

// Motor objects
static BSPMotor motor1;
static BSPMotor motor2;
static BSPMotor motor3;
static BSPMotor motor4;

// Throttle
static float alg_stabilizer_throttle_percent = 0;

// PID constant
static float asP = 0.2;
static float asI = 0.0;
static float asD = 0.0;

// Fitler coefficients
static float A = 0.98;
static float dt = 20.0e-3; // units s

// Calibration
static bool do_calibration = false;

// Pitch and roll
static float pitch_set_point = 0;
static float roll_set_point = 0;
static float yaw_set_point = 0;

/*******************************************************************************
 * Local Function Section
 ******************************************************************************/
static void alg_stabilizer_task( void *p_arg );
static void alg_stabilizer_throttle_msg_cb(uint8_t* data,uint16_t len);
static void alg_stabilizer_PID_msg_cb(uint8_t* data,uint16_t len);
static void alg_stabilizer_calibrate_msg_cb(uint8_t* data,uint16_t len);
static void alg_stabilizer_set_throttle( float throttle );
static float alg_stabilizer_get_throttle( void );
static void alg_stabilizer_set_PID_consts( float P, float I, float D );
static void alg_stabilizer_get_PID_consts( float* P, float* I, float *D );
static void alg_stabilizer( float pitch, float roll, float gravity );
static void alg_stabilizer_compute_pitch_roll( float* pitch, float* roll, float* gravity );
static void alg_stabilizer_set_calibrate( bool en );
static bool alg_stabilizer_get_calibrate( void );
static void alg_stabilizer_pitch_roll_yaw_msg_cb( uint8_t* data, uint16_t len );
static void alg_stabilizer_set_pitch_roll_yaw( float pitch, float roll, float yaw );
static void alg_stabilizer_get_pitch_roll_yaw( float* pitch, float* roll, float* yaw );

/*******************************************************************************
 * Public Function Section
 ******************************************************************************/

/*******************************************************************************
 * alg_stabilizer_init
 *
 * Description: Initializes the accelerometer, creates a task, and registers for
 *              accel interrupts.
 *
 * Inputs:      None
 *
 * Returns:     None (expected to always succeed to throw a fault.
 *
 * Revision:    Initial Creation 02/12/2019 - Mitchell S. Tilson
 *
 ******************************************************************************/
void alg_stabilizer_init( void )
{
    OS_ERR err;

    // Create the thread
    OSTaskCreate((OS_TCB      *)&alg_stabilizer_TCB,
                 (CPU_CHAR    *)"Alg Stabilizer Task",
                 (OS_TASK_PTR  )alg_stabilizer_task,
                 (void        *)0,
                 (OS_PRIO      )ALG_STABILIZER_PRIO,
                 (CPU_STK     *)&alg_stabilizer_stack[0],
                 (CPU_STK_SIZE )ALG_STABILIZER_STK_SIZE/10,
                 (CPU_STK_SIZE )ALG_STABILIZER_STK_SIZE,
                 (OS_MSG_QTY   )ALG_STB_TX_Q_DEPTH,
                 (OS_TICK      )0u,
                 (void        *)0,
                 (OS_OPT       )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR      *)&err);
    assert(err==OS_ERR_NONE);

    OSMutexCreate(&alg_stabilizer_throttle_mutex,(CPU_CHAR*)"Throttle Mutex",&err);
    assert(err==OS_ERR_NONE);
    OSMutexCreate(&alg_stabilizer_PID_mutex,(CPU_CHAR*)"PID Mutex",&err);
    assert(err==OS_ERR_NONE);
    OSMutexCreate(&alg_stabilizer_calibrate_mutex,(CPU_CHAR*)"Cal Mutex",&err);
    assert(err==OS_ERR_NONE);
    OSMutexCreate(&alg_stabilizer_pitch_roll_yaw_mutex,(CPU_CHAR*)"Pitch Roll Mutex",&err);
    assert(err==OS_ERR_NONE);

    // Register the TCB with the accel bsp code
    bsp_accel_gyro_int_register(&alg_stabilizer_TCB);

    /*
     * Throttle callback
     */
    comms_xbee_rx_cb_t throttle_rx_cb = {
        .cb = alg_stabilizer_throttle_msg_cb,
        .msg_id = COMMS_SET_THROTTLE,
    };
    ret_t ret = COMMS_xbee_register_rx_cb(throttle_rx_cb);
    assert(ret==rSUCCESS);

    /*
     * Set PID callback
     */
    comms_xbee_rx_cb_t pi_rx_cb = {
        .cb = alg_stabilizer_PID_msg_cb,
        .msg_id = COMMS_SET_PID,
    };
    ret = COMMS_xbee_register_rx_cb(pi_rx_cb);
    assert(ret==rSUCCESS);

    /*
     * Execute calibration callback
     */
    comms_xbee_rx_cb_t calibrate_cb = {
        .cb = alg_stabilizer_calibrate_msg_cb,
        .msg_id = COMMS_CALIBRATE,
    };
    ret = COMMS_xbee_register_rx_cb(calibrate_cb);
    assert(ret==rSUCCESS);

    /*
     * Set pitch and roll setpoints
     */
    comms_xbee_rx_cb_t pitch_roll_yaw_cb = {
        .cb = alg_stabilizer_pitch_roll_yaw_msg_cb,
        .msg_id = COMMS_PITCH_ROLL,
    };
    ret = COMMS_xbee_register_rx_cb(pitch_roll_yaw_cb);
    assert(ret==rSUCCESS);
}

/*******************************************************************************
 * alg_stabilizer_task
 *
 * Description: Reads accel, computes roll and pitch, and controls the motors
 *
 * Inputs:      None
 *
 * Returns:     None (expected to always succeed to throw a fault.
 *
 * Revision:    Initial Creation 02/12/2019 - Mitchell S. Tilson
 *
 ******************************************************************************/
static void alg_stabilizer_task( void *p_arg )
{
    OS_ERR err;

    // Initialize the timer
    BSPMotor::InitTmr();

    // Initialize all the motors
    motor1.Init(PWM1);
    motor2.Init(PWM2);
    motor3.Init(PWM3);
    motor4.Init(PWM4);

    // Turn on the timer
    BSPMotor::TmrEn();

    // Start the motors
    motor1.Start();
    motor2.Start();
    motor3.Start();
    motor4.Start();

#if DO_THROTTLE_CAL == 1
    // Calibrate the motors
    motor1.SetSpeedPercent(1.0);
    motor2.SetSpeedPercent(1.0);
    motor3.SetSpeedPercent(1.0);
    motor4.SetSpeedPercent(1.0);

    // Delay 6s
    OSTimeDlyHMSM(0u, 0u, 6u, 0u,OS_OPT_TIME_HMSM_STRICT,&err);

    motor1.SetSpeedPercent(0.0);
    motor2.SetSpeedPercent(0.0);
    motor3.SetSpeedPercent(0.0);
    motor4.SetSpeedPercent(0.0);

    while(1);

    // Delay 2.25s
    OSTimeDlyHMSM(0u, 0u, 2u, 250u,OS_OPT_TIME_HMSM_STRICT,&err);
#endif

    // TRISEbits.TRISE7 = 0;

    // Wait on comms
    BSP_PrintfInit();

    // Initialize the AccelGyro
    AclGyro.Init();

    // Start the accel gyro interrupt
    AclGyro.Start();

    // Print the offsets
    AclGyro.PrintOffsets();

    uint32_t ts = 0;
    while (DEF_ON)
    {
        /*
         * Pend on the task semaphore (posted to from the interrupt for accel)
         */
        OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,&ts,&err);

        // PORTEINV = (1<<7);

        /*
         * Check to see if calibration was requested.
         * The throttle should be off.  The motors don't turn
         * on until 35 percent.
         */
        if( alg_stabilizer_get_throttle() < 5.0 )
        {
            if( alg_stabilizer_get_calibrate() )
            {
                AclGyro.Stop();
                AclGyro.Calibrate();
                AclGyro.Start();
                alg_stabilizer_set_calibrate( false );
            }
        }
        else
        {
            if( alg_stabilizer_get_calibrate() )
            {
                alg_stabilizer_set_calibrate( false );
            }
        }

        /*
         * Get the pitch and roll
         * gravity is included for error conditions.
         * If -gravity is detected, somethings wrong in this design as
         * the quad copter is upside down.
         */
        float pitch, roll, gravity;
        alg_stabilizer_compute_pitch_roll(&pitch,&roll,&gravity);

        /*
         * Apply the values
         */
        alg_stabilizer(pitch,roll,gravity);

        // OSTimeDlyHMSM(0u, 0u, 0u, 20u,OS_OPT_TIME_HMSM_STRICT,&err);
    }
}

/*******************************************************************************
 * alg_stabilizer_throttle_msg_cb
 *
 * Description: This function is called when a message is recieved and has the
 *              msg_id = COMMS_SET_THROTTLE (0x01)
 *
 * Inputs:      None
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 03/02/2019 - Mitchell S. Tilson
 *
 ******************************************************************************/
static void alg_stabilizer_throttle_msg_cb(uint8_t* data,uint16_t len)
{
    float throttle_percent = 0;

    if( data[0] == COMMS_SET_THROTTLE )
    {
        len -= 1;
        if( len == sizeof(throttle_percent) )
        {
            memcpy(&throttle_percent,&data[1],len);
            if( throttle_percent >= 0.0 && throttle_percent <= 100.0 )
            {
                alg_stabilizer_set_throttle(throttle_percent);
            }
        }
    }
}

/*******************************************************************************
 * alg_stabilizer_PID_msg_cb
 *
 * Description: This function is called when a message is recieved and has the
 *              msg_id = COMMS_SET_PID (0x02)
 *
 * Inputs:      None
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 03/02/2019 - Mitchell S. Tilson
 *
 ******************************************************************************/
static void alg_stabilizer_PID_msg_cb( uint8_t* data, uint16_t len )
{
    float loc_P;
    float loc_I;
    float loc_D;
    if( data[0] == COMMS_SET_PID )
    {
        len -= 1;
        if( len == sizeof(loc_P)+sizeof(loc_I)+sizeof(loc_D) )
        {
            memcpy(&loc_P,&data[1],sizeof(loc_P));
            memcpy(&loc_I,&data[sizeof(loc_P)+1],sizeof(loc_I));
            memcpy(&loc_D,&data[sizeof(loc_P)+sizeof(loc_I)+1],sizeof(loc_D));
            alg_stabilizer_set_PID_consts(loc_P,loc_I,loc_D);
        }
    }
}

/*******************************************************************************
 * alg_stabilizer_calibrate_msg_cb
 *
 * Description: This function is called when a message is recieved and has the
 *              msg_id = COMMS_CALIBRATE (0x03)
 *
 * Inputs:      None
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 06/17/2019 - Mitchell S. Tilson
 *
 ******************************************************************************/
static void alg_stabilizer_calibrate_msg_cb( uint8_t* data, uint16_t len )
{
    if( data[0] == COMMS_CALIBRATE )
    {
        alg_stabilizer_set_calibrate( true );
    }
}

/*******************************************************************************
 * alg_stabilizer_pitch_roll_yaw_msg_cb
 *
 * Description: This function is called when a message is recieved and has the
 *              msg_id = COMMS_PITCH_ROLL (0x04)
 *
 * Inputs:      None
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 07/07/2019 - Mitchell S. Tilson
 *
 ******************************************************************************/
static void alg_stabilizer_pitch_roll_yaw_msg_cb( uint8_t* data, uint16_t len )
{
    float loc_pitch_set_point;
    float loc_roll_set_point;
    float loc_yaw_set_point;
    if( data[0] == COMMS_PITCH_ROLL )
    {
        memcpy(&loc_pitch_set_point,&data[1],sizeof(loc_pitch_set_point));
        memcpy(&loc_roll_set_point,&data[sizeof(loc_pitch_set_point)+1],sizeof(loc_roll_set_point));
        memcpy(&loc_yaw_set_point,&data[sizeof(loc_pitch_set_point)+sizeof(loc_roll_set_point)+1],sizeof(loc_yaw_set_point));
        alg_stabilizer_set_pitch_roll_yaw(loc_pitch_set_point,loc_roll_set_point,loc_yaw_set_point);
    }
}

/*******************************************************************************
 * alg_stabilizer_set/get_throttle
 *
 * Description: These functions set and get the throttle speed
 *
 * Inputs:      float throttle
 *
 * Returns:     float throttle
 *
 * Revision:    Initial Creation 03/03/2019 - Mitchell S. Tilson
 *
 * Notes:       This value is set from a different thread so its safer to use
 *              a mutex given it is a float.
 *
 ******************************************************************************/
static void alg_stabilizer_set_throttle( float throttle )
{
    OS_ERR err;
    CPU_TS ts = 0;
    OSMutexPend(&alg_stabilizer_throttle_mutex,0,OS_OPT_PEND_BLOCKING,&ts,&err);
    alg_stabilizer_throttle_percent = throttle;
    OSMutexPost(&alg_stabilizer_throttle_mutex,OS_OPT_POST_NONE,&err);
}
static float alg_stabilizer_get_throttle( void )
{
    float throttle;
    OS_ERR err;
    CPU_TS ts = 0;
    OSMutexPend(&alg_stabilizer_throttle_mutex,0,OS_OPT_PEND_BLOCKING,&ts,&err);
    throttle = alg_stabilizer_throttle_percent;
    OSMutexPost(&alg_stabilizer_throttle_mutex,OS_OPT_POST_NONE,&err);
    return throttle;
}

/*******************************************************************************
 * alg_stabilizer_set/get_PID_consts
 *
 * Description: These functions set and get the PID controller constants
 *
 * Inputs:      float P
 *              float I
 *              float D
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 04/25/2019 - Mitchell S. Tilson
 *
 * Notes:       These values are set from a different thread so its safer to use
 *              a mutex given it is a float.
 *
 ******************************************************************************/
static void alg_stabilizer_set_PID_consts( float P, float I, float D )
{
    OS_ERR err;
    CPU_TS ts = 0;
    OSMutexPend(&alg_stabilizer_PID_mutex,0,OS_OPT_PEND_BLOCKING,&ts,&err);
    asP = P;
    asI = I;
    asD = D;
    OSMutexPost(&alg_stabilizer_PID_mutex,OS_OPT_POST_NONE,&err);
}
static void alg_stabilizer_get_PID_consts( float* P, float* I, float* D )
{
    float throttle;
    OS_ERR err;
    CPU_TS ts = 0;
    OSMutexPend(&alg_stabilizer_PID_mutex,0,OS_OPT_PEND_BLOCKING,&ts,&err);
    *P = asP;
    *I = asI;
    *D = asD;
    OSMutexPost(&alg_stabilizer_PID_mutex,OS_OPT_POST_NONE,&err);
}

/*******************************************************************************
 * alg_stabilizer_set_calibrate/alg_stabilizer_get_calibrate
 *
 * Description: This function sets a variable indicating calibration should be done.
 *
 * Inputs:      None
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 06/17/2019 - Mitchell S. Tilson
 *
 * Notes:
 *
 ******************************************************************************/
static void alg_stabilizer_set_calibrate( bool en )
{
    OS_ERR err;
    CPU_TS ts = 0;
    OSMutexPend(&alg_stabilizer_calibrate_mutex,0,OS_OPT_PEND_BLOCKING,&ts,&err);
    do_calibration = en;
    OSMutexPost(&alg_stabilizer_calibrate_mutex,OS_OPT_POST_NONE,&err);
}
static bool alg_stabilizer_get_calibrate( void )
{
    OS_ERR err;
    CPU_TS ts = 0;
    bool ret = false;
    OSMutexPend(&alg_stabilizer_calibrate_mutex,0,OS_OPT_PEND_BLOCKING,&ts,&err);
    ret = do_calibration;
    OSMutexPost(&alg_stabilizer_calibrate_mutex,OS_OPT_POST_NONE,&err);
    return ret;
}

/*******************************************************************************
 * alg_stabilizer_set_pitch_roll_yaw/alg_stabilizer_get_pitch_roll_yaw
 *
 * Description: This function sets sets the pitch and roll set points.
 *
 * Inputs:      float pitch - the pitch the controller will strive for
 *              float roll - the roll the controller will strive for
 *              float yaw - the yaw the controller will strive for (not supported)        
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 06/17/2019 - Mitchell S. Tilson
 *
 * Notes:
 *
 ******************************************************************************/
static void alg_stabilizer_set_pitch_roll_yaw( float pitch, float roll, float yaw )
{
    OS_ERR err;
    CPU_TS ts = 0;
    OSMutexPend(&alg_stabilizer_pitch_roll_yaw_mutex,0,OS_OPT_PEND_BLOCKING,&ts,&err);
    pitch_set_point = pitch;
    roll_set_point = roll;
    yaw_set_point = yaw;
    OSMutexPost(&alg_stabilizer_pitch_roll_yaw_mutex,OS_OPT_POST_NONE,&err);
}
static void alg_stabilizer_get_pitch_roll_yaw( float* pitch, float* roll, float* yaw )
{
    OS_ERR err;
    CPU_TS ts = 0;
    OSMutexPend(&alg_stabilizer_pitch_roll_yaw_mutex,0,OS_OPT_PEND_BLOCKING,&ts,&err);
    *pitch = pitch_set_point;
    *roll = roll_set_point;
    *yaw = yaw_set_point;
    OSMutexPost(&alg_stabilizer_pitch_roll_yaw_mutex,OS_OPT_POST_NONE,&err);
}

/*******************************************************************************
 * alg_stabilizer
 *
 * Description: This function takes in the pitch and roll and calculates the
 *              appropriate motor throttle values.
 *
 * Inputs:      float - pitch
 *              float - roll
 *              float - gravity (for error detection if negative or upside down)
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 03/03/2019 - Mitchell S. Tilson
 *
 * Notes:       Right now this function assumes the desired pitch and roll is 0.
 *
 ******************************************************************************/
static void alg_stabilizer( float pitch, float roll, float gravity )
{
    // Read the set points
    float loc_pitch_sp = 0;
    float loc_roll_sp = 0;
    float loc_yaw_sp = 0;
    alg_stabilizer_get_pitch_roll_yaw( &loc_pitch_sp, &loc_roll_sp, &loc_yaw_sp );

    // Calculate the errors
    float pitch_error = pitch - loc_pitch_sp;
    float roll_error = roll - loc_roll_sp;

    // Integral and derivative variables
    static float pitch_error_sum = 0;
    static float roll_error_sum = 0;
    static float last_pitch_error = 0;
    static float last_roll_error = 0;
    float pitch_d = pitch_error-last_pitch_error;
    float roll_d = roll_error-last_roll_error;

    // Get the PID constants
    float P,I,D;
    alg_stabilizer_get_PID_consts(&P,&I,&D);

    // Get the desired throttle
    float throttle_percent = alg_stabilizer_get_throttle();

    if( throttle_percent > 40.0 )
    {
        pitch_error_sum += pitch_error;
        roll_error_sum += roll_error;
    }


    // Calculate Motor1's desired throttle
    float motor1_throttle_err = P*(-pitch_error)+P*(-roll_error)+I*(-pitch_error_sum)+I*(-roll_error_sum)+D*(pitch_d)+D*(roll_d);
    motor1_throttle_err = max(motor1_throttle_err,-throttle_percent);
    float motor1_throttle = throttle_percent + motor1_throttle_err;
    motor1_throttle = min(motor1_throttle,100);

    // Calculate Motor2's desired speed
    float motor2_throttle_err = P*(pitch_error)+P*(-roll_error)+I*(pitch_error_sum)+I*(-roll_error_sum)+D*(-pitch_d)+D*(roll_d);
    motor2_throttle_err = max(motor2_throttle_err,-throttle_percent);
    float motor2_throttle = throttle_percent + motor2_throttle_err;
    motor2_throttle = min(motor2_throttle,100);

    // Calculate Motor3's desired speed
    float motor3_throttle_err = P*(pitch_error)+P*(roll_error)+I*(pitch_error_sum)+I*(roll_error_sum)+D*(-pitch_d)+D*(-roll_d);
    motor3_throttle_err = max(motor3_throttle_err,-throttle_percent);
    float motor3_throttle = throttle_percent + motor3_throttle_err;
    motor3_throttle = min(motor3_throttle,100);

    // Calculate Motor4's desired speed
    float motor4_throttle_err = P*(-pitch_error)+P*(roll_error)+I*(-pitch_error_sum)+I*(roll_error_sum)+D*(pitch_d)+D*(-roll_d);
    motor4_throttle_err = max(motor4_throttle_err,-throttle_percent);
    float motor4_throttle = throttle_percent + motor4_throttle_err;
    motor4_throttle = min(motor4_throttle,100);

    /*
     * Catch moving too much or upside down.
     */
    if( pitch > 45 || roll > 45 || pitch < -45 || roll < -45 || gravity < 0 )
    {
        motor1_throttle = 1.0;
        motor2_throttle = 1.0;
        motor3_throttle = 1.0;
        motor4_throttle = 1.0;
    }

    motor1.SetSpeedPercent( motor1_throttle/100.0 );
    motor2.SetSpeedPercent( motor2_throttle/100.0 );
    motor3.SetSpeedPercent( motor3_throttle/100.0 );
    motor4.SetSpeedPercent( motor4_throttle/100.0 );

    last_pitch_error = pitch_error;
    last_roll_error = roll_error;

    /*
     * Debug data
     */
#if SEND_DEBUG == 1 
    uint8_t msg_hdr = COMMS_DBG_HDR_MOTOR_PITCH_ROLL;
    uint8_t data_buff[sizeof(msg_hdr)+10*sizeof(float)] = {0};
    data_buff[0] = msg_hdr;
    uint32_t ts = CPU_TS_Get32();
    memcpy(&data_buff[sizeof(msg_hdr)],&ts,sizeof(float));
    memcpy(&data_buff[sizeof(uint32_t)+sizeof(msg_hdr)],&motor1_throttle,sizeof(float));
    memcpy(&data_buff[2*sizeof(float)+sizeof(msg_hdr)],&motor2_throttle,sizeof(float));
    memcpy(&data_buff[3*sizeof(float)+sizeof(msg_hdr)],&motor3_throttle,sizeof(float));
    memcpy(&data_buff[4*sizeof(float)+sizeof(msg_hdr)],&motor4_throttle,sizeof(float));
    memcpy(&data_buff[5*sizeof(float)+sizeof(msg_hdr)],&pitch,sizeof(float));
    memcpy(&data_buff[6*sizeof(float)+sizeof(msg_hdr)],&roll,sizeof(float));
    memcpy(&data_buff[7*sizeof(float)+sizeof(msg_hdr)],&P,sizeof(float));
    memcpy(&data_buff[8*sizeof(float)+sizeof(msg_hdr)],&I,sizeof(float));
    memcpy(&data_buff[9*sizeof(float)+sizeof(msg_hdr)],&D,sizeof(float));


    // Send the message
    comms_xbee_msg_t msg;
    msg.data = data_buff;
    msg.len = sizeof(data_buff);
    COMMS_xbee_send(msg);
#endif

}

/*******************************************************************************
 * alg_stabilizer_compute_pitch_roll
 *
 * Description: This function computes the pitch and roll using the x,y,z accel
 *              and gyroscope data.
 *
 * Outputs:     float - pitch
 *              float - roll
 *              float - gravity (gravity accel vector)
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 04/25/2019 - Mitchell S. Tilson
 *
 * Notes:       The output is current computed using a complimentary filter
 *              consisting of a low pass filter for the accel and a high pass
 *              for the gyroscope outputs.
 *
 ******************************************************************************/
static void alg_stabilizer_compute_pitch_roll( float* pitch, float* roll, float* gravity )
{
        /*
         * Read accel from the hardware
         */
        AppAccelGyroClass::motion6_data_type data;
        AclGyro.GetMotion6Data(&data);
        float ax = (float)data.ax;
        float ay = (float)data.ay;
        float az = (float)data.az;
        float gx = (float)data.gx;
        float gy = (float)data.gy;
        float gz = (float)data.gz;

        /*
         * Convert the data to units of g and degrees
         */
        float accel_divisor, gyro_divisor;
        AccelGyro.GetFullRangeDivisor(&accel_divisor,&gyro_divisor);

        ax /= accel_divisor; // Convert to g
        ay /= accel_divisor;
        az /= accel_divisor;
        *gravity = az;

        gx /= gyro_divisor; // Convert to degrees
        gy /= gyro_divisor;
        gz /= gyro_divisor;

        /*
         * Calculate roll and pitch
         */
        float accel_pitch = atan2f(ax,az);
        float accel_roll = atan2f(ay,sqrt(ax*ax+az*az));

        /*
         * Convert to degrees
         */
        accel_pitch = accel_pitch*180.0/M_PI;
        accel_roll = accel_roll*180.0/M_PI;

        /*
         * This uses a complimentary filter where gyro is
         * prioritized over accel.
         */
        static float loc_roll = 0;
        static float loc_pitch = 0;
        loc_roll = A*(loc_roll+gx*dt)+(1-A)*accel_roll;
        loc_pitch = A*(loc_pitch+gy*dt)+(1-A)*accel_pitch;

        *pitch = loc_pitch;
        *roll = loc_roll;
}

