import struct
import time
import serial

UP = '\x1b[A'
LEFT = '\x1b[D'
RIGHT = '\x1b[C'
DOWN = '\x1b[B'

def print_pitch_roll():
    data = ser.read(8)
    pitch = struct.unpack('<f', data[0:4])[0]
    roll = struct.unpack('<f', data[4:8])[0]
    print "pitch: %f, roll: %f" % (pitch,roll)

COMMS_SET_THROTTLE = 0x01
MSG_ID = COMMS_SET_THROTTLE
MSG_THROTTLE = 30.0;
MSG = bytearray(5)
struct.pack_into('B',MSG,0,MSG_ID)
struct.pack_into('<f',MSG,1,MSG_THROTTLE)

ser = serial.Serial('/dev/tty.usbserial-A602TSTD',115200)

if ser.is_open:
    print "Serial device found"
else:
    print "Unable to connect to serial device"
    quit()

#init_str = ser.read(4)
#print init_str

while True:
    try:
        key = raw_input("Increase Speed")
        if key == UP:
            MSG_THROTTLE = MSG_THROTTLE + 1.0
            struct.pack_into('<f',MSG,1,MSG_THROTTLE)
            ser.write(MSG)
            print "Sent: %f" % MSG_THROTTLE
        elif key == DOWN:
            MSG_THROTTLE = MSG_THROTTLE - 1.0
            struct.pack_into('<f',MSG,1,MSG_THROTTLE)
            ser.write(MSG)
            print "Sent: %f" % MSG_THROTTLE
    except (KeyboardInterrupt, SystemExit):
        MSG_THROTTLE = 0.0
        struct.pack_into('<f',MSG,1,MSG_THROTTLE)
        print "Set throttle to %f" % MSG_THROTTLE
        ser.write(MSG)
        ser.close()
        print "Exiting..."
        raise
