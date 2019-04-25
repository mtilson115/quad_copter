import threading
import queue
import serial

msg_queue = queue.Queue()
msg = bytearray()
COMMS_SET_THROTTLE = 0x01
COMMS_SET_PI = 0x02
ser = serial.Serial('/dev/ttyUSB0',115200)

def print_motor_speeds_pitch_roll_P_I():
    data = ser.read(32)
    motor1  = struct.unpack('<f', data[0:4])[0]
    motor2  = struct.unpack('<f', data[4:8])[0]
    motor3  = struct.unpack('<f', data[8:12])[0]
    motor4  = struct.unpack('<f', data[12:16])[0]
    pitch   = struct.unpack('<f', data[16:20])[0]
    roll    = struct.unpack('<f', data[20:24])[0]
    P       = struct.unpack('<f', data[24:28])[0]
    I       = struct.unpack('<f', data[28:32])[0]
    print "%f,%f,%f,%f,%f,%f,%f,%f" % (motor1,motor2,motor3,motor4,pitch,roll,P,I)

def msg_thread():
    if ser.is_open:
        while True:
            if msg_queue.empty() != True:
                msg = msg_queue.get()
                if msg[0] == COMMS_SET_THROTTLE or msg[0] == COMMS_SET_PI:
                    ser.write(msg)

def rx_thread():
    if ser.is_open:
        while True:
            hdr = ser.read(1)
            hdr = struct.unpack('B',hdr[0])[0]
            if hdr == 17:
                print_motor_speeds_pitch_roll_P_I()

def input_thread():
