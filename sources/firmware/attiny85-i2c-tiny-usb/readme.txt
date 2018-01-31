how to prepare digispark attiny85 board as i2c-tiny-usb converter.

1)program firmware/micronucleus.hex bootloader in the digispark-attiny85-board using ponyser(or USBasp) programmer.
	avrdude  -P /dev/ttyUSB0 -c ponyser -p t85 -U flash:w:micronucleus.hex -U lfuse:w:0xe1:m -U hfuse:w:0x5d:m -U efuse:w:0xfe:m
2)plugin usb digispark board on host machine, and run : sudo ./micronucleus.exe i2c-tiny-usb-fmw.hex
3)after proramming the firmware, run dmesg, check if kernel detects this hardware

Note: i2c-tiny-usb-fmw.hex is from little-wire-versio-1.3

https://github.com/nopdotcom/i2c_tiny_usb-on-Little-Wire/releases
https://forum.openwrt.org/viewtopic.php?id=57325
