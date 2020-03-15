/*
 * File:   cfg.h
 * Author: Mitch Tilson
 *
 * Created on 8/1/2019
 */


/****************************************************************
 * CPU Constants
 ****************************************************************/
#define SYS_CLK_80MHz 80000000
#define DO_THROTTLE_CAL 0
#define SEND_DEBUG 1
#define SEND_DEBUG2 0
#define THROTTLE_ADJUST_INC 2.0
#define ANGLE_RATE_INC 0.50 // Results in a rate of inc/loop update --> .5/.005 --> 100 degrees/sec
#define DEBUG_LOOPS 1
#define CFG_PRINTF_USE_UART 1
