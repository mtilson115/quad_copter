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

/*******************************************************************************
 * Constants
 ******************************************************************************/
#define ALG_STB_TX_Q_DEPTH (0)
#define M_PI (3.14159)

#define ALG_STB_DBG_HDR_MOTOR_PITCH_ROLL (17)

/*******************************************************************************
 * Local Data
 ******************************************************************************/

// Task data
CPU_STK alg_stabilizer_stack[ALG_STABILIZER_STK_SIZE];
OS_TCB alg_stabilizer_TCB;
OS_MUTEX alg_stabilizer_throttle_mutex;
OS_MUTEX alg_stabilizer_PI_mutex;

// Motor objects
static BSPMotor motor1;
static BSPMotor motor2;
static BSPMotor motor3;
static BSPMotor motor4;

// Throttle
static float alg_stabilizer_throttle_percent = 0;

// PID constants
static float asP = 0.5;
static float asI = 0.005;

// Fitler coefficients
static float A = 0.8;
static float dt = 20e-3; // 20ms

/*******************************************************************************
 * Local Function Section
 ******************************************************************************/
static void alg_stabilizer_task( void *p_arg );
static void alg_stabilizer_throttle_msg_cb(uint8_t* data,uint16_t len);
static void alg_stabilizer_PI_msg_cb(uint8_t* data,uint16_t len);
static void alg_stabilizer_set_throttle( float throttle );
static float alg_stabilizer_get_throttle( void );
static void alg_stabilizer_set_PI_consts( float P, float I );
static void alg_stabilizer_get_PI_consts( float* P, float* I );
static void alg_stabilizer( float pitch, float roll );
static void alg_stabilizer_compute_pitch_roll( float* pitch, float* roll );

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
    OSMutexCreate(&alg_stabilizer_PI_mutex,(CPU_CHAR*)"PI Mutex",&err);
    assert(err==OS_ERR_NONE);

    // Register the TCB with the accel bsp code
    bsp_accel_gyro_int_register(&alg_stabilizer_TCB);

    // Register a message call back routine
    comms_xbee_rx_cb_t throttle_rx_cb = {
        .cb = alg_stabilizer_throttle_msg_cb,
        .msg_id = COMMS_SET_THROTTLE,
    };
    ret_t ret = COMMS_xbee_register_rx_cb(throttle_rx_cb);
    assert(ret==rSUCCESS);
    comms_xbee_rx_cb_t pi_rx_cb = {
        .cb = alg_stabilizer_PI_msg_cb,
        .msg_id = COMMS_SET_PI,
    };
    ret_t ret = COMMS_xbee_register_rx_cb(pi_rx_c);
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

    // Wait on comms
    BSP_PrintfInit();

    // Initialize the AccelGyro
    AclGyro.Init();

    uint32_t ts = 0;
    while (DEF_ON)
    {
        /*
         * Pend on the task semaphore (posted to from the interrupt for accel)
         */
        OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,&ts,&err);

        /*
         * Get the pitch and roll
         */
        float pitch, roll;
        alg_stabilizer_compute_pitch_roll(&pitch,&roll);

        /*
         * Apply the values
         */
        alg_stabilizer(pitch,roll);
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
 * alg_stabilizer_PI_msg_cb
 *
 * Description: This function is called when a message is recieved and has the
 *              msg_id = COMMS_SET_PI (0x01)
 *
 * Inputs:      None
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 03/02/2019 - Mitchell S. Tilson
 *
 ******************************************************************************/
static void alg_stabilizer_PI_msg_cb(uint8_t* data,uint16_t len)
{
    float loc_P;
    float loc_I;
    if( data[0] == COMMS_SET_PI )
    {
        len -= 1;
        if( len == sizeof(loc_P)+sizeof(loc_I) )
        {
            memcpy(&loc_P,&data[1],sizeof(loc_P));
            memcpy(&loc_P,&data[sizeof(loc_P)+1],sizeof(loc_P));
            alg_stabilizer_set_PI_consts(loc_P,loc_I);
        }
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
 * alg_stabilizer_set/get_PI_consts
 *
 * Description: These functions set and get the PI controller constants
 *
 * Inputs:      float P
 *              float I
 *
 * Returns:     float P
 *              float I
 *
 * Revision:    Initial Creation 04/25/2019 - Mitchell S. Tilson
 *
 * Notes:       These values are set from a different thread so its safer to use
 *              a mutex given it is a float.
 *
 ******************************************************************************/
static void alg_stabilizer_set_PI_consts( float P, float I )
{
    OS_ERR err;
    CPU_TS ts = 0;
    OSMutexPend(&alg_stabilizer_PI_mutex,0,OS_OPT_PEND_BLOCKING,&ts,&err);
    asP = P;
    asI = I;
    OSMutexPost(&alg_stabilizer_PI_mutex,OS_OPT_POST_NONE,&err);
}
static void alg_stabilizer_get_PI_consts( float* P, float* I )
{
    float throttle;
    OS_ERR err;
    CPU_TS ts = 0;
    OSMutexPend(&alg_stabilizer_PI_mutex,0,OS_OPT_PEND_BLOCKING,&ts,&err);
    *P = asP;
    *I = asI;
    OSMutexPost(&alg_stabilizer_PI_mutex,OS_OPT_POST_NONE,&err);
}

/*******************************************************************************
 * alg_stabilizer
 *
 * Description: This function takes in the pitch and roll and calculates the
 *              appropriate motor throttle values.
 *
 * Inputs:      float - pitch
 *              float - roll
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 03/03/2019 - Mitchell S. Tilson
 *
 * Notes:       Right now this function assumes the desired pitch and roll is 0.
 *
 ******************************************************************************/
static void alg_stabilizer( float pitch, float roll )
{
    static float pitch_sum = 0;
    static float roll_sum = 0;

    pitch_sum += pitch;
    roll_sum += roll;

    // Get the PI constants
    float P,I;
    alg_stabilizer_get_PI_consts(&P,&I);

    // Get the desired throttle
    float throttle_percent = alg_stabilizer_get_throttle();

    /*
     * Catch moving too much.
     */
    if( pitch > 30 || roll > 30 )
    {
        throttle_percent = 0;
    }

    // Calculate Motor1's desired throttle
    float motor1_throttle_err = P*(-pitch)+P*(-roll)+I*(-pitch_sum)+I*(-roll_sum);
    float motor1_throttle = throttle_percent + motor1_throttle_err;

    // Calculate Motor2's desired speed
    float motor2_throttle_err = P*(pitch)+P*(-roll)+I*(pitch_sum)+I*(-roll_sum);
    float motor2_throttle = throttle_percent + motor2_throttle_err;

    // Calculate Motor3's desired speed
    float motor3_throttle_err = P*(pitch)+P*(roll)+I*(pitch_sum)+I*(roll_sum);
    float motor3_throttle = throttle_percent + motor3_throttle_err;

    // Calculate Motor4's desired speed
    float motor4_throttle_err = P*(-pitch)+P*(roll)+I*(-pitch_sum)+I*(roll_sum);
    float motor4_throttle = throttle_percent + motor4_throttle_err;

    motor1.SetSpeedPercent( motor1_throttle/100.0 );
    motor2.SetSpeedPercent( motor2_throttle/100.0 );
    motor3.SetSpeedPercent( motor3_throttle/100.0 );
    motor4.SetSpeedPercent( motor4_throttle/100.0 );

    /*
     * Debug data
     */
    uint8_t msg_hdr = ALG_STB_DBG_HDR_MOTOR_PITCH_ROLL;
    uint8_t data_buff[sizeof(msg_hdr)+8*sizeof(float)] = {0};
    data_buff[0] = msg_hdr;
    memcpy(&data_buff[sizeof(msg_hdr)],&motor1_throttle,sizeof(float));
    memcpy(&data_buff[sizeof(float)+sizeof(msg_hdr)],&motor2_throttle,sizeof(float));
    memcpy(&data_buff[2*sizeof(float)+sizeof(msg_hdr)],&motor3_throttle,sizeof(float));
    memcpy(&data_buff[3*sizeof(float)+sizeof(msg_hdr)],&motor4_throttle,sizeof(float));
    memcpy(&data_buff[4*sizeof(float)+sizeof(msg_hdr)],&pitch,sizeof(float));
    memcpy(&data_buff[5*sizeof(float)+sizeof(msg_hdr)],&roll,sizeof(float));
    memcpy(&data_buff[6*sizeof(float)+sizeof(msg_hdr)],&P,sizeof(float));
    memcpy(&data_buff[7*sizeof(float)+sizeof(msg_hdr)],&I,sizeof(float));

    // Send the message
    comms_xbee_msg_t msg;
    msg.data = data_buff;
    msg.len = sizeof(data_buff);
    COMMS_xbee_send(msg);
}

/*******************************************************************************
 * alg_stabilizer_compute_pitch_roll
 *
 * Description: This function computes the pitch and roll using the x,y,z accel
 *              and gyroscope data.
 *
 * Outputs:     float - pitch
 *              float - roll
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
static void alg_stabilizer_compute_pitch_roll( float* pitch, float* roll )
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

        gx /= gyro_divisor; // Convert to degrees
        gy /= gyro_divisor;
        gz /= gyro_divisor;

        /*
         * Calculate roll and pitch
         */
        float accel_pitch = atan2(ax,sqrt(ay*ay+az*az));
        float accel_roll = atan2(ay,sqrt(ax*ax+az*az));

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

