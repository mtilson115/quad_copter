//
// Copyright (C) 2013 Blue Chip Technology Ltd
//
// Class to provide messages to MPLAB debug session via AppIO
//
// This class emulates the PIC32 debug macros which don't currently work in C++
//
// Class is static, so constructor is private to prevent instantiation
//
/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <stdarg.h>
#include <stdio.h>
#include <plib.h>
#include <p32xxxx.h>
#include "bctdebug.h"

/******************************************************************************/
/* Typedefs and Enums                                                         */
/******************************************************************************/
typedef union {    unsigned char byte[4];    unsigned int        w; } buffer_t;

/******************************************************************************/
/* Constructor / Destructor                                                   */
/******************************************************************************/

bctDebug::bctDebug(){}
bctDebug::~bctDebug(){}

/******************************************************************************/
/* Public Methods - Operations                                                */
/******************************************************************************/
void bctDebug::printf(const char* format, ...)
{
    // This is defined as static to avoid allocating it on the stack frame
    // Its contents do not need to persist between calls.
    static char lineBuffer[MAX_DEBUG_LINE_SIZE];

    // These are defined as static so that they persist between calls.
    // This is necessary because the output must be 4 bytes at a time, so
    // up to 3 bytes may need to be saved until the next message is sent
    static buffer_t outbuf;
    static unsigned int outBufCount = 0;

    // Render the string (with arguments) into the line buffer
    va_list argPtr;
    va_start(argPtr, format);
    vsnprintf(lineBuffer, MAX_DEBUG_LINE_SIZE, format, argPtr);
    va_end(argPtr);

    // Send the formatted string up to (but not including) the terminating null
    int i=0;
    while ((i < MAX_DEBUG_LINE_SIZE) && (lineBuffer != '\0'))
    {
        // Process the next character (i.e. pack it into the 32-bit word)
        outbuf.byte[outBufCount] = lineBuffer[i];
        outBufCount++;

        // Send the 32-bit word if it is now full
        if (outBufCount > 3)
        {
            outBufCount = 0;
            while(_DDPSTATbits.APOFUL);
            _APPO = outbuf.w;
            outbuf.w = 0;
        }

        // Loop for the next character
        i++;
    }
}
