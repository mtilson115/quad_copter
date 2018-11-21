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

static  OS_TCB    reader_TCB;
static  CPU_STK   reaer_stack[APP_BUTTON_READER_STK_SIZE];
static  void  reader_task   (void  *p_arg);
#define LOW_DUTY 4.0
#define MAX_DUTY 9.0
void start_reader( void )
{
    // Initialize the AccelGyro
/*    AclGyro.Init();
 */

    // Initialize the pwm driver
    pwm_init_t init_settings = {
        25000,      // (80e6/64)/25000 = 50Hz (80e6 == clock source, 64 == pre-scale, 25000 == num ticks before roll over)
        LOW_DUTY,
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
    float duty_cycle = LOW_DUTY;
    OS_ERR err;
    OSTimeDlyHMSM(0u, 0u, 30u, 0u,OS_OPT_TIME_HMSM_STRICT,&err);
    while(DEF_ON)
    {
        //AclGyro.PrintMotion6Data();
        if( duty_cycle <= MAX_DUTY )
        {
            PWM_chg_duty( PWM0, duty_cycle );
            duty_cycle += 0.1;
        }
        else
        {
            duty_cycle = LOW_DUTY;
        }
        // AclGyro.PrintOffsets();
        LED_On(3);
        OSTimeDlyHMSM(0u, 0u, 0u, 250u,OS_OPT_TIME_HMSM_STRICT,&err);
        LED_Off(3);
        OSTimeDlyHMSM(0u, 0u, 0u, 250u,OS_OPT_TIME_HMSM_STRICT,&err);
    }
}
