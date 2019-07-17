import serial


out = open("data.txt", "w")
state = 's 3. 0. 0. 0. 0. 0.\n'
port = serial.Serial('/dev/ttyACM0', 9600)
port.write(state.encode())
while True:
    port.write(b'c\n')
    temp = port.readline().decode('utf-8')
    out.write(temp)
