On Marlin there are added commands:
	upload: platformio run -t upload
	monitor: platformio device monitor


# initialize directory - only on first creation
platformio init --board megaatmega2560

# can initialize with multiple boards
platformio init --board megaatmega2560 --board uno

# just build - no upload
platformio run

# upload to device
platformio run -t upload

# if multiple boards, can choose which one to run on
platformio run -e uno -t upload

# For information on configuration of .ini file
> http://docs.platformio.org/en/latest/projectconf.html#projectconf

