#include <os.h>
#include <app_cfg.h>
#include <p32xxxx.h>
#include <string.h>
#include "driver_uart.h"
#include "type_defs.h"
#include "driver_i2c.h"

UART_ID id;
static  OS_TCB    reader_TCB;
static  CPU_STK   reaer_stack[APP_BUTTON_READER_STK_SIZE];
static  void  reader_task   (void  *p_arg);
void start_reader( void )
{
		// Initialize the I2C 1
		I2C_ID id;
		uint8_t byte;
		I2C_MSG msg;
		msg.dev_addr = 0x55;
		msg.reg_addr = 0x11;
		msg.dir = 0x00;
		msg.data = &byte;
		msg.count = 1;
		byte = 0xAA;
		id = I2C_init( I2C_1, 400e3 );
		I2C_enable(id);
		
    OS_ERR err;
    OSTaskCreate((OS_TCB      *)&reader_TCB,                        /* Create the start task                                    */
                 (CPU_CHAR    *)"Button Reader",
                 (OS_TASK_PTR  )reader_task,
                 (void        *)0,
                 (OS_PRIO      )APP_CFG_TASK_START_PRIO,
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
    while(DEF_ON)
    {
				// Send the I2C byte for testing
				I2C_communicate(id,&msg);
				
        if( PORTDbits.RD6 == 0 )
        {
            // PORTDINV = 0x1;
            /*PORTFbits.RF8 = b;
            if( b == 1 ) {
                b = 0;
            } else {
                b = 1;
            }*/
        }
        OS_ERR err;
        OSTimeDlyHMSM(0u, 0u, 0u, 500u,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);
    }
}