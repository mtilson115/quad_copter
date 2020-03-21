/*******************************************************************************
 * File:    app_battery_read.cpp
 * Author:  Mitchell S. Tilson
 * Created: 09/01/2019
 *
 * Description: This file contains the bsp for reading the battery voltage
 *
 ******************************************************************************/

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <app_cfg.h>
#include <p32xxxx.h>
#include "bsp_utils.h"
#include "comms_xbee.h"
#include "driver_analog.h"

/*******************************************************************************
 * Local static data
 ******************************************************************************/
static OS_TCB battery_TCB;
CPU_STK battery_stack[APP_BUTTON_READER_STK_SIZE];
static void battery_task(void *p_arg);

/*******************************************************************************
 * battery_read_init
 *
 * Description: Initializes the analog circuits for reading the battery and starts
 *              a task to read the battery
 *
 * Inputs:      none
 *
 * Returns:     none
 *
 * Revision:    Initial Creation 09/01/2019 - Mitchell S. Tilson
 *
 * Notes:
 *
 ******************************************************************************/
void battery_read_init( void )
{
    OS_ERR err;
    OSTaskCreate((OS_TCB      *)&battery_TCB,      /* Create the start task */
                 (CPU_CHAR    *)"Battery Task",
                 (OS_TASK_PTR  )battery_task,
                 (void        *)0,
                 (OS_PRIO      )APP_BUTTON_READER_PRIO,
                 (CPU_STK     *)&battery_stack[0],
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

/*******************************************************************************
 * battery_task
 *
 * Description: Performs the reading of the battery voltage
 *
 * Inputs:      none
 *
 * Returns:     none
 *
 * Revision:    Initial Creation 09/01/2019 - Mitchell S. Tilson
 *
 * Notes:
 *
 ******************************************************************************/
uint32_t battery_task_call_cnt = 0;
static void battery_task(void  *p_arg)
{
    OS_ERR err;
    while( !comms_xbee_ready() )
    {
        OSTimeDlyHMSM(0u, 0u, 1u, 0u,OS_OPT_TIME_HMSM_STRICT,&err);
    }
    while(DEF_ON)
    {
        // Take a reading every 1s
        uint32_t pin = 7;
        int16_t bat_adc = driver_analog_get_10bit(pin);
        float voltage = (3.3)*(bat_adc/512.0);
        voltage = (voltage*(20.5*1000+4.99*1000))/(4.99*1000);

        // Create the message
        uint8_t msg_hdr = COMMS_BATTERY_VOLTAGE;
        uint8_t data_buff[sizeof(msg_hdr)+sizeof(float)] = {0};
        data_buff[0] = msg_hdr;
        memcpy(&data_buff[sizeof(msg_hdr)],&voltage,sizeof(float));

        // Send the message
        comms_xbee_msg_t msg;
        msg.data = data_buff;
        msg.len = sizeof(data_buff);
        comms_xbee_send(msg);
        // BSP_Printf("Battery = %f\n\r",voltage);

        // Delay 1s
        OSTimeDlyHMSM(0u, 0u, 1u, 0u,OS_OPT_TIME_HMSM_STRICT,&err);

        battery_task_call_cnt++;
    }
}
#ifdef	__cplusplus
extern "C" {
#endif
OS_TCB* app_battery_get_tcb( void )
{
    return &battery_TCB;
}
void app_battery_get_stack( CPU_STK* stk )
{
    stk = battery_stack;
}
#ifdef	__cplusplus
}
#endif
