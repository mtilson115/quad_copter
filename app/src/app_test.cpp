#include <os.h>
#include <app_cfg.h>
#include <p32xxxx.h>
#include "app_accel_gyro.h"
#include "driver_i2c.h"
#include "bsp_accel_gyro.h"
#include "type_defs.h"
#include "bsp_utils.h"
#include "driver_pwm.h"
#include "bsp.h"
#include "bsp_xbee.h"
#include "comms_xbee.h"
#include "bsp_accel_gyro_int.h"

static  OS_TCB    test_TCB;
static  CPU_STK   test_stack[APP_BUTTON_READER_STK_SIZE];
static  void  test_task   (void  *p_arg);
#define LOW_DUTY 4.0
#define MAX_DUTY 9.0
void start_test( void )
{

    // Initialize the pwm driver

    /*
    pwm_init_t init_settings = {
        25000,      // (80e6/64)/25000 = 50Hz (80e6 == clock source, 64 == pre-scale, 25000 == num ticks before roll over)
        LOW_DUTY,
    };
    PWM_init( PWM0, init_settings );

    // Start the PWM
    PWM_start( PWM0 );
    */

    // Make the RE0 pin an output
    TRISEbits.TRISE7 = 0;  // output
    ODCEbits.ODCE7 = 0; // CMOS outout

    OS_ERR err;
    OSTaskCreate((OS_TCB      *)&test_TCB,      /* Create the start task */
                 (CPU_CHAR    *)"Test Task",
                 (OS_TASK_PTR  )test_task,
                 (void        *)0,
                 (OS_PRIO      )APP_BUTTON_READER_PRIO,
                 (CPU_STK     *)&test_stack[0],
                 (CPU_STK_SIZE )APP_BUTTON_READER_STK_SIZE/10,
                 (CPU_STK_SIZE )APP_BUTTON_READER_STK_SIZE,
                 (OS_MSG_QTY   )10u,
                 (OS_TICK      )0u,
                 (void        *)0,
                 (OS_OPT       )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR      *)&err);

    /*
    BSP_xbee_register_tcb(&test_TCB);
    BSP_xbee_init();
    */
    bsp_accel_gyro_int_register(&test_TCB);
}

static void test_task(void  *p_arg)
{
    // float duty_cycle = LOW_DUTY;
    OS_ERR err;

    // Allow the printf system to init (xbee has to associate)
    BSP_PrintfInit();

    OSTimeDlyHMSM(0u, 0u, 2u, 0u,OS_OPT_TIME_HMSM_STRICT,&err);

    // Initialize the AccelGyro
    AclGyro.Init();
    AclGyro.PrintOffsets();

    uint32_t idx = 0;
    uint32_t ts = 0;
    while(DEF_ON)
    {
        // OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,&ts,&err);
        AclGyro.PrintMotion6Data();
        /*
        if( duty_cycle <= 7.0 )
        {
            PWM_chg_duty( PWM0, duty_cycle );
            duty_cycle += 0.1;
        }
        else
        {
            duty_cycle = LOW_DUTY;
        }
        */
        // AclGyro.PrintOffsets();
        OSTimeDlyHMSM(0u, 0u, 0u, 20u,OS_OPT_TIME_HMSM_STRICT,&err);
        PORTEINV = (1<<7);
        // BSP_Printf("Hello %d\n",idx++);
        // BSP_xbee_test();
    }
}
