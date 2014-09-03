/*
*********************************************************************************************************
*                                                uC/CPU
*                                    CPU CONFIGURATION & PORT LAYER
*
*                          (c) Copyright 2004-2011; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*
*               uC/CPU is provided in source form to registered licensees ONLY.  It is 
*               illegal to distribute this source code to any third party unless you receive 
*               written permission by an authorized Micrium representative.  Knowledge of 
*               the source code may NOT be used to develop a similar product.
*
*               Please help us continue to provide the Embedded community with the finest 
*               software available.  Your honesty is greatly appreciated.
*
*               You can contact us at www.micrium.com.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                            CPU PORT FILE
*
*                                              MIPS32 4K
*                                                MPLAB
*
* Filename      : cpu_a.s
* Version       : V1.28.02.00
* Programmer(s) : MG
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                           PUBLIC FUNCTIONS
*********************************************************************************************************
*/

    .global  CPU_SR_Save
    .global  CPU_SR_Restore

/*
*********************************************************************************************************
*                                      CODE GENERATION DIRECTIVES
*********************************************************************************************************
*/

    .section .text,code

/*
**********************************************************************************************************
*                                      CRITICAL SECTION FUNCTIONS
*
* Description : Disable/Enable interrupts by preserving the state of interrupts.  Generally speaking, the
*               state of the interrupt disable flag is stored in the local variable 'cpu_sr' & interrupts
*               are then disabled ('cpu_sr' is allocated in all functions that need to disable interrupts).
*               The previous interrupt state is restored by copying 'cpu_sr' into the CPU's status register.
*
* Prototypes  : CPU_SR  CPU_SR_Save(void);
*               void    CPU_SR_Restore(CPU_SR cpu_sr);
*
* Note(s)     : (1) These functions are used in general like this :
*
*                       void  Task (void  *p_arg)
*                       {
*                           CPU_SR_ALLOC();               
*                               :
*                               :
*                           CPU_CRITICAL_ENTER();           
*                               :
*                               :
*                           CPU_CRITICAL_EXIT();              
*                               :
*                       }
**********************************************************************************************************
*/

    .ent CPU_SR_Save
CPU_SR_Save:

    jr    $31
    di    $2                                   /* Disable interrupts, and move the old value of the... */
                                               /* ...Status register into v0 ($2)                      */
    .end CPU_SR_Save


    .ent CPU_SR_Restore
CPU_SR_Restore:

    jr    $31
    mtc0  $4, $12, 0                           /* Restore the status register to its previous state    */

    .end CPU_SR_Restore

