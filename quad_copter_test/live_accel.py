import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style
import socket
import collections

class udp_listener:

    def __init__(self):
        self.UDP_PORT = 5005
        self.UDP_IP = "10.0.0.78"
        self.sock = socket.socket(socket.AF_INET, # Internet
                             socket.SOCK_DGRAM) # UDP
        self.sock.bind((self.UDP_IP, self.UDP_PORT))

    def get_data(self):
        data, addr = self.sock.recvfrom(40) # buffer size is 32 bytes
        return [data,addr]

class accel_plotter:

    def __init__(self):
        self.listener = udp_listener()
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
            return self.ax1,self.ax2,self.ax3
        self.ax1.clear()
        self.ax2.clear()
        self.ax3.clear()
        self.i1.append(i)
        self.x1.append(int_data[3])
        self.i2.append(i)
        self.y2.append(int_data[4])
        self.i3.append(i)
        self.z3.append(int_data[5])
        self.ax1.grid()
        self.ax2.grid()
        self.ax3.grid()
        self.ax1.plot(self.i1,self.x1,'r-o')
        self.ax2.plot(self.i2,self.y2,'b-o')
        self.ax3.plot(self.i3,self.z3,'g-o')
        return self.ax1,self.ax2,self.ax3

if __name__ == "__main__":

    accel_plotter = accel_plotter()
    plt.grid()
    ani = animation.FuncAnimation(accel_plotter.fig, accel_plotter.animate, interval=10, blit=False)
    plt.show()
