quad_copter
===========

Microchip PIC32MX795F512L With uCOS RTOS and Accel Gyro driver port from arduino

**Compiling**

1. Install the xc32 compiler from microchip
2. Modify Makefile to point CC to the to where the compiler is located
3. type make

**Output Files**

A binary (same as a .elf) and a .hex file will be placed in the BIN directory

**Modifying what is included in the build**

In side Makefile, you can modify the DIRS variable to include new folders to compile.

**Required directory structure**

If you do add new folders to the build, you must use the ./src and ./inc file naming structure for the
build system to work.

