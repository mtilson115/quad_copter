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
#include "driver_pwm.h"
#include "bsp_accel_gyro_int.h"
#include "bsp_utils.h"
#include <math.h>
#include <string.h>
#include <assert.h>

/*******************************************************************************
 * Constants
 ******************************************************************************/
#define ALG_STB_TX_Q_DEPTH (0)
#define LOW_PWM_DUTY_CYCLE (4.0)
#define MAX_PWM_DUTY_CYCLE (9.0)
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

    // Initial settings for PWM
    pwm_init_t init_settings = {
        .period = 6250,             // (80e6/256)/6250 = 50Hz (80e6 == clock source, 256 == pre-scale, 6250 == num ticks before roll over)
        .duty = LOW_PWM_DUTY_CYCLE,
    };

    // Initialize the timer
    PWM_init_tmr( init_settings.period );

    // Initialize all PWMs
    PWM_init( PWM1, init_settings );
    PWM_init( PWM2, init_settings );
    PWM_init( PWM3, init_settings );
    PWM_init( PWM4, init_settings );

    // Turn on the timer
    PWM_tmr_en( TRUE );

    // Start the PWMs
    PWM_start( PWM1 );
    PWM_start( PWM2 );
    PWM_start( PWM3 );
    PWM_start( PWM4 );

    PWM_oc3_work_around_init();

    // Necessary?
    OSTimeDlyHMSM(0u, 0u, 10u, 0u,OS_OPT_TIME_HMSM_STRICT,&err);

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

        // Compute PWM outputs
        /*
         * This is test code for now.  The real implementation will most likely use a PID
         */
        float angle_percent = .5;
        float pwm_duty_cycle = 6.0;
        if( accel_pitch > 0 )
        {
            angle_percent = (accel_pitch/90.0);
            pwm_duty_cycle = (MAX_PWM_DUTY_CYCLE - 6.0)*angle_percent+6.0;
            PWM_chg_duty( PWM1, pwm_duty_cycle );
            PWM_chg_duty( PWM2, pwm_duty_cycle );
            PWM_chg_duty( PWM3, pwm_duty_cycle );
            PWM_chg_duty( PWM4, pwm_duty_cycle );
        }
        else if( accel_pitch == 0 )
        {
            pwm_duty_cycle = 6.0;
            PWM_chg_duty( PWM1, pwm_duty_cycle );
            PWM_chg_duty( PWM2, pwm_duty_cycle );
            PWM_chg_duty( PWM3, pwm_duty_cycle );
            PWM_chg_duty( PWM4, pwm_duty_cycle );
        }

        // Allocate buffer and copy in the data
        uint8_t data_buff[3*sizeof(float)] = {0};
        memcpy(data_buff,&accel_pitch,sizeof(float));
        memcpy(&data_buff[sizeof(float)],&accel_roll,sizeof(float));
        memcpy(&data_buff[2*sizeof(float)],&pwm_duty_cycle,sizeof(float));

        // Send the message
        comms_xbee_msg_t msg;
        msg.data = data_buff;
        msg.len = sizeof(data_buff);
        COMMS_xbee_send(msg);
    }
}
