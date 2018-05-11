I've been meaning to write documentation for Nautical control for a while, so here is the TLDR version. If by chance you end up running a pool test you'll need to know this info. I'll also add how the code works in a Google doc once I'm finished with it, but for now comments should explain most of it.

Startup:
-> First ssh onto the sub (*ssh user@192.168.2.3* after connecting to the Smurfnet WiFi network)
-> Run *tmux*, then split into 3 different panes (quick Google should tell you, if not run *Ctrl-b "* and *Ctrl-b %* and navigate using *Ctrl-b o*)
-> The latest version of Nautical should already be uploaded onto the Arduino, but if it's not, go to ~/Nautical and run *sudo make upload*
-> Run *./comms.sh* in ~/Nautical so you can communicate to the Arduino
-> In one of the tmux panes, run *cat > /dev/ttyUSB0* and in the other run *cat < /dev/ttyUSB0*. The former is the input stream, where you send commands, while the latter is the output stream, where you get data back. To send a command, just type the command and press enter. The output will automatically appear in the pane with cat < /dev/ttyUSB0.

Preface:
Our sub has 6 degrees of freedom: X, Y, Z, Yaw, Pitch, and Roll. The angles are from 0 to 360 degrees. Each *State* object has 6 floats to represent where our sub is, in that order of X to Roll. Nautical's main job is to move from it's current State to another State accurately.

Commands:
-> *c* (prints out the sub's current state)
-> *d* (prints out the sub's desired state, or where it is trying to go)
-> *a* (prints out the whether or not the sub is killed, 0 = killed and 1 = alive)
-> *x* (cuts all motor power to the submarine)
-> *p %f* (used to set power between 0 and 1, should be between 0.1 and 0.2 for testing)
-> *s %f %f %f %f %f %f* (used to set a desired state, NOT relative to the current state)
Replace the %f with floats of the values you want to set.

Example input/output:
-> (input) *c* (output) *0 0 1 180 0 0* (the sub is at 0 in the X and Y direction, 1 meter down in the Z direction, and pointing at 180 degrees) 
-> (input) *d* (output) *1 1 1 180 0 0* (the sub is already trying to move to 1 unit for X and Y, 1 meter down in Z, and pointing at 180 degrees)
-> (input) *a* (output) *1* (the sub is alive meaning motors are on)
-> (input) *x* (no output, the sub's power is now at 0, and will not move unless you change power)
-> (input) *p 0.1* (no output, setting the sub's power to 0.1. note, this doesn't mean 10%, it's usually decently fast at around 0.1 to 0.2)
-> (input) *s 3 0 0 180 0 0* (no output, the sub will now ignore where it was trying to move to previously, and move to 3 units X, 0 units Y and Z, and point towards 180 degrees)
