This folder contains the source code for digispark-attiny85 to be used as volume control knob.
This firmware allows you to turn your digispark-attiny85 as volume-up/volume-down/mute usb-hid-keyboard.
this code is a modified version of: https://github.com/wagiminator/ATtiny85-TinyKnob.git 
digispark-attiny85 board: http://digistump.com/products/1
rotary-knob-board       : https://www.amazon.de/-/en/AZDelivery-KY-040-Encoder-Compatible-Arduino/dp/B07CMSHWV6 

connection between rotary-knob and digispart-attiny85
Rotary_encoder ==> Digispark-attiny85
           CLK ==> P2
           DT  ==> P0
           SW  ==> P1
           +   ==> 5V
           GND ==> GND

How to prepare digispark-attiny85 with micronucleus bootloader?
01)cd ~/
02)git clone https://github.com/hackboxguy/brbox.git
03)cd ~/brbox/sources/firmware/binaries/
04)ensure that avr-gcc and related tools are installed on your linux machine.
04)sudo avrdude  -c usbasp -p t85 -U flash:w:digispark-attiny85-micronucleus.hex -U lfuse:w:0xe1:m -U hfuse:w:0x5d:m -U efuse:w:0xfe:m
05)after programming digispark-attiny85 with micronucleus bootloader build micronucleus uploader utility using following commands

How to build micronucleus uploader utility?
01)sudo apt-get install libusb-1.0-0-dev
02)cd ~/brbox/sources/firmware/micronucleus-cmdline
03)gcc -o micronucleus littleWire_util.c micronucleus_lib.c micronucleus.c `pkg-config --libs --cflags libusb-1.0`

How to prepare attiny85-hid-rotary.hex(main.hex)?
01)cd ~/brbox/sources/firmware/attiny85-hid-rotary
02)make clean;make hex
03)ls and check if main.hex has been created

How to upload attiny85-hid-rotary.hex(main.hex) using micronucleusi-uploader-utility?
01)cd ~/brbox/sources/firmware/micronucleus-cmdline
02)ensure that digispark-attiny85 board is unplugged from your PC's usb
03)sudo ./micronucleus ../attiny85-hid-rotary/main.hex
04)plugin the digispark-attiny85 board in the usb of your linux pc wait for micronucleus to upload the hex file
05)after hex file updload is done, digispark-attiny85 will show up as input device under /dev/input(dmesg shall show "input,hidraw4: USB HID v1.01 Device")
06)turn the rotary knob and see if your pc's volume control menu shows up with increasing and decreasing volume
