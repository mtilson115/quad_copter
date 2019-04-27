import threading
import queue
import serial

msg_queue = queue.Queue()
COMMS_SET_THROTTLE = 0x01
COMMS_SET_PI = 0x02
#ser = serial.Serial('/dev/tty.usbserial-A602TSTD',115200)
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
            msg = msg_queue.get(block=True)
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
    while True:
        print "Commands:"
        print "s: set the speed as a percentage (0 - 100)"
        print "p: set PI constants"
        cmd = raw_input()
        if cmd == 's':
            speed = raw_input()
            speed = float(speed)
            if speed >= 0.0 and speed <= 100:
                msg = bytearray(5)
                struct.pack_into('B',msg,0,COMMS_SET_THROTTLE)
                struct.pack_into('<f',msg,1,speed)
                print "Set throttle to: %f" % speed
                msg_queue.put(msg)
        if cmd == 'p':
            print "Enter the P value:"
            P = raw_input()
            P = float(P)
            print "Enter the I value:"
            I = raw_input()
            I = float(I)
            msg = bytearray(9)
            struct.pack_into('B',msg,0,COMMS_SET_PI)
            struct.pack_into('<f',msg,1,P)
            struct.pack_into('<f',msg,5,I)
            print "Set P = %f I = %f" % (P,I)
            msg_queue.put(msg)

if __name__ == "__main__":
    if ser.is_open:
        print "Serial connected"
        input_thrd = threading.Thread(target=input_thread)
        rx_thrd = threading.Thread(target=rx_thread)
        msg_thrd = threading.Thread(target=msg_thread)
        input_thrd.start()
        msg_thrd.start()
        rx_thrd.start()
    else:
        print "No serial device found"
        quit()
