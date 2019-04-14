import serial
import struct
import time

ser = serial.Serial('/dev/ttyUSB0',115200)

print (ser.is_open)

if ser.is_open == True:
    print "Ctrl-C to exit"
    data = ser.read(4)
    print data
    while True:
        try:
            data = ser.read(12)
            float1 = struct.unpack('<f', data[0:4])[0]
            float2 = struct.unpack('<f', data[4:8])[0]
            float3 = struct.unpack('<f', data[8:12])[0]
            print "Pitch: %f, Roll: %f, MotorSpeed: %f" % (float1,float2,100*float3)
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
