how to compile:
sudo apt-get install libusb-1.0-0-dev
gcc -o micronucleus littleWire_util.c micronucleus_lib.c micronucleus.c `pkg-config --libs --cflags libusb-1.0`
