import threading
import queue as queue
import serial
import struct

msg_queue = queue.Queue()
COMMS_SET_THROTTLE = 0x01
COMMS_SET_PID = 0x02
COMMS_CALIBRATE = 0x03
thread_stop = False

#ser = serial.Serial('/dev/tty.usbserial-A602TSTD',115200)
ser = serial.Serial('/dev/ttyUSB0',115200)
#ser = serial.Serial('/dev/tty.usbserial-DN050LLX',115200)

def print_motor_speeds_pitch_roll_P_I():
    data = ser.read(40)
    timestamp = struct.unpack('<I', data[0:4])[0];
    motor1  = struct.unpack('<f', data[4:8])[0]
    motor2  = struct.unpack('<f', data[8:12])[0]
    motor3  = struct.unpack('<f', data[12:16])[0]
    motor4  = struct.unpack('<f', data[16:20])[0]
    pitch   = struct.unpack('<f', data[20:24])[0]
    roll    = struct.unpack('<f', data[24:28])[0]
    P       = struct.unpack('<f', data[28:32])[0]
    I       = struct.unpack('<f', data[32:36])[0]
    D       = struct.unpack('<f', data[36:40])[0]
    print ("{:.3f},{:.3f},{:.3f},{:.3f},{:.3f},{:.3f},{:.3f},{:.3f},{:.3f},{:.3f}".format(timestamp,motor1,motor2,motor3,motor4,pitch,roll,P,I,D))
#    print "%f,%f,%f,%f,%f,%f,%f,%f,%f" % (motor1,motor2,motor3,motor4,pitch,roll,P,I,D)

def print_battery_voltage():
    data = ser.read(4)
    voltage = struct.unpack('<f', data[0:4])[0]
    print(("Battery = {}V").format(voltage))

def print_calibration():
    data = ser.read(12)
    AX = struct.unpack('<h', data[0:2])[0]
    AY = struct.unpack('<h', data[2:4])[0]
    AZ = struct.unpack('<h', data[4:6])[0]
    GX = struct.unpack('<h', data[6:8])[0]
    GY = struct.unpack('<h', data[8:10])[0]
    GZ = struct.unpack('<h', data[10:12])[0]
    print ("AX = {},AY = {},AZ = {},GX = {},GY = {},GZ = {}".format(AX,AY,AZ,GX,GY,GZ))
#    print "AX = %d,AY = %d,AZ = %d,GX = %d,GY = %d,GZ = %d" % (AX,AY,AZ,GX,GY,GZ)

def msg_thread():
    global thread_stop
    if ser.is_open:
        while True:
            if thread_stop == True:
                return
            msg = msg_queue.get(block=True)
            if msg[0] == COMMS_SET_THROTTLE or msg[0] == COMMS_SET_PID or msg[0] == COMMS_CALIBRATE:
                ser.write(msg)

def rx_thread():
    global thread_stop
    if ser.is_open:
        while True:
            if thread_stop == True:
                return
            hdr = ser.read(1)
            hdr = struct.unpack('B',hdr)[0]
            if hdr == 17:
                print_motor_speeds_pitch_roll_P_I()
            elif hdr == 20:
                print_battery_voltage()
            elif hdr == 1:
                print_calibration()
            else:
                print (hdr)

def input_thread():
    global thread_stop
    while True:
        if thread_stop == True:
            return
        print("Commands:")
        print("s: set the speed as a percentage (0 - 100)")
        print("p: set PID constants")
        print("c: Run Calibration")
        print("e: exit")
        cmd = input()
        if cmd == 's':
            speed = input()
            speed = float(speed)
            if speed >= 0.0 and speed <= 100:
                msg = bytearray(5)
                struct.pack_into('B',msg,0,COMMS_SET_THROTTLE)
                struct.pack_into('<f',msg,1,speed)
                print (("Set throttle to: {}").format(speed))
                msg_queue.put(msg)
        if cmd == 'p':
            print ("Enter the P value:")
            P = input()
            P = float(P)
            print ("Enter the I value:")
            I = input()
            I = float(I)
            print ("Enter the D value:")
            D = input()
            D = float(D)
            msg = bytearray(13)
            struct.pack_into('B',msg,0,COMMS_SET_PID)
            struct.pack_into('<f',msg,1,P)
            struct.pack_into('<f',msg,5,I)
            struct.pack_into('<f',msg,9,D)
            print (("Set P = {} I = {} D = {}").format(P,I,D))
            msg_queue.put(msg)
        if cmd == 'c':
            msg = bytearray(1)
            struct.pack_into('B',msg,0,COMMS_CALIBRATE)
            msg_queue.put(msg)
        if cmd == 'e':
            thread_stop = True


if __name__ == "__main__":
    if ser.is_open:
        print ("Serial connected")
        input_thrd = threading.Thread(target=input_thread)
        rx_thrd = threading.Thread(target=rx_thread)
        msg_thrd = threading.Thread(target=msg_thread)
        input_thrd.start()
        msg_thrd.start()
        rx_thrd.start()
    else:
        print ("No serial device found")
        quit()
