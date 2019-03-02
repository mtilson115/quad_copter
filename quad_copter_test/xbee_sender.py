import socket
import time

UDP_IP = "10.0.0.111"
UDP_PORT = 5005
MESSAGE = "Hello, World!"

print "UDP target IP:", UDP_IP
print "UDP target port:", UDP_PORT
print "message:", MESSAGE

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP

while True:
    try:
        sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))
        time.sleep(5)
    except (KeyboardInterrupt, SystemExit):
        print "Exiting..."
        raise

