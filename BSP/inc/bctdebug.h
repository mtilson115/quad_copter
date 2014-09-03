//
// Copyright (C) 2013 Blue Chip Technology Ltd
//
// Class to provide messages to MPLAB debug session via AppIO
//
// This class emulates the PIC32 debug macros which don't currently work in C++
//
// Class is static, so constructor is private to prevent instantiation
//
#ifndef BCTDEBUG_H
#define	BCTDEBUG_H
/******************************************************************************/
/* Definitions and Macros                                                     */
/******************************************************************************/
#define MAX_DEBUG_LINE_SIZE 256

// Redefine DBPRINTF to use this class
#ifdef DBPRINTF
#undef DBPRINTF
#endif
#ifdef __DEBUG
#define DBPRINTF(fmt, ...) bctDebug::printf(fmt, ##__VA_ARGS__)
#else
#define DBPRINTF(fmt, ...) ((void)0)
#endif

// Redefine DBINIT to safely do nothing
#ifdef DBINIT
#undef DBINIT
#endif
#define DBINIT() ((void)0)

/******************************************************************************/
/* Class definition                                                           */
/******************************************************************************/
class bctDebug {
// Constructors
private:    bctDebug();
// Destructor
virtual ~bctDebug();
// Methods - Operations
public:    static void printf(const int8_t *format, ...);
};
#endif	/* BCTDEBUG_HPP */

