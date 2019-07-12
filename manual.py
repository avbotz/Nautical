import keyboard, time, socket

print ("Attempting to connect. Ensure that the sub-side node is running.")
s = socket.socket()
port = 8080
s.connect(('192.168.2.50', port))
print ("Finished connecting.")
logging = True

while True:
    time.sleep(0.5)
    state = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    if keyboard.is_pressed('w'):
        state[0] = 1
    if keyboard.is_pressed('a'):
        state[1] = 1
    if keyboard.is_pressed('s'):
        state[2] = 1
    if keyboard.is_pressed('d'):
        state[3] = 1
    if keyboard.is_pressed('left'):
        state[4] = 1
    if keyboard.is_pressed('right'):
        state[5] = 1
    if keyboard.is_pressed('up'):
        state[6] = 1
    if keyboard.is_pressed('down'):
        state[7] = 1
    if keyboard.is_pressed('q'):
        state[8] = 1
    if keyboard.is_pressed('l'):
        # Toggle logging of current state
        print("Logging: " + logging)
        logging = not logging

    if logging:
        # Note: We are piggybacking log state on the state data here.
        # This is OK because of the way the `w` command is implemented;
        # it will only read the first NUM_MOTORS numbers (0-7 here).
        # This means we should be able to piggyback data without issues.
        state[9] = 1

        # Fetch state
        fetchbuf = s.recv(1024).decode("utf-8")

        # Match first state that comes up (most recent one)
        current_state = fetchbuf[fetchbuf.find("("):fetchbuf.find(")")]
        print(current_state)

    out = 'w '
    for i in state:
        out += str(i) + ' '
    out += '\n'

    s.send(out)
    print(out)
    print ('\n' + str(state))
    if state[8] == 1:
        break
print ("Finished using manual control.")
