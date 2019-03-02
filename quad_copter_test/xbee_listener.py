import socket
from matplotlib import numpy as np
import struct

UDP_IP = "10.0.0.67"
UDP_PORT = 5005

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
sock.bind((UDP_IP, UDP_PORT))

roll = 0
pitch = 0

#try:
while True:
    data, addr = sock.recvfrom(256) # buffer size is 1024 bytes
    print data
    '''
    try:
        float1 = struct.unpack('<f', data[0:4])[0]
        float2 = struct.unpack('<f', data[4:8])[0]
        float3 = struct.unpack('<f', data[8:12])[0]
        print "Pitch: %f, Roll: %f, MotorSpeed: %f" % (float1,float2,100*float3)
    except (KeyboardInterrupt, SystemExit):
        print "Exiting..."
        raise
    except:
        print len(data)
   '''
