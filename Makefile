all:
	platformio -f -c vim run

upload:
	sudo chmod 666 /dev/ttyACM*
	platformio -f -c vim run --target upload
	sudo stty -F /dev/ttyACM* 9600 raw -clocal -echo

clean:
	platformio -f -c vim run --target clean

program:
	platformio -f -c vim run --target program

uploadfs:
	platformio -f -c vim run --target uploadfs

update:
	platformio -f -c vim update
