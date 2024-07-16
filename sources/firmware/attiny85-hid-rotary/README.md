## HID-Volume-Control-Knob with Digispark-ATtiny85
![Volume Control Knob.](/sources/firmware/attiny85-hid-rotary/images/volume-control-knob.jpg "Volume Control Knob.")

## Required Items
![Required Items.](/sources/firmware/attiny85-hid-rotary/images/required-items.jpg "Required Items.")

## Connection
![Connection Diagram.](/sources/firmware/attiny85-hid-rotary/images/connection-diagram.jpg "Connection Diagram.")

| Rotary-Encoder| Digispark-ATtiny85 |
| ------------- |:------------------:|
| CLK           | P5                 |
| DT            | P2                 |
| SW            | P0                 |
| +             | 5V                 |
| GND           | GND                |

## Flashing Digispark-ATtiny85 with latest micronucleus-bootloader and hid-volume-control firmware
1. `sudo apt install libusb-dev`
2. `git clone https://github.com/hackboxguy/brbox.git`
3. `cd brbox/sources/firmware/attiny85-hid-rotary/`
4. `git clone https://github.com/micronucleus/micronucleus.git`
5. `cd micronucleus/commandline`
6. `make`
7. run `sudo ./micronucleus --info` and insert Disispark-ATtiny85 to USB Port - will show the current micronucleus version
8. Remove Digispark-ATtiny85 from USB Port
9. `sudo ./micronucleus --run ../firmware/upgrades/upgrade-t85_default.hex`
10. Insert Digispark-ATtiny85 to USB Port - New micronucleus bootloader ver-2.6 will be flashed
11. Remove Digispark-ATtiny85 from USB Port
12. `sudo ./micronucleus --run ../../hexfiles/attiny85-volume-control-hid.hex`
13. Insert Digispark-ATtiny85 to USB Port - hid-volume-control firmware will be flashed
14. Rotate or press knob to see volume function of your PC being controlled


## What If the steps above doesnt work?
1. Unplug and Replug Digispark-ATtiny85 to the USB Port, try `dmesg` check if this prints `device descriptor read/64, error -71`
2. Incase if you see device descriptor read error on dmesg output, try flashing your Digispark-ATtiny85 with following command,
3. `sudo avrdude -c usbasp -p t85 -U flash:w:../firmware/upgrades/upgrade-t85_default.hex -U lfuse:w:0xe1:m -U hfuse:w:0x5d:m -U efuse:w:0xfe:m`
4. After flashing with avrdude, try the flashing steps again from 8 to 13 as given above
