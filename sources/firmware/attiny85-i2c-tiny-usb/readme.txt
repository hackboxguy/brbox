how to prepare digispark attiny85 board as i2c-tiny-usb converter.
1)make sure that red led is blinking on digistamp board.
2)using usbasp programmer, connect vcc/gnd/mosi/miso/sck/rst(6lines).
3)sudo avrdude  -c usbasp -p t85 -U flash:w:/home/user/sandbox/brbox/trunk/sources/firmware/binaries/digispark-attiny85-micronucleus.hex -U lfuse:w:0xe1:m -U hfuse:w:0x5d:m -U efuse:w:0xfe:m
4)cd ~/sandbox/brbox/trunk/sources/firmware/micronucleus-cmdline
5)sudo ./micronucleus ../attiny85-i2c-tiny-usb/i2c-tiny-usb-fmw.hex


Extra Notes:
1)program firmware/micronucleus.hex bootloader in the digispark-attiny85-board using ponyser(or USBasp) programmer.
	avrdude  -P /dev/ttyUSB0 -c ponyser -p t85 -U flash:w:micronucleus.hex -U lfuse:w:0xe1:m -U hfuse:w:0x5d:m -U efuse:w:0xfe:m
2)plugin usb digispark board on host machine, and run : sudo ./micronucleus.exe i2c-tiny-usb-fmw.hex
3)after proramming the firmware, run dmesg, check if kernel detects this hardware
Note: i2c-tiny-usb-fmw.hex is from little-wire-version-1.3
https://github.com/nopdotcom/i2c_tiny_usb-on-Little-Wire/releases
https://forum.openwrt.org/viewtopic.php?id=57325



