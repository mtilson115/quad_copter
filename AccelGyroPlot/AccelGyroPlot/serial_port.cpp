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
#include "serial_port.h"
#include <stdlib.h>

/***************************************************************************
 *
 * Opens the desired serial port for communications.
 *
 * Creater: Mitchell Tilson
 *
 * Revision: 1.0 4/13/2014
 *
 * port must be of the form: "/dev/ttyname"
 *
 ***************************************************************************/
int open_port(char port[])
{
  int fd; /* File descriptor for the port */


  // fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
  fd = open(port, O_RDWR | O_NOCTTY | O_NONBLOCK);
  if (fd == -1)
  {
   /*
    * Could not open the port.
    */

    perror("open_port: Unable to open the port");
  } else {
    fcntl(fd, F_SETFL, 0);
  }

  return (fd);
}

/***************************************************************************
 *
 * Sets the buad rate
 *
 * Creater: Mitchell Tilson
 *
 * Revision: 1.0 4/13/2014
 *
 * baud must be of the form: "115200"
 *
 * fd must be a file descriptor or id returned by open_port
 *
 ***************************************************************************/
void set_baud( int fd, char baud[] )
{
  struct termios options;
  int int_baud = 0;
  tcgetattr(fd, &options);
  int_baud = atoi(baud);

  if( int_baud == 9600 ) {
    cfsetispeed(&options,B9600);
    cfsetospeed(&options,B9600);
  } else if( int_baud == 115200 ) {
    cfsetispeed(&options,B115200);
    cfsetospeed(&options,B115200);
  } else {
    cfsetispeed(&options,B9600);
    cfsetospeed(&options,B9600);
  }

  options.c_cflag |= (CLOCAL | CREAD );

  tcsetattr(fd, TCSANOW, &options );
}
