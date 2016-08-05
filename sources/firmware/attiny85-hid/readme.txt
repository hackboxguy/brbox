This folder contains the source code of digispark-attiny85 firmware,
This firmware allows you to turn your digispark board as 4-input usb-hid-keyboard.
digispark-attiny85 board: http://digistump.com/products/1

how to build and flash firmware:
1)ensure that avr-gcc and related tools are installed on your linux machine.
2)ensure that your digispark-attiny85 board is programmed with micronucleus bootloader. 
3)do "make hex" to generate main.hex file
4)sudo ./micronucleus main.hex ("Please plug in the device" will show-up, 
	now insert the board to usb port of your linux machine)
5)After successful flashing of main.hex, your digispark-attiny85 board will be
	detected by your linux machine as USB HID v1.01 Keyboard.
6)you need to connect four free pin's of digispark-attiny85 to four push-buttons between
	pullup resistors and GND. 

