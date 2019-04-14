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
#include <math.h>
#include <string.h>
#include <assert.h>

/*******************************************************************************
 * Constants
 ******************************************************************************/
#define ALG_STB_TX_Q_DEPTH (0)
#define M_PI (3.14159)

/*******************************************************************************
 * Local Data
 ******************************************************************************/

// Task data
CPU_STK alg_stabilizer_stack[ALG_STABILIZER_STK_SIZE];
OS_TCB alg_stabilizer_TCB;
OS_MUTEX alg_stabilizer_throttle_mutex;

// Motor objects
static BSPMotor motor1;
static BSPMotor motor2;
static BSPMotor motor3;
static BSPMotor motor4;

// Throttle
static float alg_stabilizer_throttle = 0;

/*******************************************************************************
 * Local Function Section
 ******************************************************************************/
static void alg_stabilizer_task( void *p_arg );
static void alg_stabilizer_msg_cb(uint8_t* data,uint16_t len);
static void alg_stabilizer_set_throttle( float throttle );
static float alg_stabilizer_get_throttle( void );
static void alg_stabilizer( float pitch, float roll );

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

    // Register the TCB with the accel bsp code
    bsp_accel_gyro_int_register(&alg_stabilizer_TCB);

    // Register a message call back routine
    comms_xbee_rx_cb_t rx_cb = {
        .cb = alg_stabilizer_msg_cb,
        .msg_id = COMMS_SET_THROTTLE,
    };
    ret_t ret = COMMS_xbee_register_rx_cb(rx_cb);
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

    // Delay here if necessary

    // Wait on comms
    BSP_PrintfInit();

    // Initialize the AccelGyro
    AclGyro.Init();

    uint32_t ts = 0;
    while (DEF_ON)
    {
        // Pend on the task semaphore (posted to from the interrupt for accel)
        OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,&ts,&err);

        // Read accel
        AppAccelGyroClass::motion6_data_type data;
        AclGyro.GetMotion6Data(&data);

        // Calculate roll and pitch
        float accel_pitch = atan2(data.ax,data.az);
        float accel_roll = atan2(data.ay,data.az);

        // Convert to degrees
        accel_pitch = accel_pitch*180.0/M_PI;
        accel_roll = accel_roll*180.0/M_PI;

        // alg_stabilizer(accel_pitch,accel_roll);

        // Compute motor outputs
        /*
         * This is test code for now.  The real implementation will most likely use a PID
         */

        float angle_percent = .10;
        if( accel_pitch > 0 )
        {
            angle_percent = (accel_pitch/90.0);
            motor1.SetSpeedPercent( angle_percent );
            motor2.SetSpeedPercent( angle_percent );
            motor3.SetSpeedPercent( angle_percent );
            motor4.SetSpeedPercent( angle_percent );
        }
        else if( accel_pitch == 0 )
        {
            motor1.SetSpeedPercent( angle_percent );
            motor2.SetSpeedPercent( angle_percent );
            motor3.SetSpeedPercent( angle_percent );
            motor4.SetSpeedPercent( angle_percent );
        }

        // Allocate buffer and copy in the data
        uint8_t data_buff[3*sizeof(float)] = {0};
        memcpy(data_buff,&accel_pitch,sizeof(float));
        memcpy(&data_buff[sizeof(float)],&accel_roll,sizeof(float));
        memcpy(&data_buff[2*sizeof(float)],&angle_percent,sizeof(float));

        // uint8_t data_buff[] = "Hello";

        // Send the message
        comms_xbee_msg_t msg;
        msg.data = data_buff;
        msg.len = sizeof(data_buff);
        COMMS_xbee_send(msg);
    }
}

/*******************************************************************************
 * alg_stabilizer_msg_cb
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
static void alg_stabilizer_msg_cb(uint8_t* data,uint16_t len)
{
    float throttle_percent = 0;
    if( data[0] == COMMS_SET_THROTTLE )
    {
        len -= 1;
        if( len == sizeof(throttle_percent) )
        {
            memcpy(&throttle_percent,&data[1],len);
            if( throttle_percent >= 0 && throttle_percent <= 1 )
            {
                alg_stabilizer_set_throttle(throttle_percent);
            }
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
    alg_stabilizer_throttle = throttle;
    OSMutexPost(&alg_stabilizer_throttle_mutex,OS_OPT_POST_NONE,&err);
}
static float alg_stabilizer_get_throttle( void )
{
    float throttle;
    OS_ERR err;
    CPU_TS ts = 0;
    OSMutexPend(&alg_stabilizer_throttle_mutex,0,OS_OPT_PEND_BLOCKING,&ts,&err);
    throttle = alg_stabilizer_throttle;
    OSMutexPost(&alg_stabilizer_throttle_mutex,OS_OPT_POST_NONE,&err);
    return throttle;
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

    static const float asP = 0.1;
    static const float asI = 0.001;

    pitch_sum += pitch;
    roll_sum += roll;

    // Get the desired throttle
    float throttle = alg_stabilizer_get_throttle();

    // Calculate Motor1's desired throttle
    float motor1_throttle_err = asP*(-pitch)+asP*(-roll)+asI*(-pitch_sum)+asI*(-roll_sum);
    float motor1_throttle = throttle + motor1_throttle_err;

    // Calculate Motor2's desired speed
    float motor2_throttle_err = asP*(pitch)+asP*(-roll)+asI*(pitch_sum)+asI*(-roll_sum);
    float motor2_throttle = throttle + motor2_throttle_err;

    // Calculate Motor3's desired speed
    float motor3_throttle_err = asP*(pitch)+asP*(roll)+asI*(pitch_sum)+asI*(roll_sum);
    float motor3_throttle = throttle + motor3_throttle_err;

    // Calculate Motor4's desired speed
    float motor4_throttle_err = asP*(-pitch)+asP*(roll)+asI*(-pitch_sum)+asI*(roll_sum);
    float motor4_throttle = throttle + motor4_throttle_err;
}
