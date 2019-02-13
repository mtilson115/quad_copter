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
#include <math.h>

/*******************************************************************************
 * Constants
 ******************************************************************************/
#define ALG_STB_TX_Q_DEPTH (0)

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
    // Initialize the AccelGyro
    AclGyro.Init();
    OS_ERR err;
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
    }
}
