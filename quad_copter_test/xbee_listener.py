import socket
from matplotlib import numpy as np

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
    print "%s" % (data)
    int_data = data.split(",");
    try:
        accel_x = np.float(int_data[0]);
        accel_y = np.float(int_data[1]);
        accel_z = np.float(int_data[2]);
        gyro_x = np.float(int_data[0]);
        gyro_y = np.float(int_data[1]);
        gyro_z = np.float(int_data[2]);
        #roll = roll + (gyro_y*.02);
        #pitch = pitch + (gyro_x*.02);
        accel_pitch = np.arctan2(accel_x,accel_z);
        accel_roll = np.arctan2(accel_y,accel_z);
        mroll = roll + accel_roll;
        mpitch = pitch + accel_pitch;
        roll_d = (mroll*180.0)/np.pi;
        pitch_d = (mpitch*180.0)/np.pi;
        print "Roll = %f degrees" % roll_d;
        print "Pitch = %f degrees" % pitch_d;
    except (KeyboardInterrupt, SystemExit):
        print "Exiting..."
        raise
    except:
        continue
#except:
#    print "Exiting..."
