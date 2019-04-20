import serial
import struct
import time

def print_accel_pitch_speed():
    data = ser.read(8)
    float1 = struct.unpack('<f', data[0:4])[0]
    float2 = struct.unpack('<f', data[4:8])[0]
    print "%f,%f" % (float1,float2)

def print_motor_speeds():
    data = ser.read(16)
    motor1 = struct.unpack('<f', data[0:4])[0]
    motor2 = struct.unpack('<f', data[4:8])[0]
    motor3 = struct.unpack('<f', data[8:12])[0]
    motor4 = struct.unpack('<f', data[12:16])[0]
    print "Motor1: %f, Motor2: %f, Motor3: %f, Motor4: %f" % (motor1,motor2,motor3,motor4)

def print_accel_gyro():
    data = ser.read(12)
    ax = struct.unpack('<h', data[0:2])[0]
    ay = struct.unpack('<h', data[2:4])[0]
    az = struct.unpack('<h', data[4:6])[0]
    gx = struct.unpack('<h', data[6:8])[0]
    gy = struct.unpack('<h', data[8:10])[0]
    gz = struct.unpack('<h', data[10:12])[0]
    print "%d,%d,%d,%d,%d,%d" % (ax,ay,az,gx,gy,gz)

def print_accel_x():
    data = ser.read(2)
    ax = struct.unpack('<H', data[0:2])[0]
    print "%d" % (ax)

ser = serial.Serial('/dev/ttyUSB0',115200)

print (ser.is_open)

if ser.is_open == True:
    print "Ctrl-C to exit"
    data = ser.read(4)
    print data
    while True:
        try:
            hdr = ser.read(1)
            hdr = struct.unpack('B',hdr[0])[0]
            if hdr == 16:
                print_motor_speeds();
            elif hdr == 10:
                print_accel_pitch_speed();
            elif hdr == 11:
                print_accel_gyro();
            elif hdr == 12:
                print_accel_x();
            time.sleep(0.020)
        except (KeyboardInterrupt, SystemExit):
            print "Exiting..."
            ser.close()
            raise
        except:
            if data != '':
                print data
                print len(data)

else:
    print "Serial did not open"


