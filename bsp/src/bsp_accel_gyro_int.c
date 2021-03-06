/*******************************************************************************
 * File:    bsp_accel_gyro_int.c
 * Author:  Mitchell S. Tilson
 * Created: 01/31/2019
 *
 * Description: This file provides the interrupt service routine for the accel/gyro
 * chip.
 *
 ******************************************************************************/

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <p32xxxx.h>
#include "bsp_accel_gyro_int.h"
#include <stdint.h>

/*******************************************************************************
 * Local Data
 ******************************************************************************/
#define TCB_LIST_LEN (1)
OS_TCB* tcb_list[TCB_LIST_LEN];
static uint32_t tcb_cnt = 0;

/*******************************************************************************
 * IntHanlder (called from bsp_a.S)
 *
 * Description:	Called when the interrupt happens (INT1, RE8 from the MPU6050)
 *
 * Inputs: none
 *
 * Revision: Initial Mitch Tilson 01/26/19
 *
 *
 * Notes: None
 *
 ******************************************************************************/
void bsp_accel_gyro_int( void )
{
    OS_ERR err;
    for( uint32_t tcb_idx = 0; tcb_idx < tcb_cnt; tcb_idx++ )
    {
        OSTaskSemPost(tcb_list[tcb_idx],OS_OPT_POST_NO_SCHED,&err);
    }
    IFS0bits.INT1IF = 0;
}

/*******************************************************************************
 * bsp_accel_gyro_int_en
 *
 * Description:	Sets up the microchip interrupts
 *
 * Inputs: none
 *
 * Revision: Initial Mitch Tilson 01/26/19
 *
 *
 * Notes: None
 *
 ******************************************************************************/
void bsp_accel_gyro_int_en( void )
{
    // INT is on pin 8 of port E
    // This configures it as an input
    TRISEbits.TRISE8 = 1;

    // INT1 enable bit (disable the interrupt)
    IEC0bits.INT1IE = 0;

    // Set the interrupt polarity for rising edge
    INTCONbits.INT1EP = 1;

    // Clear the interrupt status flag
    IFS0bits.INT1IF = 0;

    // Interrupt priority
    IPC1bits.INT1IP = 6;

    // Interrupt sub-priority
    IPC1bits.INT1IS = 0;

    // INT1 enable bit
    IEC0bits.INT1IE = 1;

}

/*******************************************************************************
 * bsp_accel_gyro_int_register
 *
 * Description:	Registers a task control block so that a task can be signaled
 *              when a sample is available.
 *
 * Inputs: OS_TCB* tcb - a pointer to a task control block
 *
 * Revision: Initial Mitch Tilson 01/28/19
 *
 *
 * Notes: None
 *
 ******************************************************************************/
void bsp_accel_gyro_int_register( OS_TCB* tcb )
{
    tcb_cnt++;
    if( tcb_cnt <= TCB_LIST_LEN )
    {
        tcb_list[tcb_cnt-1] = tcb;
    }
}
