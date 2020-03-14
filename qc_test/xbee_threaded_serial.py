import threading
import queue as queue
import serial
import struct
import glob
import sys
import csv
import time
import platform
import readchar
import random

msg_queue = queue.Queue()
COMMS_SET_THROTTLE = 0x01
COMMS_SET_PID = 0x02
COMMS_CALIBRATE = 0x03
COMMS_DEBUG_M_PR = 0x05
COMMS_MOTOR_CAL = 0x06
COMMS_ERASE_CAL = 0x07
COMMS_SAVE_MOTOR_CAL = 0x08
COMMS_PITCH_ROLL = 0x04
COMMS_SET_COMP_FILT_CONST = 0x09

thread_stop = False
current_speed = 0.0
current_pitch = 0.0
current_roll = 0.0
current_yaw = 0.0

if platform.system() == 'Linux':
    ser = glob.glob('/dev/ttyUSB*');
else:
    ser = glob.glob('/dev/tty.usb*');

ser_str = ser[0];
ser = serial.Serial(ser_str,38400)
csv_file = None
stop_threads = False

def print_motor_speeds_pitch_roll_yaw_P_I(csv_writer):
    data = ser.read(48)
    # upack_data = struct.unpack('<Qffffffffffffffff',data)
    timestamp = struct.unpack('<Q', data[0:8])[0];
    motor1  = struct.unpack('<f', data[8:12])[0]
    motor2  = struct.unpack('<f', data[12:16])[0]
    motor3  = struct.unpack('<f', data[16:20])[0]
    motor4  = struct.unpack('<f', data[20:24])[0]
    pitch   = struct.unpack('<f', data[24:28])[0]
    pitchsp = struct.unpack('<f', data[28:32])[0]
    roll    = struct.unpack('<f', data[32:36])[0]
    rollsp  = struct.unpack('<f', data[36:40])[0]
    yaw     = struct.unpack('<f', data[40:44])[0]
    yawsp   = struct.unpack('<f', data[44:48])[0]
    if csv_writer != None:
        data = [timestamp,motor1,motor2,motor3,motor4,pitch,pitchsp,roll,rollsp,yaw,yawsp]
        csv_writer.writerow(data)
    else:
        print ("{},{:.4f},{:.4f},{:.4f},{:.4f},{:.4f},{:.4f},{:.4f},{:.4f},{:.4f},{:.4f}\r".format(timestamp,motor1,motor2,motor3,motor4,pitch,pitchsp,roll,rollsp,yaw,yawsp))
        # print("{}".format(unpack_data))

def print_pitch_roll():
    data = ser.read(24)
    g_pitch = struct.unpack('<f', data[0:4])[0]
    g_roll  = struct.unpack('<f', data[4:8])[0]
    a_pitch = struct.unpack('<f', data[8:12])[0]
    a_roll  = struct.unpack('<f', data[12:16])[0]
    pitch   = struct.unpack('<f', data[16:20])[0]
    roll    = struct.unpack('<f', data[20:24])[0]
    print ("{:.4f},{:.4f},{:.4f},{:.4f},{:.4f},{:.4f}\r".format(g_pitch,g_roll,a_pitch,a_roll,pitch,roll))

def print_battery_voltage():
    data = ser.read(4)
    voltage = struct.unpack('<f', data[0:4])[0]
    print(("Battery = {}V\r").format(voltage))

def print_calibration():
    data = ser.read(14)
    AX = struct.unpack('<h', data[0:2])[0]
    AY = struct.unpack('<h', data[2:4])[0]
    AZ = struct.unpack('<h', data[4:6])[0]
    GX = struct.unpack('<h', data[6:8])[0]
    GY = struct.unpack('<h', data[8:10])[0]
    GZ = struct.unpack('<h', data[10:12])[0]
    temperature = struct.unpack('<h', data[12:14])[0]
    print ("{},{},{},{},{},{},{}\r".format(AX,AY,AZ,GX,GY,GZ,temperature))

def set_motor_speed(speed):
    global current_speed
    current_speed = speed
    msg = bytearray(5)
    struct.pack_into('B',msg,0,COMMS_SET_THROTTLE)
    struct.pack_into('<f',msg,1,current_speed)
    print (("Set throttle to: {}").format(current_speed))
    msg_queue.put(msg)

def set_pitch_roll_yaw(pitch,roll,yaw):
    global current_pitch
    global current_roll
    global current_yaw
    current_pitch = pitch
    current_roll = roll
    current_yaw = yaw
    msg = bytearray(13)
    struct.pack_into('B',msg,0,COMMS_PITCH_ROLL)
    struct.pack_into('<f',msg,1,current_pitch)
    struct.pack_into('<f',msg,5,current_roll)
    struct.pack_into('<f',msg,9,current_yaw)
    print(("Attitude set to: {},{},{}").format(current_pitch,current_roll,current_yaw))
    msg_queue.put(msg)

def run_comms_test():
    for s in range(1,200):
        speed = random.uniform(10,40)
        set_motor_speed(speed)
        sleep_time = random.uniform(10,50)
        sleep_time = sleep_time/1000.0
        time.sleep(sleep_time)
        set_pitch_roll_yaw(speed,speed,speed)
        sleep_time = random.uniform(10,50)
        sleep_time = sleep_time/1000.0
        time.sleep(sleep_time)
    set_motor_speed(0.0)

def msg_thread():
    global stop_threads
    if ser.is_open:
        try:
            while True:
                try:
                    msg = msg_queue.get(block=False)
                    ser.write(msg)
                except queue.Empty:
                    if stop_threads == True:
                        msg = bytearray(5)
                        struct.pack_into('B',msg,0,COMMS_SET_THROTTLE)
                        struct.pack_into('<f',msg,1,0.0)
                        ser.write(msg)
                        return
                    else:
                        time.sleep(0.2)
        except:
            msg = bytearray(5)
            struct.pack_into('B',msg,0,COMMS_SET_THROTTLE)
            struct.pack_into('<f',msg,9,0.0)
            ser.write(msg)
            return

def rx_thread(csvW):
    global stop_threads
    if ser.is_open:
        try:
            while True:
                if ser.in_waiting > 0:
                    data = ser.read(1)
                    hdr = struct.unpack('B',data)[0]
                    if hdr == 17:
                        print_motor_speeds_pitch_roll_yaw_P_I(csvW)
                    elif hdr == 20:
                        print_battery_voltage()
                    elif hdr == 1:
                        print_calibration()
                    elif hdr == 21:
                        print_pitch_roll()
                    else:
                        print ("0x{:X}\r".format(hdr))
                    # 23 is the id for printf
                    '''
                    elif hdr == 23:
                        str_ = ser.readline()
                        str_ = data+str_
                        str_ = str_.decode("utf-8")
                        str_ = str_.strip('\n\r')
                        print (str_)
                    '''
                elif stop_threads == True:
                    return
                else:
                    time.sleep(0.001)
        except:
            return

def input_thread():
    global stop_threads
    global current_speed
    global current_pitch
    global current_roll
    global current_yaw
    try:
        while True:
            print("Commands:")
            print("s: set the speed as a percentage (0 - 100)")
            print("p: set PID constants")
            print("c: Run Accel Gyro Calibration (erase flash first)")
            print("g: Set Debug Output (0 - off 1 - on)")
            print("e: exit")
            print("f: erase cal flash")
            print("m: Send motor calibration")
            print("o: Store motor calibration to flash (erase flash first)")
            print("u: Throttle Up")
            print("d: Throttle Down")
            print("r: run comms test")
            print("up arrow: Pitch forward")
            print("down arrow: Pitch backward")
            print("left arrow: roll left")
            print("right arrow: roll right")
            print("a: adjust comp filt coeff")
            cmd = readchar.readkey()
            if cmd == 'a':
                A = input()
                A = float(A)
                msg = bytearray(5)
                struct.pack_into('B',msg,0,COMMS_SET_COMP_FILT_CONST)
                struct.pack_into('<f',msg,1,A)
                print("Set comp filt const to: {}".format(A))
                msg_queue.put(msg)
            if cmd == 'r':
                for idx in range(0,100):
                    run_comms_test()
            if cmd == 's':
                speed = input()
                speed = float(speed)
                if speed >= 0.0 and speed <= 100:
                    current_speed = speed
                    msg = bytearray(5)
                    struct.pack_into('B',msg,0,COMMS_SET_THROTTLE)
                    struct.pack_into('<f',msg,1,speed)
                    print (("Set throttle to: {}").format(speed))
                    msg_queue.put(msg)
            if cmd == 'p':
                print ("Enter the pitch P value:")
                pitchP = input()
                pitchP = float(pitchP)
                print ("Enter the pitch I value:")
                pitchI = input()
                pitchI = float(pitchI)
                print ("Enter the pitch D value:")
                pitchD = input()
                pitchD = float(pitchD)
                print ("Enter the roll P value:")
                rollP = input()
                rollP = float(rollP)
                print ("Enter the roll I value:")
                rollI = input()
                rollI = float(rollI)
                print ("Enter the roll D value:")
                rollD = input()
                rollD = float(rollD)
                msg = bytearray(25)
                struct.pack_into('B',msg,0,COMMS_SET_PID)
                struct.pack_into('<f',msg,1,pitchP)
                struct.pack_into('<f',msg,5,pitchI)
                struct.pack_into('<f',msg,9,pitchD)
                struct.pack_into('<f',msg,13,rollP)
                struct.pack_into('<f',msg,17,rollI)
                struct.pack_into('<f',msg,21,rollD)
                print (("Set pitchP = {} pitchI = {} pitchD = {} rollP = {} rollI = {} rollD = {}").format(pitchP,pitchI,pitchD,rollP,rollI,rollD))
                msg_queue.put(msg)
            if cmd == 'm':
                print ("Enter the m1 value:")
                m1 = input()
                m1 = float(m1)
                print ("Enter the m2 value:")
                m2 = input()
                m2 = float(m2)
                print ("Enter the m3 value:")
                m3 = input()
                m3 = float(m3)
                print ("Enter the m4 value:")
                m4 = input()
                m4 = float(m4)
                msg = bytearray(17)
                struct.pack_into('B',msg,0,COMMS_MOTOR_CAL)
                struct.pack_into('<f',msg,1,m1)
                struct.pack_into('<f',msg,5,m2)
                struct.pack_into('<f',msg,9,m3)
                struct.pack_into('<f',msg,13,m4)
                print (("Set m1 = {} m2 = {} m3 = {} m4 = {}\r").format(m1,m2,m3,m4))
                msg_queue.put(msg)
            if cmd == 'o':
                msg = bytearray(1)
                struct.pack_into('B',msg,0,COMMS_SAVE_MOTOR_CAL)
                print ("Storing motor calibration")
                msg_queue.put(msg)
            if cmd == 'c':
                msg = bytearray(1)
                struct.pack_into('B',msg,0,COMMS_CALIBRATE)
                print ("Calibrating")
                for cnt in range(0,80):
                    msg_queue.put(msg)
                    time.sleep(9)
            if cmd == 'f':
                msg = bytearray(1)
                struct.pack_into('B',msg,0,COMMS_ERASE_CAL)
                print ("Erasing Cal")
                msg_queue.put(msg)
            if cmd == 'g':
                debug = input()
                debug = int(debug)
                msg = bytearray(2)
                struct.pack_into('B',msg,0,COMMS_DEBUG_M_PR)
                struct.pack_into('B',msg,1,debug)
                msg_queue.put(msg)
            if cmd == 'e':
                print("Exiting...");
                stop_threads = True
                return
            if cmd == 'u':
                if current_speed <= 89.0:
                    current_speed += 0.5 
                msg = bytearray(5)
                struct.pack_into('B',msg,0,COMMS_SET_THROTTLE)
                struct.pack_into('<f',msg,1,current_speed)
                print (("Set throttle to: {}").format(current_speed))
                msg_queue.put(msg)
            if cmd == 'd':
                if current_speed >= 1.0:
                    current_speed -= 0.5 
                msg = bytearray(5)
                struct.pack_into('B',msg,0,COMMS_SET_THROTTLE)
                struct.pack_into('<f',msg,1,current_speed)
                print (("Set throttle to: {}").format(current_speed))
                msg_queue.put(msg)
            if cmd == '\x1b[A':
                if current_pitch <= 20.0:
                    current_pitch +=  0.1
                msg = bytearray(13)
                struct.pack_into('B',msg,0,COMMS_PITCH_ROLL)
                struct.pack_into('<f',msg,1,current_pitch)
                struct.pack_into('<f',msg,5,current_roll)
                struct.pack_into('<f',msg,9,current_yaw)
                print(("Pitch set to: {}").format(current_pitch))
                msg_queue.put(msg)
            if cmd == '\x1b[B':
                if current_pitch >= -20.0:
                    current_pitch -=  0.1
                msg = bytearray(13)
                struct.pack_into('B',msg,0,COMMS_PITCH_ROLL)
                struct.pack_into('<f',msg,1,current_pitch)
                struct.pack_into('<f',msg,5,current_roll)
                struct.pack_into('<f',msg,9,current_yaw)
                print(("Pitch set to: {}").format(current_pitch))
                msg_queue.put(msg)
            if cmd == '\x1b[C':
                if current_roll <= 20.0:
                    current_roll +=  0.1
                msg = bytearray(13)
                struct.pack_into('B',msg,0,COMMS_PITCH_ROLL)
                struct.pack_into('<f',msg,1,current_pitch)
                struct.pack_into('<f',msg,5,current_roll)
                struct.pack_into('<f',msg,9,current_yaw)
                print(("Roll set to: {}").format(current_roll))
                msg_queue.put(msg)
            if cmd == '\x1b[D':
                if current_pitch >= -20.0:
                    current_roll -=  0.1
                msg = bytearray(13)
                struct.pack_into('B',msg,0,COMMS_PITCH_ROLL)
                struct.pack_into('<f',msg,1,current_pitch)
                struct.pack_into('<f',msg,5,current_roll)
                struct.pack_into('<f',msg,9,current_yaw)
                print(("Roll set to: {}").format(current_roll))
                msg_queue.put(msg)

    except:
        print("Exiting...");
        stop_threads = True
        return

if __name__ == "__main__":
    if ser.is_open:
        print (("Serial port {} connected").format(ser_str))
        if len(sys.argv) > 1:
            csv_file = sys.argv[1]
            print(("Using {} to data record").format(csv_file))
            with open(csv_file,'w',newline='') as csvFile:
                csvWrtier = csv.writer(csvFile, delimiter=',')
                input_thrd = threading.Thread(target=input_thread)
                rx_thrd = threading.Thread(target=rx_thread,args=(csvWrtier,))
                msg_thrd = threading.Thread(target=msg_thread)
                input_thrd.daemon = True
                msg_thrd.daemon = True
                rx_thrd.daemon = True
                input_thrd.start()
                msg_thrd.start()
                rx_thrd.start()
                input_thrd.join()
                msg_thrd.join()
                rx_thrd.join()
                quit()
        else:
            print("No log file passed, will print debug data")
            input_thrd = threading.Thread(target=input_thread)
            rx_thrd = threading.Thread(target=rx_thread,args=(None,))
            msg_thrd = threading.Thread(target=msg_thread)
            input_thrd.daemon = True
            msg_thrd.daemon = True
            rx_thrd.daemon = True
            input_thrd.start()
            msg_thrd.start()
            rx_thrd.start()
            input_thrd.join()
            msg_thrd.join()
            rx_thrd.join()
            quit()
    else:
        print ("No serial device found")
        quit()
