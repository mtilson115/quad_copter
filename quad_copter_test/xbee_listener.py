import socket

UDP_IP = "10.0.0.67"
UDP_PORT = 5005

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
sock.bind((UDP_IP, UDP_PORT))

try:
    while True:
        data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
        data = data.split(',')
        int_data = map(int,data)
        print int_data
except:
    print "Exiting..."
