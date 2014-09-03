/***************************************************************************
 *
 * Serial port communications
 *
 * Creater: Mitchell Tilson
 *
 * Revision: 1.0 4/13/2014
 *
 *
 ***************************************************************************/
#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H

// Serial port
#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

extern int open_port(char port[]);
extern void set_baud( int fd, char baud[] );

#endif // SERIAL_PORT_H
