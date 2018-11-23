import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style
import socket
import collections

class udp_listener:

    def __init__(self):
        self.UDP_PORT = 5005
        self.UDP_IP = "10.0.0.67"
        self.sock = socket.socket(socket.AF_INET, # Internet
                             socket.SOCK_DGRAM) # UDP
        self.sock.bind((self.UDP_IP, self.UDP_PORT))

    def get_data(self):
        data, addr = self.sock.recvfrom(1024) # buffer size is 1024 bytes
        return [data,addr]

class accel_plotter:

    def __init__(self):
        self.listener = udp_listener()
        #style.use('fivethirtyeight')
        self.fig = plt.figure()
        self.ax1 = self.fig.add_subplot(3,1,1)
        self.ax2 = self.fig.add_subplot(3,1,2)
        self.ax3 = self.fig.add_subplot(3,1,3)
        self.fig_len = 20
        self.i1 = collections.deque(maxlen=self.fig_len)
        self.x1 = collections.deque(maxlen=self.fig_len)
        self.i2 = collections.deque(maxlen=self.fig_len)
        self.y2 = collections.deque(maxlen=self.fig_len)
        self.i3 = collections.deque(maxlen=self.fig_len)
        self.z3 = collections.deque(maxlen=self.fig_len)

    def animate(self,i):
        [data,addr] = self.listener.get_data()
        print data
        data = data.split(',')
        try:
            int_data = map(int,data)
        except:
            return
        self.ax1.clear()
        self.ax2.clear()
        self.ax3.clear()
        self.i1.append(i)
        self.x1.append(int_data[0])
        self.i2.append(i)
        self.y2.append(int_data[1])
        self.i3.append(i)
        self.z3.append(int_data[2])
        self.ax1.grid()
        self.ax2.grid()
        self.ax3.grid()
        self.ax1.plot(self.i1,self.x1,'r')
        self.ax2.plot(self.i2,self.y2,'b')
        self.ax3.plot(self.i3,self.z3,'g')

if __name__ == "__main__":

    accel_plotter = accel_plotter()
    plt.grid()
    ani = animation.FuncAnimation(accel_plotter.fig, accel_plotter.animate, interval=500)
    plt.show()
