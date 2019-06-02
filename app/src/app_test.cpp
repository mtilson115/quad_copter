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
#include "driver_analog.h"

static  OS_TCB    test_TCB;
CPU_STK   test_stack[APP_BUTTON_READER_STK_SIZE];
static  void  test_task   (void  *p_arg);
void start_test( void )
{
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

    // Initialize the analog pin attached to the battery
    uint32_t pb = 1;
    uint32_t pin = 7;
    driver_analog_init_pin( pb, pin );
    driver_analog_init();
}

static void test_task(void  *p_arg)
{
    OS_ERR err;
    BSP_PrintfInit();
    while(DEF_ON)
    {
        // Take a reading every 1s
        int16_t bat_adc = driver_analog_get_10bit(7);
        float voltage = (3.3)*(bat_adc/512.0);
        voltage = (voltage*(10*1000+2.8*1000))/(2.8*1000);

        // Create the message
        uint8_t msg_hdr = 20;
        uint8_t data_buff[sizeof(msg_hdr)+sizeof(float)] = {0};
        data_buff[0] = msg_hdr;
        memcpy(&data_buff[sizeof(msg_hdr)],&voltage,sizeof(float));

        // Send the message
        comms_xbee_msg_t msg;
        msg.data = data_buff;
        msg.len = sizeof(data_buff);
        COMMS_xbee_send(msg);

        // Delay 1s
        OSTimeDlyHMSM(0u, 0u, 1u, 0u,OS_OPT_TIME_HMSM_STRICT,&err);
    }
}
#ifdef	__cplusplus
extern "C" {
#endif
OS_TCB* app_test_get_tcb( void )
{
    return &test_TCB;
}
void app_test_get_stack( CPU_STK* stk )
{
    stk = test_stack;
}
#ifdef	__cplusplus
}
#endif
