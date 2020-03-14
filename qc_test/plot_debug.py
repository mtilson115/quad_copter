import matplotlib.pyplot as plt
import csv
import sys
import os.path

time = []
m1 = []
m2 = []
m3 = []
m4 = []
pitch = []
roll = []
yaw = []
PitchP = 0
PitchI = 0
PitchD = 0
RollP = 0
RollI = 0
RollD = 0
data_file = sys.argv[1]
throttle_cut = 50
if len(sys.argv) > 2:
    throttle_cut = float(sys.argv[2])
data_file = os.path.abspath(data_file)

with open(data_file,'r') as csvfile:
    plots = csv.reader(csvfile, delimiter=',')
    for row in plots:
        if float(row[1]) > throttle_cut:
            time.append(float(row[0]))
            m1.append(float(row[1]))
            m2.append(float(row[2]))
            m3.append(float(row[3]))
            m4.append(float(row[4]))
            pitch.append(float(row[5]))
            roll.append(float(row[7]))
            yaw.append(float(row[9]))
            PitchP = float(row[11])
            PitchI = float(row[12])
            PitchD = float(row[13])
            RollP = float(row[11])
            RollI = float(row[12])
            RollD = float(row[13])

fig, (ax1, ax2) = plt.subplots(2, 1)
ax1.plot(time,m1,label='M1')
ax1.plot(time,m2,label='M2')
ax1.plot(time,m3,label='M3')
ax1.plot(time,m4,label='M4')
ax1.set_ylabel('Throttle %')
ax1.set_title('Initial Flight Data')
ax1.legend(loc='upper left')
ax1.grid()

ax2.plot(time,pitch,label='Pitch')
ax2.plot(time,roll,label='Roll')
ax2.plot(time,yaw,label='Yaw')
ax2.set_ylabel('Degrees')
ax2.set_xlabel('Time')
ax2.legend(loc='upper left')
ax2.grid()

plt.show()
