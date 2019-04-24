import struct
import time
import serial

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

ser = serial.Serial('/dev/ttyUSB0',115200)
init_str = ser.read(4)
print init_str

while True:
    try:
        ser.write(MSG)
        time.sleep(1.0)
        print "Sent: %f" % MSG_THROTTLE
        if MSG_THROTTLE < 40.0:
            MSG_THROTTLE = MSG_THROTTLE + 0.25
        else:
            MSG_THROTTLE = 0.0
            quit()
        struct.pack_into('<f',MSG,1,MSG_THROTTLE)
        hdr = ser.read(1)
        hdr = struct.unpack('B',hdr[0])[0]
        if hdr == 14:
            print_pitch_roll();
    except (KeyboardInterrupt, SystemExit):
        MSG_THROTTLE = 0.0
        struct.pack_into('<f',MSG,1,MSG_THROTTLE)
        ser.write(MSG)
        ser.close()
        print "Exiting..."
        raise
