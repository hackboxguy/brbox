## HID-Volume-Control-Knob with Digispark-ATtiny85
![Volume Control Knob.](/images/volume-control-knob.png "Volume Control Knob.")

## Required Items
![Required Items.](/images/required-items.jpg "Required Items.")

## Connection
![Connection Diagram.](/images/connection-diagram.jpg "Connection Diagram.")

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
7. `sudo ./micronucleus --info` (will show the current micronucleus version)
8. Remove Digispark-ATtiny85 from USB Port
9. `sudo ./micronucleus --run ../firmware/upgrades/upgrade-t85_default.hex`
10. Insert Digispark-ATtiny85 to USB Port - New micronucleus bootloader ver-2.6 will be flashed
11. Remove Digispark-ATtiny85 from USB Port
12. `sudo ./micronucleus --run ../../hexfiles/attiny85-volume-control-hid.hex`
13. Insert Digispark-ATtiny85 to USB Port - hid-volume-control firmware will be flashed
14. Rotate or press knob to see volume function of your PC being controlled
