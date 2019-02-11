/*
*********************************************************************************************************
*                                         Board Support Package
*
*                             (c) Copyright 2010, Micrium, Inc., Weston, FL
*                                           All Rights Reserved
*
*                                               Microchip
*                                      PIC32 Ethernet Starter Kit     
*
* File    : bsp_a.S
* Version : v1.00
*********************************************************************************************************
*/

    .section .text,code
    .set noreorder
    .set noat
    
/*
*********************************************************************************************************
*                           CONSTANTS USED TO ACCESS TASK CONTEXT STACK
*********************************************************************************************************
*/

.equ    STK_OFFSET_SR,      4
.equ    STK_OFFSET_EPC,     STK_OFFSET_SR    + 4
.equ    STK_OFFSET_LO,      STK_OFFSET_EPC   + 4
.equ    STK_OFFSET_HI,      STK_OFFSET_LO    + 4
.equ    STK_OFFSET_GPR1,    STK_OFFSET_HI    + 4
.equ    STK_OFFSET_GPR2,    STK_OFFSET_GPR1  + 4
.equ    STK_OFFSET_GPR3,    STK_OFFSET_GPR2  + 4
.equ    STK_OFFSET_GPR4,    STK_OFFSET_GPR3  + 4
.equ    STK_OFFSET_GPR5,    STK_OFFSET_GPR4  + 4
.equ    STK_OFFSET_GPR6,    STK_OFFSET_GPR5  + 4
.equ    STK_OFFSET_GPR7,    STK_OFFSET_GPR6  + 4
.equ    STK_OFFSET_GPR8,    STK_OFFSET_GPR7  + 4
.equ    STK_OFFSET_GPR9,    STK_OFFSET_GPR8  + 4
.equ    STK_OFFSET_GPR10,   STK_OFFSET_GPR9  + 4
.equ    STK_OFFSET_GPR11,   STK_OFFSET_GPR10 + 4
.equ    STK_OFFSET_GPR12,   STK_OFFSET_GPR11 + 4
.equ    STK_OFFSET_GPR13,   STK_OFFSET_GPR12 + 4
.equ    STK_OFFSET_GPR14,   STK_OFFSET_GPR13 + 4
.equ    STK_OFFSET_GPR15,   STK_OFFSET_GPR14 + 4
.equ    STK_OFFSET_GPR16,   STK_OFFSET_GPR15 + 4
.equ    STK_OFFSET_GPR17,   STK_OFFSET_GPR16 + 4
.equ    STK_OFFSET_GPR18,   STK_OFFSET_GPR17 + 4
.equ    STK_OFFSET_GPR19,   STK_OFFSET_GPR18 + 4
.equ    STK_OFFSET_GPR20,   STK_OFFSET_GPR19 + 4
.equ    STK_OFFSET_GPR21,   STK_OFFSET_GPR20 + 4
.equ    STK_OFFSET_GPR22,   STK_OFFSET_GPR21 + 4
.equ    STK_OFFSET_GPR23,   STK_OFFSET_GPR22 + 4
.equ    STK_OFFSET_GPR24,   STK_OFFSET_GPR23 + 4
.equ    STK_OFFSET_GPR25,   STK_OFFSET_GPR24 + 4
.equ    STK_OFFSET_GPR26,   STK_OFFSET_GPR25 + 4
.equ    STK_OFFSET_GPR27,   STK_OFFSET_GPR26 + 4
.equ    STK_OFFSET_GPR28,   STK_OFFSET_GPR27 + 4
.equ    STK_OFFSET_GPR30,   STK_OFFSET_GPR28 + 4
.equ    STK_OFFSET_GPR31,   STK_OFFSET_GPR30 + 4 
.equ    STK_CTX_SIZE,       STK_OFFSET_GPR31 + 4


.MACRO   ISR_PROLOGUE

    addi  $29, $29, -STK_CTX_SIZE                          

    sw    $1,  STK_OFFSET_GPR1($29)      
    sw    $2,  STK_OFFSET_GPR2($29)
    sw    $3,  STK_OFFSET_GPR3($29)
    sw    $4,  STK_OFFSET_GPR4($29)
    sw    $5,  STK_OFFSET_GPR5($29)
    sw    $6,  STK_OFFSET_GPR6($29)
    sw    $7,  STK_OFFSET_GPR7($29)
    sw    $8,  STK_OFFSET_GPR8($29)
    sw    $9,  STK_OFFSET_GPR9($29)
    sw    $10, STK_OFFSET_GPR10($29)
    sw    $11, STK_OFFSET_GPR11($29)
    sw    $12, STK_OFFSET_GPR12($29)
    sw    $13, STK_OFFSET_GPR13($29)
    sw    $14, STK_OFFSET_GPR14($29)
    sw    $15, STK_OFFSET_GPR15($29)
    sw    $16, STK_OFFSET_GPR16($29)
    sw    $17, STK_OFFSET_GPR17($29)
    sw    $18, STK_OFFSET_GPR18($29)
    sw    $19, STK_OFFSET_GPR19($29)
    sw    $20, STK_OFFSET_GPR20($29)
    sw    $21, STK_OFFSET_GPR21($29)
    sw    $22, STK_OFFSET_GPR22($29)
    sw    $23, STK_OFFSET_GPR23($29)
    sw    $24, STK_OFFSET_GPR24($29)
    sw    $25, STK_OFFSET_GPR25($29)
    sw    $26, STK_OFFSET_GPR26($29)
    sw    $27, STK_OFFSET_GPR27($29)
    sw    $28, STK_OFFSET_GPR28($29)
    sw    $30, STK_OFFSET_GPR30($29)
    sw    $31, STK_OFFSET_GPR31($29)
                                              
    mflo  $8
    mfhi  $9
    sw    $8,  STK_OFFSET_LO($29)
    sw    $9,  STK_OFFSET_HI($29)

    mfc0  $8,  $14, 0                     
    sw    $8,  STK_OFFSET_EPC($29)
  
    mfc0  $8,  $12, 0
    sw    $8,  STK_OFFSET_SR($29)

    la    $8,  OSIntNestingCtr
    lbu   $9,  0($8) 
    bne   $0,  $9, 1f
    nop   

    la    $10, OSTCBCurPtr
    lw    $11, 0($10)
    sw    $29, 0($11)
  
1:

    addi  $9,  $9, 1                        
    sb    $9,  0($8)

    mfc0  $8, $12, 0                          
    mfc0  $9, $13, 0
    andi  $9, $9,  0xFC00
    ins   $8, $0,  10, 6
    or    $8, $8,  $9
    mtc0  $8, $12, 0

.ENDM

.MACRO  ISR_EPILOGUE

    la    $8,  OSIntExit              
    jalr  $8
    nop

    lw    $8,  STK_OFFSET_SR($29)     
    mtc0  $8,  $12, 0

    lw    $8,  STK_OFFSET_EPC($29)      
    mtc0  $8,  $14, 0

    lw    $8,  STK_OFFSET_LO($29)         
    lw    $9,  STK_OFFSET_HI($29)
    mtlo  $8
    mthi  $9

    lw    $31, STK_OFFSET_GPR31($29)        
    lw    $30, STK_OFFSET_GPR30($29)
    lw    $28, STK_OFFSET_GPR28($29)
    lw    $27, STK_OFFSET_GPR27($29)
    lw    $26, STK_OFFSET_GPR26($29)
    lw    $25, STK_OFFSET_GPR25($29)
    lw    $24, STK_OFFSET_GPR24($29)
    lw    $23, STK_OFFSET_GPR23($29)
    lw    $22, STK_OFFSET_GPR22($29)
    lw    $21, STK_OFFSET_GPR21($29)
    lw    $20, STK_OFFSET_GPR20($29)
    lw    $19, STK_OFFSET_GPR19($29)
    lw    $18, STK_OFFSET_GPR18($29)
    lw    $17, STK_OFFSET_GPR17($29)
    lw    $16, STK_OFFSET_GPR16($29)
    lw    $15, STK_OFFSET_GPR15($29)
    lw    $14, STK_OFFSET_GPR14($29)
    lw    $13, STK_OFFSET_GPR13($29)
    lw    $12, STK_OFFSET_GPR12($29)
    lw    $11, STK_OFFSET_GPR11($29)
    lw    $10, STK_OFFSET_GPR10($29)
    lw    $9,  STK_OFFSET_GPR9($29)
    lw    $8,  STK_OFFSET_GPR8($29)
    lw    $7,  STK_OFFSET_GPR7($29)
    lw    $6,  STK_OFFSET_GPR6($29)
    lw    $5,  STK_OFFSET_GPR5($29)
    lw    $4,  STK_OFFSET_GPR4($29)
    lw    $3,  STK_OFFSET_GPR3($29)
    lw    $2,  STK_OFFSET_GPR2($29)
    lw    $1,  STK_OFFSET_GPR1($29)

    addi  $29, $29, STK_CTX_SIZE           

    eret     

.ENDM


/*
*********************************************************************************************************
*                                  Vector 1  Core Software Interrupt 0
*********************************************************************************************************
*/

    .section .vector_1,code
    .set noreorder
    .ent _bsp_cs0int

_bsp_cs0int:
    la  $26,BSP_CS0ISR
    jr  $26 
    nop
    
    .end _bsp_cs0int
    
    .section .bsp_cs0isr,code
    .ent BSP_CS0ISR
BSP_CS0ISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_CS0ISR

/*
*********************************************************************************************************
*                                  Vector 2  Core Software Interrupt 1
*********************************************************************************************************
*/

    .section .vector_2,code
    .set noreorder
    .ent _bsp_cs1int

_bsp_cs1int:
    la  $26,BSP_CS1ISR
    jr  $26 
    nop
    
    .end _bsp_cs1int
    
    .section .bsp_cs1isr,code
    .ent BSP_CS1ISR
BSP_CS1ISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_CS1ISR

/*
*********************************************************************************************************
*                                  Vector 3  External Interrupt 0
*********************************************************************************************************
*/

    .section .vector_3,code
    .set noreorder
    .ent _bsp_ext0int

_bsp_ext0int:
    la  $26,BSP_EXT0ISR
    jr  $26 
    nop
    
    .end _bsp_ext0int
    
    .section .bsp_ext0isr,code
    .ent BSP_EXT0ISR
BSP_EXT0ISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_EXT0ISR

/*
*********************************************************************************************************
*                                  Vector 4  Timer1
*********************************************************************************************************
*/

    .section .vector_4,code
    .set noreorder
    .ent _bsp_t1int

_bsp_t1int:
    la  $26,BSP_T1ISR
    jr  $26 
    nop
    
    .end _bsp_t1int
    
    .section .bsp_t1isr,code
    .ent BSP_T1ISR
BSP_T1ISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_T1ISR

/*
*********************************************************************************************************
*                                  Vector 5  Input Capture 1
*********************************************************************************************************
*/

    .section .vector_5,code
    .set noreorder
    .ent _bsp_ic1int

_bsp_ic1int:
    la  $26,BSP_IC1ISR
    jr  $26 
    nop
    
    .end _bsp_ic1int
    
    .section .bsp_ic1isr,code
    .ent BSP_IC1ISR
BSP_IC1ISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_IC1ISR

/*
*********************************************************************************************************
*                                  Vector 6  Output Compare 1
*********************************************************************************************************
*/

    .section .vector_6,code
    .set noreorder
    .ent _bsp_oc1int

_bsp_oc1int:
    la  $26,BSP_OC1ISR
    jr  $26 
    nop
    
    .end _bsp_oc1int
    
    .section .bsp_oc1isr,code
    .ent BSP_OC1ISR
BSP_OC1ISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_OC1ISR

/*
*********************************************************************************************************
*                                  Vector 7  External Interrupt 1
*********************************************************************************************************
*/

    .section .vector_7,code
    .set noreorder
    .ent _bsp_ext1int

_bsp_ext1int:
    la  $26,BSP_EXT1ISR
    jr  $26 
    nop
    
    .end _bsp_ext1int
    
    .section .bsp_ext1isr,code
    .ent BSP_EXT1ISR
BSP_EXT1ISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler           /* Call AccelGyroIntHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_EXT1ISR

/*
*********************************************************************************************************
*                                  Vector 8  Timer2
*********************************************************************************************************
*/

    .section .vector_8,code
    .set noreorder
    .ent _bsp_t2int

_bsp_t2int:
    la  $26,BSP_T2ISR
    jr  $26 
    nop
    
    .end _bsp_t2int
    
    .section .bsp_t2isr,code
    .ent BSP_T2ISR
BSP_T2ISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_T2ISR

/*
*********************************************************************************************************
*                                  Vector 9  Input Capture 2
*********************************************************************************************************
*/

    .section .vector_9,code
    .set noreorder
    .ent _bsp_ic2int

_bsp_ic2int:
    la  $26,BSP_IC2ISR
    jr  $26 
    nop
    
    .end _bsp_ic2int
    
    .section .bsp_ic2isr,code
    .ent BSP_IC2ISR
BSP_IC2ISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_IC2ISR

/*
*********************************************************************************************************
*                                  Vector 10  Output Compare 2
*********************************************************************************************************
*/

    .section .vector_10,code
    .set noreorder
    .ent _bsp_oc2int

_bsp_oc2int:
    la  $26,BSP_OC2ISR
    jr  $26 
    nop
    
    .end _bsp_oc2int
    
    .section .bsp_oc2isr,code
    .ent BSP_OC2ISR
BSP_OC2ISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_OC2ISR

/*
*********************************************************************************************************
*                                  Vector 11  External Interrupt 2
*********************************************************************************************************
*/

    .section .vector_11,code
    .set noreorder
    .ent _bsp_ext2int

_bsp_ext2int:
    la  $26,BSP_EXT2ISR
    jr  $26 
    nop
    
    .end _bsp_ext2int
    
    .section .bsp_ext2isr,code
    .ent BSP_EXT2ISR
BSP_EXT2ISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler       /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_EXT2ISR

/*
*********************************************************************************************************
*                                  Vector 12  Timer3
*********************************************************************************************************
*/

    .section .vector_12,code
    .set noreorder
    .ent _bsp_t3int

_bsp_t3int:
    la  $26,BSP_T3ISR
    jr  $26 
    nop
    
    .end _bsp_t3int
    
    .section .bsp_t3isr,code
    .ent BSP_T3ISR
BSP_T3ISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_T3ISR

/*
*********************************************************************************************************
*                                  Vector 13  Input Capture 3
*********************************************************************************************************
*/

    .section .vector_13,code
    .set noreorder
    .ent _bsp_ic3int

_bsp_ic3int:
    la  $26,BSP_IC3ISR
    jr  $26 
    nop
    
    .end _bsp_ic3int
    
    .section .bsp_ic3isr,code
    .ent BSP_IC3ISR
BSP_IC3ISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_IC3ISR

/*
*********************************************************************************************************
*                                  Vector 14  Output Compare 3
*********************************************************************************************************
*/

    .section .vector_14,code
    .set noreorder
    .ent _bsp_oc3int

_bsp_oc3int:
    la  $26,BSP_OC3ISR
    jr  $26 
    nop
    
    .end _bsp_oc3int
    
    .section .bsp_oc3isr,code
    .ent BSP_OC3ISR
BSP_OC3ISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_OC3ISR

/*
*********************************************************************************************************
*                                  Vector 15  External Interrupt 3
*********************************************************************************************************
*/

    .section .vector_15,code
    .set noreorder
    .ent _bsp_ext3int

_bsp_ext3int:
    la  $26,BSP_EXT3ISR
    jr  $26 
    nop
    
    .end _bsp_ext3int
    
    .section .bsp_ext3isr,code
    .ent BSP_EXT3ISR
BSP_EXT3ISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_EXT3ISR

/*
*********************************************************************************************************
*                                  Vector 16  Timer4
*********************************************************************************************************
*/

    .section .vector_16,code
    .set noreorder
    .ent _bsp_t4int

_bsp_t4int:
    la  $26,BSP_T4ISR
    jr  $26 
    nop
    
    .end _bsp_t4int
    
    .section .bsp_t4isr,code
    .ent BSP_T4ISR
BSP_T4ISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_T4ISR

/*
*********************************************************************************************************
*                                  Vector 17  Input Capture 4
*********************************************************************************************************
*/

    .section .vector_17,code
    .set noreorder
    .ent _bsp_ic4int

_bsp_ic4int:
    la  $26,BSP_IC4ISR
    jr  $26 
    nop
    
    .end _bsp_ic4int
    
    .section .bsp_ic4isr,code
    .ent BSP_IC4ISR
BSP_IC4ISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_IC4ISR

/*
*********************************************************************************************************
*                                  Vector 18  Output Compare 4
*********************************************************************************************************
*/

    .section .vector_18,code
    .set noreorder
    .ent _bsp_oc4int

_bsp_oc4int:
    la  $26,BSP_OC4ISR
    jr  $26 
    nop
    
    .end _bsp_oc4int
    
    .section .bsp_oc4isr,code
    .ent BSP_OC4ISR
BSP_OC4ISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_OC4ISR

/*
*********************************************************************************************************
*                                  Vector 19  External Interrupt 4
*********************************************************************************************************
*/

    .section .vector_19,code
    .set noreorder
    .ent _bsp_ext4int

_bsp_ext4int:
    la  $26,BSP_EXT4ISR
    jr  $26 
    nop
    
    .end _bsp_ext4int
    
    .section .bsp_ext4isr,code
    .ent BSP_EXT4ISR
BSP_EXT4ISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_EXT4ISR

/*
*********************************************************************************************************
*                                  Vector 20  Timer5
*********************************************************************************************************
*/

    .section .vector_20,code
    .set noreorder
    .ent _bsp_t5int

_bsp_t5int:
    la  $26,BSP_T5ISR
    jr  $26 
    nop
    
    .end _bsp_t5int
    
    .section .bsp_t5isr,code
    .ent BSP_T5ISR
BSP_T5ISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_T5ISR

/*
*********************************************************************************************************
*                                  Vector 21  Input Capture 5
*********************************************************************************************************
*/

    .section .vector_21,code
    .set noreorder
    .ent _bsp_ic5int

_bsp_ic5int:
    la  $26,BSP_IC5ISR
    jr  $26 
    nop
    
    .end _bsp_ic5int
    
    .section .bsp_ic5isr,code
    .ent BSP_IC5ISR
BSP_IC5ISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_IC5ISR

/*
*********************************************************************************************************
*                                  Vector 22  Output Compare 5
*********************************************************************************************************
*/

    .section .vector_22,code
    .set noreorder
    .ent _bsp_oc5int

_bsp_oc5int:
    la  $26,BSP_OC5ISR
    jr  $26 
    nop
    
    .end _bsp_oc5int
    
    .section .bsp_oc5isr,code
    .ent BSP_OC5ISR
BSP_OC5ISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_OC5ISR

/*
*********************************************************************************************************
*                                  Vector 23  SPI1
*********************************************************************************************************
*/

    .section .vector_23,code
    .set noreorder
    .ent _bsp_spi1int

_bsp_spi1int:
    la  $26,BSP_SPI1ISR
    jr  $26 
    nop
    
    .end _bsp_spi1int
    
    .section .bsp_spi1isr,code
    .ent BSP_SPI1ISR
BSP_SPI1ISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_SPI1ISR

/*
*********************************************************************************************************
*                                  Vector 24  UART1A
*********************************************************************************************************
*/

    .section .vector_24,code
    .set noreorder
    .ent _bsp_uart1aint

_bsp_uart1aint:
    la  $26,BSP_UART1AISR
    jr  $26 
    nop
    
    .end _bsp_uart1aint
    
    .section .bsp_uart1aisr,code
    .ent BSP_UART1AISR
BSP_UART1AISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_UART1AISR

/*
*********************************************************************************************************
*                                  Vector 25  I2C1
*********************************************************************************************************
*/

    .section .vector_25,code
    .set noreorder
    .ent _bsp_i2c1int

_bsp_i2c1int:
    la  $26,BSP_I2C1ISR
    jr  $26 
    nop
    
    .end _bsp_i2c1int
    
    .section .bsp_i2c1isr,code
    .ent BSP_I2C1ISR
BSP_I2C1ISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_I2C1ISR

/*
*********************************************************************************************************
*                                  Vector 26  Change Notice
*********************************************************************************************************
*/

    .section .vector_26,code
    .set noreorder
    .ent _bsp_cnint

_bsp_cnint:
    la  $26,BSP_CNISR
    jr  $26 
    nop
    
    .end _bsp_cnint
    
    .section .bsp_cnisr,code
    .ent BSP_CNISR
BSP_CNISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler              
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_CNISR

/*
*********************************************************************************************************
*                                  Vector 27  ADC1
*********************************************************************************************************
*/

    .section .vector_27,code
    .set noreorder
    .ent _bsp_ad1int

_bsp_ad1int:
    la  $26,BSP_AD1ISR
    jr  $26 
    nop
    
    .end _bsp_ad1int
    
    .section .bsp_ad1isr,code
    .ent BSP_AD1ISR
BSP_AD1ISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_AD1ISR

/*
*********************************************************************************************************
*                                  Vector 28  Parallel Master Port
*********************************************************************************************************
*/

    .section .vector_28,code
    .set noreorder
    .ent _bsp_pmpint

_bsp_pmpint:
    la  $26,BSP_PMPISR
    jr  $26 
    nop
    
    .end _bsp_pmpint
    
    .section .bsp_pmpisr,code
    .ent BSP_PMPISR
BSP_PMPISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_PMPISR

/*
*********************************************************************************************************
*                                  Vector 29  Comparator Interrupt 1
*********************************************************************************************************
*/

    .section .vector_29,code
    .set noreorder
    .ent _bsp_cmp1int

_bsp_cmp1int:
    la  $26,BSP_CMP1ISR
    jr  $26 
    nop
    
    .end _bsp_cmp1int
    
    .section .bsp_cmp1isr,code
    .ent BSP_CMP1ISR
BSP_CMP1ISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_CMP1ISR

/*
*********************************************************************************************************
*                                  Vector 30  Comparator Interrupt 2
*********************************************************************************************************
*/

    .section .vector_30,code
    .set noreorder
    .ent _bsp_cmp2int

_bsp_cmp2int:
    la  $26,BSP_CMP2ISR
    jr  $26 
    nop
    
    .end _bsp_cmp2int
    
    .section .bsp_cmp2isr,code
    .ent BSP_CMP2ISR
BSP_CMP2ISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_CMP2ISR

/*
*********************************************************************************************************
*                                  Vector 31  UART2A
*********************************************************************************************************
*/

    .section .vector_31,code
    .set noreorder
    .ent _bsp_uart2aint

_bsp_uart2aint:
    la  $26,BSP_UART2AISR
    jr  $26 
    nop
    
    .end _bsp_uart2aint
    
    .section .bsp_uart2aisr,code
    .ent BSP_UART2AISR
BSP_UART2AISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_UART2AISR

/*
*********************************************************************************************************
*                                  Vector 32  UART3A
*********************************************************************************************************
*/

    .section .vector_32,code
    .set noreorder
    .ent _bsp_uart3aint

_bsp_uart3aint:
    la  $26,BSP_UART3AISR
    jr  $26 
    nop
    
    .end _bsp_uart3aint
    
    .section .bsp_uart3aisr,code
    .ent BSP_UART3AISR
BSP_UART3AISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_UART3AISR

/*
*********************************************************************************************************
*                                  Vector 33  I2C2
*********************************************************************************************************
*/

    .section .vector_33,code
    .set noreorder
    .ent _bsp_i2c2int

_bsp_i2c2int:
    la  $26,BSP_I2C2ISR
    jr  $26 
    nop
    
    .end _bsp_i2c2int
    
    .section .bsp_i2c2isr,code
    .ent BSP_I2C2ISR
BSP_I2C2ISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_I2C2ISR

/*
*********************************************************************************************************
*                                  Vector 34  Fail-Safe Clock Monitor
*********************************************************************************************************
*/

    .section .vector_34,code
    .set noreorder
    .ent _bsp_fscmint

_bsp_fscmint:
    la  $26,BSP_FSCMISR
    jr  $26 
    nop
    
    .end _bsp_fscmint
    
    .section .bsp_fscmisr,code
    .ent BSP_FSCMISR
BSP_FSCMISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_FSCMISR

/*
*********************************************************************************************************
*                                  Vector 35  Real-Time Clock
*********************************************************************************************************
*/

    .section .vector_35,code
    .set noreorder
    .ent _bsp_rtccint

_bsp_rtccint:
    la  $26,BSP_RTCCISR
    jr  $26 
    nop
    
    .end _bsp_rtccint
    
    .section .bsp_rtccisr,code
    .ent BSP_RTCCISR
BSP_RTCCISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_RTCCISR

/*
*********************************************************************************************************
*                                  Vector 36  DMA Channel 0
*********************************************************************************************************
*/

    .section .vector_36,code
    .set noreorder
    .ent _bsp_dma0int

_bsp_dma0int:
    la  $26,BSP_DMA0ISR
    jr  $26 
    nop
    
    .end _bsp_dma0int
    
    .section .bsp_dma0isr,code
    .ent BSP_DMA0ISR
BSP_DMA0ISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_DMA0ISR

/*
*********************************************************************************************************
*                                  Vector 37  DMA Channel 1
*********************************************************************************************************
*/

    .section .vector_37,code
    .set noreorder
    .ent _bsp_dma1int

_bsp_dma1int:
    la  $26,BSP_DMA1ISR
    jr  $26 
    nop
    
    .end _bsp_dma1int
    
    .section .bsp_dma1isr,code
    .ent BSP_DMA1ISR
BSP_DMA1ISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_DMA1ISR

/*
*********************************************************************************************************
*                                  Vector 38  DMA Channel 2
*********************************************************************************************************
*/

    .section .vector_38,code
    .set noreorder
    .ent _bsp_dma2int

_bsp_dma2int:
    la  $26,BSP_CNISR
    jr  $26 
    nop
    
    .end _bsp_dma2int
    
    .section .bsp_dma2isr,code
    .ent BSP_DMA2ISR
BSP_DMA2ISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_DMA2ISR

/*
*********************************************************************************************************
*                                  Vector 39  DMA Channel 3
*********************************************************************************************************
*/

    .section .vector_39,code
    .set noreorder
    .ent _bsp_dma3int

_bsp_dma3int:
    la  $26,BSP_DMA3ISR
    jr  $26 
    nop
    
    .end _bsp_dma3int
    
    .section .bsp_dma3isr,code
    .ent BSP_DMA3ISR
BSP_DMA3ISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_DMA3ISR

/*
*********************************************************************************************************
*                                  Vector 40  DMA Channel 4
*********************************************************************************************************
*/

    .section .vector_40,code
    .set noreorder
    .ent _bsp_dma4int

_bsp_dma4int:
    la  $26,BSP_DMA4ISR
    jr  $26 
    nop
    
    .end _bsp_dma4int
    
    .section .bsp_dma4isr,code
    .ent BSP_DMA4ISR
BSP_DMA4ISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_DMA4ISR

/*
*********************************************************************************************************
*                                  Vector 41  DMA Channel 5
*********************************************************************************************************
*/

    .section .vector_41,code
    .set noreorder
    .ent _bsp_dma5int

_bsp_dma5int:
    la  $26,BSP_DMA5ISR
    jr  $26 
    nop
    
    .end _bsp_dma5int
    
    .section .bsp_dma5isr,code
    .ent BSP_DMA5ISR
BSP_DMA5ISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_DMA5ISR

/*
*********************************************************************************************************
*                                  Vector 42  DMA Channel 6
*********************************************************************************************************
*/

    .section .vector_42,code
    .set noreorder
    .ent _bsp_dma6int

_bsp_dma6int:
    la  $26,BSP_DMA6ISR
    jr  $26 
    nop
    
    .end _bsp_dma6int
    
    .section .bsp_dma6isr,code
    .ent BSP_DMA6ISR
BSP_DMA6ISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_DMA6ISR

/*
*********************************************************************************************************
*                                  Vector 43  DMA Channel 7
*********************************************************************************************************
*/

    .section .vector_43,code
    .set noreorder
    .ent _bsp_dma7int

_bsp_dma7int:
    la  $26,BSP_DMA7ISR
    jr  $26 
    nop
    
    .end _bsp_dma7int
    
    .section .bsp_dma6isr,code
    .ent BSP_DMA7ISR
BSP_DMA7ISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_DMA7ISR

/*
*********************************************************************************************************
*                                  Vector 44  Flash Control Event
*********************************************************************************************************
*/

    .section .vector_44,code
    .set noreorder
    .ent _bsp_fceint

_bsp_fceint:
    la  $26,BSP_FCEISR
    jr  $26 
    nop
    
    .end _bsp_fceint
    
    .section .bsp_fceisr,code
    .ent BSP_FCEISR
BSP_FCEISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_FCEISR

/*
*********************************************************************************************************
*                                  Vector 45  USB
*********************************************************************************************************
*/

    .section .vector_45,code
    .set noreorder
    .ent _bsp_usbint

_bsp_usbint:
    la  $26,BSP_USBISR
    jr  $26 
    nop
    
    .end _bsp_usbint
    
    .section .bsp_usbisr,code
    .ent BSP_USBISR
BSP_USBISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_USBISR

/*
*********************************************************************************************************
*                                  Vector 46  CAN1
*********************************************************************************************************
*/

    .section .vector_46,code
    .set noreorder
    .ent _bsp_can1int

_bsp_can1int:
    la  $26,BSP_CAN1ISR
    jr  $26 
    nop
    
    .end _bsp_can1int
    
    .section .bsp_can1isr,code
    .ent BSP_CAN1ISR
BSP_CAN1ISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_CAN1ISR

/*
*********************************************************************************************************
*                                  Vector 47  CAN2
*********************************************************************************************************
*/

    .section .vector_47,code
    .set noreorder
    .ent _bsp_can2int

_bsp_can2int:
    la  $26,BSP_CAN2ISR
    jr  $26 
    nop
    
    .end _bsp_can2int
    
    .section .bsp_can2isr,code
    .ent BSP_CAN2ISR
BSP_CAN2ISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_CAN2ISR

/*
*********************************************************************************************************
*                                  Vector 48  Ethernet
*********************************************************************************************************
*/

    .section .vector_48,code
    .set noreorder
    .ent _bsp_ethint

_bsp_ethint:
    la  $26,BSP_ETHISR
    jr  $26 
    nop
    
    .end _bsp_ethint
    
    .section .bsp_ethisr,code
    .ent BSP_ETHISR
BSP_ETHISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_ETHISR

/*
*********************************************************************************************************
*                                  Vector 49  UART1B
*********************************************************************************************************
*/

    .section .vector_49,code
    .set noreorder
    .ent _bsp_uart1bint

_bsp_uart1bint:
    la  $26,BSP_UART1BISR
    jr  $26 
    nop
    
    .end _bsp_uart1bint
    
    .section .bsp_uart1bisr,code
    .ent BSP_UART1BISR
BSP_UART1BISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_UART1BISR

/*
*********************************************************************************************************
*                                  Vector 50  UART2B
*********************************************************************************************************
*/

    .section .vector_50,code
    .set noreorder
    .ent _bsp_uart2bint

_bsp_uart2bint:
    la  $26,BSP_UART2BISR
    jr  $26 
    nop
    
    .end _bsp_uart2bint
    
    .section .bsp_uart2bisr,code
    .ent BSP_UART2BISR
BSP_UART2BISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_UART2BISR

/*
*********************************************************************************************************
*                                  Vector 51  UART3B
*********************************************************************************************************
*/

    .section .vector_51,code
    .set noreorder
    .ent _bsp_uart3bint

_bsp_uart3bint:
    la  $26,BSP_UART3BISR
    jr  $26 
    nop
    
    .end _bsp_uart3bint
    
    .section .bsp_uart3bisr,code
    .ent BSP_UART3BISR
BSP_UART3BISR:

    ISR_PROLOGUE

    la    $8,  BSP_DefaultHandler            /* Call BSP_DefaultHandler() to handle the interrupt  */
    jalr  $8
    nop

    ISR_EPILOGUE    

    .end BSP_UART3BISR

