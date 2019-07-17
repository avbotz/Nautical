import pylab as plt 
import numpy as np


data = np.genfromtxt('data.txt', delimiter=' ')
x = data[:, 0]
y = data[:, 1]
z = data[:, 2]
yaw = data[:, 3]
pitch = data[:, 4]
roll = data[:, 5]
X = np.arange(0, len(data))

plt.figure(0)
plt.xlabel('Time')
plt.ylabel('X')
plt.plot(X, x)

plt.figure(1)
plt.xlabel('Time')
plt.ylabel('Y')
plt.plot(X, y)

plt.figure(2)
plt.xlabel('Time')
plt.ylabel('Z')
plt.plot(X, z)

plt.figure(3)
plt.xlabel('Time')
plt.ylabel('Yaw')
plt.plot(X, yaw)

plt.figure(4)
plt.xlabel('Time')
plt.ylabel('Pitch')
plt.plot(X, pitch)

plt.figure(5)
plt.xlabel('Time')
plt.ylabel('Roll')
plt.plot(X, roll)

plt.show()
