#include <os.h>
#include <app_cfg.h>
#include <p32xxxx.h>
#include "app_accel_gyro.h"
#include "driver_i2c.h"
#include "bsp_accel_gyro.h"
#include "type_defs.h"
#include "bsp_utils.h"
#include "driver_pwm.h"

static  OS_TCB    reader_TCB;
static  CPU_STK   reaer_stack[APP_BUTTON_READER_STK_SIZE];
static  void  reader_task   (void  *p_arg);
void start_reader( void )
{
    // Initialize the AccelGyro
    AclGyro.Init();

    // Initialize the pwm driver
    pwm_init_t init_settings = {
        2048,
        50,
    };
    PWM_init( PWM0, init_settings );

    // Start the PWM
    PWM_start( PWM0 );

		
    OS_ERR err;
    OSTaskCreate((OS_TCB      *)&reader_TCB,                        /* Create the start task                                    */
                 (CPU_CHAR    *)"Button Reader",
                 (OS_TASK_PTR  )reader_task,
                 (void        *)0,
                 (OS_PRIO      )APP_BUTTON_READER_PRIO,
                 (CPU_STK     *)&reaer_stack[0],
                 (CPU_STK_SIZE )APP_BUTTON_READER_STK_SIZE/10,
                 (CPU_STK_SIZE )APP_BUTTON_READER_STK_SIZE,
                 (OS_MSG_QTY   )0u,
                 (OS_TICK      )0u,
                 (void        *)0,
                 (OS_OPT       )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR      *)&err);

}
static  void  reader_task   (void  *p_arg)
{
    uint32_t duty_cycle = 0;
    while(DEF_ON)
    {
        AclGyro.PrintMotion6Data();
        if( duty_cycle <= 100 )
        {
            PWM_chg_duty( PWM0, duty_cycle );
            duty_cycle++;
        }
        else
        {
            duty_cycle = 0;
        }
        // AclGyro.PrintOffsets();
        OS_ERR err;
        OSTimeDlyHMSM(0u, 0u, 0u, 500u,OS_OPT_TIME_HMSM_STRICT,&err);
    }
}
