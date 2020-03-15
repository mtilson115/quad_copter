/*******************************************************************************
 * File:    alg_stabilizer.cpp
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
#include "bsp_ag.h"
#include "comms_xbee.h"
#include "bsp_accel_gyro_int.h"
#include "bsp_utils.h"
#include "bsp_motor.h"
#include "bsp_accel_gyro.h"
#include "cfg.h"
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

/*******************************************************************************
 * Typedefs
 ******************************************************************************/
typedef struct __attribute__ ((packed)) {
    uint8_t hdr;
    uint64_t ts;
    float m1;
    float m2;
    float m3;
    float m4;
    float pitch;
    float pitch_sp;
    float roll;
    float roll_sp;
    float yaw;
    float yaw_sp;
} alg_stabilizer_debug_t;

typedef struct __attribute__ ((packed)) {
    float pP;
    float pI;
    float pD;
    float rP;
    float rI;
    float rD;
    float A;
} alg_stabilizer_tuning_debug_t;

/*******************************************************************************
 * Local Data
 ******************************************************************************/

// Task data
CPU_STK alg_stabilizer_stack[ALG_STABILIZER_STK_SIZE];
OS_TCB alg_stabilizer_TCB;

// Motor objects
static BSPMotor motor1;
static BSPMotor motor2;
static BSPMotor motor3;
static BSPMotor motor4;

// Throttle
static float alg_stabilizer_throttle_percent = 0.0;
static float alg_stabilizer_throttle_percent_target = 0.0;

// PID constant
static float pitch_asP = 0.2;
static float pitch_asI = 0.0045;
static float pitch_asD = 0.002;

static float roll_asP = 0.1;
static float roll_asI = 0.003;
static float roll_asD = 0.0015;

// Fitler coefficients
static float A = 0.985;
static float dt = 5e-3; // units s

// Calibration
static bool do_calibration = false;

// Pitch and roll
static float pitch_target_set_point = 0;
static float roll_target_set_point = 0;
static float yaw_target_set_point = 0;

static float pitch_actual_set_point = 0;
static float roll_actual_set_point = 0;
static float yaw_actual_set_point = 0;

// Send Debug info
static bool send_debug_m_pr = false;

// Message tracking
static bool throttle_rcvd = false;
static bool pid_rcvd = false;
static bool cal_req_rcvd = false;
static bool debug_rcvd = false;
static bool pitch_roll_yaw_rcvd = false;

/*******************************************************************************
 * Local Function Section
 ******************************************************************************/
static void alg_stabilizer_task( void *p_arg );
static void alg_stabilizer_throttle_msg_cb(uint8_t* data,uint16_t len);
static void alg_stabilizer_PID_msg_cb(uint8_t* data,uint16_t len);
static void alg_stabilizer_calibrate_msg_cb(uint8_t* data,uint16_t len);
static void alg_stabilizer_debug_m_pr_cb(uint8_t* data,uint16_t len);
static void alg_stabilizer_filter_coef_cb(uint8_t* data,uint16_t len);
static void alg_stabilizer_set_throttle( float throttle );
static float alg_stabilizer_get_throttle( void );
static void alg_stabilizer_set_pitch_PID_consts( float P, float I, float D );
static void alg_stabilizer_set_roll_PID_consts( float P, float I, float D );
static void alg_stabilizer_get_pitch_PID_consts( float* P, float* I, float *D );
static void alg_stabilizer_get_roll_PID_consts( float* P, float* I, float *D );
static void alg_stabilizer( float pitch, float roll, float yaw, float gravity );
static void alg_stabilizer_compute_pitch_roll_yaw( float* pitch, float* roll, float* yaw, float* gravity );
static void alg_stabilizer_set_calibrate( bool en );
static bool alg_stabilizer_get_calibrate( void );
static void alg_stabilizer_set_debug_m_pr( uint8_t en );
static bool alg_stabilizer_get_debug_m_pr( void );
static void alg_stabilizer_pitch_roll_yaw_msg_cb( uint8_t* data, uint16_t len );
static void alg_stabilizer_set_pitch_roll_yaw( float pitch, float roll, float yaw );
static void alg_stabilizer_get_pitch_roll_yaw( float* pitch, float* roll, float* yaw );
static void alg_stabilizer_update_throttle( void );
static void alg_stabilizer_update_ypr_sp( void );
static void alg_stabilizer_lost_connection_cb( void );
static void alg_stabilizer_ack_msgs( void );

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

    // Register the TCB with the accel bsp code
    bsp_accel_gyro_int_register(&alg_stabilizer_TCB);

    /*
     * Throttle callback
     */
    comms_xbee_rx_cb_t throttle_rx_cb = {
        .cb = alg_stabilizer_throttle_msg_cb,
        .msg_id = COMMS_SET_THROTTLE,
    };
    ret_t ret = comms_xbee_register_rx_cb(throttle_rx_cb);
    assert(ret==rSUCCESS);

    /*
     * Set PID callback
     */
    comms_xbee_rx_cb_t pi_rx_cb = {
        .cb = alg_stabilizer_PID_msg_cb,
        .msg_id = COMMS_SET_PID,
    };
    ret = comms_xbee_register_rx_cb(pi_rx_cb);
    assert(ret==rSUCCESS);

    /*
     * Execute calibration callback
     */
    comms_xbee_rx_cb_t calibrate_cb = {
        .cb = alg_stabilizer_calibrate_msg_cb,
        .msg_id = COMMS_CALIBRATE,
    };
    ret = comms_xbee_register_rx_cb(calibrate_cb);
    assert(ret==rSUCCESS);

    /*
     * Send motor, pitch, and roll debug info
     */
    comms_xbee_rx_cb_t debug_m_pr_cb = {
        .cb = alg_stabilizer_debug_m_pr_cb,
        .msg_id = COMMS_DEBUG_M_PR,
    };
    ret = comms_xbee_register_rx_cb(debug_m_pr_cb);
    assert(ret==rSUCCESS);

    /*
     * Adjust the complimentary filter coeficient
     */
    comms_xbee_rx_cb_t filter_coef_cb = {
        .cb = alg_stabilizer_filter_coef_cb,
        .msg_id = COMMS_SET_COMP_FILT_CONST,
    };
    ret = comms_xbee_register_rx_cb(filter_coef_cb);
    assert(ret==rSUCCESS);

    /*
     * Set pitch and roll setpoints
     */
    comms_xbee_rx_cb_t pitch_roll_yaw_cb = {
        .cb = alg_stabilizer_pitch_roll_yaw_msg_cb,
        .msg_id = COMMS_PITCH_ROLL,
    };
    ret = comms_xbee_register_rx_cb(pitch_roll_yaw_cb);
    assert(ret==rSUCCESS);

    /*
     * Register the function responsible for handling a lost comms connection
     */
    comms_xbee_register_lost_connection_cb( alg_stabilizer_lost_connection_cb );
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
    // Motors start at 100% when throttle cal is enabled

    // Delay 6s
    OSTimeDlyHMSM(0u, 0u, 6u, 0u,OS_OPT_TIME_HMSM_STRICT,&err);

    motor1.SetSpeedPercent(0.05);
    motor2.SetSpeedPercent(0.05);
    motor3.SetSpeedPercent(0.05);
    motor4.SetSpeedPercent(0.05);

    // Delay 2.25s
    OSTimeDlyHMSM(0u, 0u, 2u, 250u,OS_OPT_TIME_HMSM_STRICT,&err);
#endif


    // Initialize the AccelGyro
    AclGyro.Init();

    // Start the accel gyro interrupt
    AclGyro.Start();

    while( !comms_xbee_ready() )
    {
        OSTimeDlyHMSM(0u, 0u, 1u, 0u,OS_OPT_TIME_HMSM_STRICT,&err);
    }
    uint32_t ts = 0;
    while (DEF_ON)
    {
        /*
         * Pend on the task semaphore (posted to from the interrupt for accel)
         */
        OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,&ts,&err);

        /*
         * Check to see if calibration was requested.
         * The throttle should be off.
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
         * This is called to ramp the user set throttle to avoid
         * fast changes leading to noise spikes and unstable operation
         * of the CPU and accel gyro.
         */
        alg_stabilizer_update_throttle();

        /*
         * This is called to ramp the user set angle to avoid
         * fast changes and instability
         */
        alg_stabilizer_update_ypr_sp();

        /*
         * Get the pitch and roll
         * gravity is included for error conditions.
         * If -gravity is detected, somethings wrong in this design as
         * the quad copter is upside down.
         */
        float pitch, roll, yaw, gravity;
        alg_stabilizer_compute_pitch_roll_yaw(&pitch,&roll,&yaw,&gravity);

        /*
         * Apply the values
         */
        alg_stabilizer(pitch,roll,yaw,gravity);

        /*
         * Ack all messages that we acted on.
         */
        alg_stabilizer_ack_msgs();
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
                throttle_rcvd = true;
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
    float loc_pitch_P;
    float loc_pitch_I;
    float loc_pitch_D;
    float loc_roll_P;
    float loc_roll_I;
    float loc_roll_D;
    unsigned int loc_len = sizeof(float)*6;
    if( data[0] == COMMS_SET_PID )
    {
        len -= 1;
        if( len == loc_len )
        {
            memcpy(&loc_pitch_P,&data[1],sizeof(float));
            memcpy(&loc_pitch_I,&data[sizeof(float)+1],sizeof(float));
            memcpy(&loc_pitch_D,&data[2*sizeof(float)+1],sizeof(float));
            memcpy(&loc_roll_P,&data[3*sizeof(float)+1],sizeof(float));
            memcpy(&loc_roll_I,&data[4*sizeof(float)+1],sizeof(float));
            memcpy(&loc_roll_D,&data[5*sizeof(float)+1],sizeof(float));
            alg_stabilizer_set_pitch_PID_consts(loc_pitch_P,loc_pitch_I,loc_pitch_D);
            alg_stabilizer_set_roll_PID_consts(loc_roll_P,loc_roll_I,loc_roll_D);
            pid_rcvd = true;
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
        cal_req_rcvd = true;
    }
}

/*******************************************************************************
 * alg_stabilizer_debug_m_pr_cb
 *
 * Description: This function is called when a message is recieved and has the
 *              msg_id = COMMS_DEBUG_M_PR (0x05)
 *
 * Inputs:      None
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 09/01/2019 - Mitchell S. Tilson
 *
 ******************************************************************************/
static void alg_stabilizer_debug_m_pr_cb(uint8_t* data,uint16_t len)
{
    if( data[0] == COMMS_DEBUG_M_PR )
    {
        alg_stabilizer_set_debug_m_pr( data[1] );
        debug_rcvd = true;
    }
}

/*******************************************************************************
 * alg_stabilizer_filter_coef_cb
 *
 * Description: This function is called when a message is recieved and has the
 *              msg_id = COMMS_SET_COMP_FILT_CONST (0x09) and is used to adjust
 *              the complimentary filter coefficient
 *
 * Inputs:      None
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 01/19/2020 - Mitchell S. Tilson
 *
 ******************************************************************************/
static void alg_stabilizer_filter_coef_cb(uint8_t* data,uint16_t len)
{
    if( data[0] == COMMS_SET_COMP_FILT_CONST )
    {
        len -= 1;
        if( len == sizeof(A) )
        {
            memcpy(&A,&data[1],len);
        }
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
        pitch_roll_yaw_rcvd = true;
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
    alg_stabilizer_throttle_percent_target = throttle;
}
static float alg_stabilizer_get_throttle( void )
{
    return alg_stabilizer_throttle_percent;
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
static void alg_stabilizer_set_pitch_PID_consts( float P, float I, float D )
{
    pitch_asP = P;
    pitch_asI = I;
    pitch_asD = D;
}
static void alg_stabilizer_get_pitch_PID_consts( float* P, float* I, float* D )
{
    *P = pitch_asP;
    *I = pitch_asI;
    *D = pitch_asD;
}
static void alg_stabilizer_set_roll_PID_consts( float P, float I, float D )
{
    roll_asP = P;
    roll_asI = I;
    roll_asD = D;
}
static void alg_stabilizer_get_roll_PID_consts( float* P, float* I, float* D )
{
    *P = roll_asP;
    *I = roll_asI;
    *D = roll_asD;
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
    do_calibration = en;
}
static bool alg_stabilizer_get_calibrate( void )
{
    return do_calibration;
}

/*******************************************************************************
 * alg_stabilizer_set_debug_m_pr/alg_stabilizer_get_debug_m_pr
 *
 * Description: This function sets a variable indicating debug information should
 *              be sent.
 *
 * Inputs:      None
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 09/01/2019 - Mitchell S. Tilson
 *
 * Notes:
 *
 ******************************************************************************/
static void alg_stabilizer_set_debug_m_pr( uint8_t en )
{
    send_debug_m_pr = (bool)en;
}
static bool alg_stabilizer_get_debug_m_pr( void )
{
    return send_debug_m_pr;
}

/*******************************************************************************
 * alg_stabilizer_set_pitch_roll_yaw/alg_stabilizer_get_pitch_roll_yaw
 *
 * Description: This function sets sets the pitch and roll set points.
 *
 * Inputs:      float pitch - the pitch the controller will strive for
 *              float roll - the roll the controller will strive for
 *              float yaw - the yaw the controller will strive for
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
    pitch_target_set_point = pitch;
    roll_target_set_point = roll;
    yaw_target_set_point = yaw;
}
static void alg_stabilizer_get_pitch_roll_yaw( float* pitch, float* roll, float* yaw )
{
    *pitch = pitch_actual_set_point;
    *roll = roll_actual_set_point;
    *yaw = yaw_actual_set_point;
}

/*******************************************************************************
 * alg_stabilizer
 *
 * Description: This function takes in the pitch and roll and calculates the
 *              appropriate motor throttle values.
 *
 * Inputs:      float - pitch
 *              float - roll
 *              float - yaw
 *              float - gravity (for error detection if negative or upside down)
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 03/03/2019 - Mitchell S. Tilson
 *
 * Notes:       Right now this function assumes the desired pitch and roll is 0.
 *
 ******************************************************************************/
static void alg_stabilizer( float pitch, float roll, float yaw, float gravity )
{
    // Read the set points
    float loc_pitch_sp = 0;
    float loc_roll_sp = 0;
    float loc_yaw_sp = 0;
    alg_stabilizer_get_pitch_roll_yaw( &loc_pitch_sp, &loc_roll_sp, &loc_yaw_sp );

    // Calculate the errors
    float pitch_error = pitch - loc_pitch_sp;
    float roll_error = roll - loc_roll_sp;
    float yaw_error = yaw - loc_yaw_sp;

    // Integral and derivative variables
    static float pitch_error_sum = 0;
    static float roll_error_sum = 0;
    static float yaw_error_sum = 0;
    static float last_pitch_error = 0;
    static float last_roll_error = 0;
    static float last_yaw_error = 0;
    float pitch_d = pitch_error-last_pitch_error;
    float roll_d = roll_error-last_roll_error;
    float yaw_d = yaw_error-last_yaw_error;

    float loc_dt = dt;

    // Get the PID control parameters
    float pitchP,pitchI,pitchD;
    alg_stabilizer_get_pitch_PID_consts(&pitchP,&pitchI,&pitchD);
    pitchI = pitchI*loc_dt;
    pitchD = pitchD/loc_dt;
    float rollP,rollI,rollD;
    alg_stabilizer_get_roll_PID_consts(&rollP,&rollI,&rollD);
    rollI = rollI*loc_dt;
    rollD = rollD/loc_dt;

    // Get the desired throttle
    float throttle_percent = alg_stabilizer_get_throttle();

    /*
     * 7 inch props start to fly at 39%
     */
    if( throttle_percent > 39 )
    {
        pitch_error_sum += pitch_error;
        roll_error_sum += roll_error;
        yaw_error_sum += yaw_error;
    }
    else
    {
        pitch_error_sum = 0;
        roll_error_sum = 0;
        yaw_error_sum = 0;
    }


    // Calculate Motor1's desired throttle
    float motor1_throttle_err = pitchP*(pitch_error)-rollP*(roll_error)+pitchI*(pitch_error_sum)-rollI*(roll_error_sum)+pitchD*(pitch_d)-rollD*(roll_d);
    motor1_throttle_err = max(motor1_throttle_err,-throttle_percent);
    float motor1_throttle = throttle_percent + motor1_throttle_err;
    motor1_throttle = min(motor1_throttle,100);

    // Calculate Motor2's desired speed
    float motor2_throttle_err = -1.0*pitchP*(pitch_error)-rollP*(roll_error)-pitchI*(pitch_error_sum)-rollI*(roll_error_sum)-pitchD*(pitch_d)-rollD*(roll_d);
    motor2_throttle_err = max(motor2_throttle_err,-throttle_percent);
    float motor2_throttle = throttle_percent + motor2_throttle_err;
    motor2_throttle = min(motor2_throttle,100);

    // Calculate Motor3's desired speed
    float motor3_throttle_err = -1.0*pitchP*(pitch_error)+rollP*(roll_error)-pitchI*(pitch_error_sum)+rollI*(roll_error_sum)-pitchD*(pitch_d)+rollD*(roll_d);
    motor3_throttle_err = max(motor3_throttle_err,-throttle_percent);
    float motor3_throttle = throttle_percent + motor3_throttle_err;
    motor3_throttle = min(motor3_throttle,100);

    // Calculate Motor4's desired speed
    float motor4_throttle_err = pitchP*(pitch_error)+rollP*(roll_error)+pitchI*(pitch_error_sum)+rollI*(roll_error_sum)+pitchD*(pitch_d)+rollD*(roll_d);
    motor4_throttle_err = max(motor4_throttle_err,-throttle_percent);
    float motor4_throttle = throttle_percent + motor4_throttle_err;
    motor4_throttle = min(motor4_throttle,100);

    // Handle yaw error
    float m24_error = rollP*(yaw_error)+(rollI/2.0)*(yaw_error_sum)+rollD*(yaw_d);
    float m13_error = -rollP*(yaw_error)-(rollI/2.0)*(yaw_error_sum)-rollD*(yaw_d);

    motor1_throttle += m13_error;
    motor2_throttle += m24_error;
    motor3_throttle += m13_error;
    motor4_throttle += m24_error;

    motor1.SetSpeedPercent( motor1_throttle/100.0 );
    motor2.SetSpeedPercent( motor2_throttle/100.0 );
    motor3.SetSpeedPercent( motor3_throttle/100.0 );
    motor4.SetSpeedPercent( motor4_throttle/100.0 );

    last_pitch_error = pitch_error;
    last_roll_error = roll_error;
    last_yaw_error = yaw_error;

    /*
     * Debug data
     */
    if( alg_stabilizer_get_debug_m_pr() )
    {
        OS_ERR err;
        alg_stabilizer_debug_t debug_msg = {
            .hdr = COMMS_DBG_HDR_MOTOR_PITCH_ROLL,
            .ts = OSTimeGet(&err),
            .m1 = motor1_throttle,
            .m2 = motor2_throttle,
            .m3 = motor3_throttle,
            .m4 = motor4_throttle,
            .pitch = pitch,
            .pitch_sp = loc_pitch_sp,
            .roll = roll,
            .roll_sp = loc_roll_sp,
            .yaw = yaw,
            .yaw_sp = loc_yaw_sp,
        };

        // Send the message
        comms_xbee_msg_t msg;
        msg.data = (uint8_t*)&debug_msg;
        msg.len = sizeof(debug_msg);
        comms_xbee_send(msg);
    }

}

/*******************************************************************************
 * alg_stabilizer_compute_pitch_roll_yaw
 *
 * Description: This function computes the pitch and roll using the x,y,z accel
 *              and gyroscope data.
 *
 * Outputs:     float - pitch
 *              float - roll
 *              float - yaw
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
static void alg_stabilizer_compute_pitch_roll_yaw( float* pitch, float* roll, float* yaw, float* gravity )
{
    float loc_dt = dt;

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
#if SEND_DEBUG2 == 1
    static float g_pitch_sum = 0;
    static float g_roll_sum = 0;
    g_roll_sum = g_roll_sum + gx*loc_dt;
    g_pitch_sum = g_pitch_sum + gy*loc_dt;
#endif
    float accel_pitch = 0;
    float accel_roll = 0;

    /*
     * Avoid using the values if atan2 is going
     * to be unstable.
     */
    if( az > 0.01 )
    {
        // accel_pitch = atan2f(-ax,az);
        accel_pitch = -atan2f(ax,sqrt(ay*ay+az*az));
        accel_roll = atan2f(ay,sqrt(ax*ax+az*az));
        // float accel_roll = atan2f(ay,az);
        /*
         * Convert to degrees
         */
        accel_pitch = accel_pitch*180.0/M_PI;
        accel_roll = accel_roll*180.0/M_PI;
    }

    /*
     * This uses a complimentary filter where gyro is
     * prioritized over accel.
     */
    static float loc_roll = 0;
    static float loc_pitch = 0;
    float loc_yaw = 0;
    loc_roll = A*(loc_roll+gx*loc_dt)+(1-A)*accel_roll;
    loc_pitch = A*(loc_pitch+gy*loc_dt)+(1-A)*accel_pitch;
    loc_yaw = gz;

    float throttle_percent = alg_stabilizer_get_throttle();
    if( throttle_percent < 10.0 )
    {
        loc_pitch = 0;
        loc_roll = 0;
        loc_yaw = 0;
    }
    *pitch = loc_pitch;
    *roll = loc_roll;
    *yaw = loc_yaw;

#if SEND_DEBUG2 == 1
    uint8_t msg_hdr = COMMS_DBG_HDR_PITCH_ROLL;
    uint8_t data_buff[sizeof(msg_hdr)+6*sizeof(float)] = {0};
    data_buff[0] = msg_hdr;
    memcpy(&data_buff[sizeof(msg_hdr)],&g_pitch_sum,sizeof(float));
    memcpy(&data_buff[sizeof(float)+sizeof(msg_hdr)],&g_roll_sum,sizeof(float));
    memcpy(&data_buff[2*sizeof(float)+sizeof(msg_hdr)],&accel_pitch,sizeof(float));
    memcpy(&data_buff[3*sizeof(float)+sizeof(msg_hdr)],&accel_roll,sizeof(float));
    memcpy(&data_buff[4*sizeof(float)+sizeof(msg_hdr)],&loc_pitch,sizeof(float));
    memcpy(&data_buff[5*sizeof(float)+sizeof(msg_hdr)],&loc_roll,sizeof(float));

    // Send the message
    comms_xbee_msg_t msg;
    msg.data = data_buff;
    msg.len = sizeof(data_buff);
    comms_xbee_send(msg);
#endif
}

/*******************************************************************************
 * alg_stabilizer_update_throttle
 *
 * Description: Ramps the throttle to avoid power spikes due to braking
 *
 * Outputs:     none
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 09/05/2019 - Mitchell S. Tilson
 *
 ******************************************************************************/
static void alg_stabilizer_update_throttle( void )
{
    if( alg_stabilizer_throttle_percent_target > (alg_stabilizer_throttle_percent + THROTTLE_ADJUST_INC ) )
    {
        alg_stabilizer_throttle_percent += THROTTLE_ADJUST_INC;
    }
    else if( alg_stabilizer_throttle_percent_target < (alg_stabilizer_throttle_percent - THROTTLE_ADJUST_INC ) )
    {
        alg_stabilizer_throttle_percent -= THROTTLE_ADJUST_INC;
    }
    else
    {
        alg_stabilizer_throttle_percent = alg_stabilizer_throttle_percent_target;
    }
}

/*******************************************************************************
 * alg_stabilizer_update_ypr_sp
 *
 * Description: Controls how fast the angle can actually change for yaw, pitch, or roll
 *
 * Outputs:     none
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 09/05/2019 - Mitchell S. Tilson
 *
 ******************************************************************************/
static void alg_stabilizer_update_ypr_sp( void )
{
    // Pitch
    if( pitch_target_set_point > (pitch_actual_set_point + ANGLE_RATE_INC ) )
    {
        pitch_actual_set_point += ANGLE_RATE_INC;
    }
    else if( pitch_target_set_point < (pitch_actual_set_point - ANGLE_RATE_INC) )
    {
        pitch_actual_set_point -= ANGLE_RATE_INC;
    }
    else
    {
        pitch_actual_set_point = pitch_target_set_point;
    }

    // Roll
    if( roll_target_set_point > (roll_actual_set_point + ANGLE_RATE_INC ) )
    {
        roll_actual_set_point += ANGLE_RATE_INC;
    }
    else if( roll_target_set_point < (roll_actual_set_point - ANGLE_RATE_INC) )
    {
        roll_actual_set_point -= ANGLE_RATE_INC;
    }
    else
    {
        roll_actual_set_point = roll_target_set_point;
    }

    // Yaw
    if( yaw_target_set_point > (yaw_actual_set_point + ANGLE_RATE_INC ) )
    {
        yaw_actual_set_point += ANGLE_RATE_INC;
    }
    else if( yaw_target_set_point < (yaw_actual_set_point - ANGLE_RATE_INC) )
    {
        yaw_actual_set_point -= ANGLE_RATE_INC;
    }
    else
    {
        yaw_actual_set_point = yaw_target_set_point;
    }
}

/*******************************************************************************
 * alg_stabilizer_lost_connection_cb
 *
 * Description: Handles the event where the comms lose connection
 *
 * Outputs:     none
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 11/29/2019 - Mitchell S. Tilson
 *
 ******************************************************************************/
static void alg_stabilizer_lost_connection_cb( void )
{
    alg_stabilizer_set_throttle( 0 );
}

/*******************************************************************************
 * alg_stabilizer_ack_msgs
 *
 * Description: Sends messages back to the controller indicating a message was
 *              received.
 *
 * Outputs:     none
 *
 * Returns:     None
 *
 * Revision:    Initial Creation 12/6/2019 - Mitchell S. Tilson
 *
 ******************************************************************************/
static void alg_stabilizer_ack_msgs( void )
{
    if( throttle_rcvd )
    {
        comms_xbee_send_ack(COMMS_SET_THROTTLE);
        throttle_rcvd = false;
    }
    if( pid_rcvd )
    {
        comms_xbee_send_ack(COMMS_SET_PID);
        pid_rcvd = false;
    }
    if( cal_req_rcvd )
    {
        comms_xbee_send_ack(COMMS_CALIBRATE);
        cal_req_rcvd = false;
    }
    if( debug_rcvd )
    {
        comms_xbee_send_ack(COMMS_DEBUG_M_PR);
        debug_rcvd = false;
    }
    if( pitch_roll_yaw_rcvd )
    {
        comms_xbee_send_ack(COMMS_PITCH_ROLL);
        pitch_roll_yaw_rcvd = false;
    }
}
