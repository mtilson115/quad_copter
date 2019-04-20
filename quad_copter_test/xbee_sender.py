import struct
import time
import serial

COMMS_SET_THROTTLE = 0x01
MSG_ID = COMMS_SET_THROTTLE
MSG_THROTTLE = 35.0;
MSG = bytearray(5)
struct.pack_into('B',MSG,0,MSG_ID)
struct.pack_into('<f',MSG,1,MSG_THROTTLE)

ser = serial.Serial('/dev/ttyUSB0',115200)

while True:
    try:
        ser.write(MSG)
        time.sleep(1.0)
        print MSG_THROTTLE
        if MSG_THROTTLE < 45.0:
            MSG_THROTTLE = MSG_THROTTLE + 1.0
        else:
            MSG_THROTTLE = 35.0
        struct.pack_into('<f',MSG,1,MSG_THROTTLE)
    except (KeyboardInterrupt, SystemExit):
        print "Exiting..."
        raise
