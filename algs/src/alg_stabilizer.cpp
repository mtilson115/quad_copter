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

/*******************************************************************************
 * Local Function Section
 ******************************************************************************/
static void alg_stabilizer_task( void *p_arg );

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

    BSPMotor motor1(PWM1);
    BSPMotor motor2(PWM2);
    BSPMotor motor3(PWM3);
    BSPMotor motor4(PWM4);

    // Initialize the timer
    BSPMotor::InitTmr();

    // Initialize all the motors
    motor1.Init();
    motor2.Init();
    motor3.Init();
    motor4.Init();

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

        // Send the message
        comms_xbee_msg_t msg;
        msg.data = data_buff;
        msg.len = sizeof(data_buff);
        COMMS_xbee_send(msg);
    }
}
