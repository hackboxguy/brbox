make clean (check for Debug flag).
make (under kmod/i2c-stub.ko will be created)
ls /dev/i2c* (note down existing device nodes)
sudo insmod kmod/i2c-stub.ko chip_addr=0x50  (0x50 is the virtual eeprom address - 0xA0)
ls /dev/i2c* (after inserting i2c-stub module, note the extra node that has been created)
sudo i2cdetect -y 0 (should show device 0x50 as available).
sudo i2cset -f -y 0 0x50 0x00 0x55 b (write 0x55 to first byte)
sudo ./eeprog -x /dev/i2c-0 0x50 -r 0:64 (will print out 64bytes or eeprom, observe that first byte is 55, and rest are 0).
sudo ./eeprog /dev/i2c-0 0x50 -r 0 -x (read 1 byte at i2c-slave-addr=0x50 and sub-addr=0)
date | sudo ./eeprog -f -8 -w 0  /dev/i2c-0 0x50 (write stdout of date command to i2c-slave-addr=0x50, starting at 0th location)
sudo ./eeprog -x /dev/i2c-0 0x50 -r 0:64 (readback previously written date).

dd if=/dev/urandom bs=1 count=256 > 256.dump
sudo ./eeprog -f -8  -w 0x00  /dev/i2c-0 0x50 < 256.dump
sudo ./eeprog -f -x /dev/i2c-0 0x50 -r 0:256 (-f is for suppress warning message).

24C256 I2C EEPROM (use -16 addressing)
24C02  I2C EEPROM (use -8 bit addressing).
#################################################################
!!!!!!!!!!!!warning about eeprom tool!!!!!!!!!!!!
Writing on unknown EEPROMs can brake your computer. 
DIMMs contain an EEPROM and if you overwrite it somehow your computer 
will not boot anymore.
Reading using 16bit addressing (i.e. not using the -8 switch) on a 8bit EEPROM 
can actually WRITE to the EEPROM. Be careful. 
The following chips use 8bit mode:
	24C01
	24C02
	24C04
	24C08
	24C16
Bigger ones use 16bit addressing so you must not use -8.
More could need it, check data sheets.
If you are not sure about what you're doing DON'T use this tool.
#################################################################

