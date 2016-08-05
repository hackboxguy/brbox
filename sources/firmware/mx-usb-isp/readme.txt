1)download progisp1.72.rar (http://www.electrodragon.com/w/ProgISP)
2)download fernando's usbAsp hex for atmega88 (http://www.sciencetronics.com/greenphotons/?p=938)
3)fernando hexfile download link: https://www.dropbox.com/s/88zve7kq2mc0h38/ATmega88.hex?dl=0
4)plugin working mx-usbisp programmer to windows machine.
5)connect flat ribbon cable between two mx-usbisp's
6)short <-up-> jumper on the target mx-usbisp board(open the aluminium casing).
7)start progisp1.72 program, and select atmega88 device and fuse value should be 0xF9DFFF
8)press auto button, after programming, remove the jumper.
9)connect newly updated target mx-usb-isp to linux pc, do dmesg, usbasp device should be detected.


how to convert ms-usb-isp to usp-i2c converter?
http://homes-smart.ru/index.php/oborudovanie/prochee/i2c-usb-perekhodnik
