import socket
import struct
import time

UDP_IP = "10.0.0.111"
UDP_PORT = 5005
# MESSAGE = "Hello, World!"
COMMS_SET_THROTTLE = 0x01
MSG_ID = COMMS_SET_THROTTLE
MSG_THROTTLE = 0.4; # 10%
MSG = bytearray(5)
struct.pack_into('B',MSG,0,MSG_ID)
struct.pack_into('<f',MSG,1,MSG_THROTTLE)

print "UDP target IP:", UDP_IP
print "UDP target port:", UDP_PORT
print "message:", MSG

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP

while True:
    try:
        sock.sendto(MSG, (UDP_IP, UDP_PORT))
        time.sleep(0.5)
        if MSG_THROTTLE < 1.0:
            MSG_THROTTLE = MSG_THROTTLE + .1
        else:
            MSG_THROTTLE = .4
        struct.pack_into('<f',MSG,1,MSG_THROTTLE)
    except (KeyboardInterrupt, SystemExit):
        print "Exiting..."
        raise
