# i2csrv
i2csrv is a userspace daemon which allows accessing of i2c devices over tcp-socket via json-rpc mechanism.
Before running i2csrv, ensure that /dev/i2c-n exists, n should be the index of the bus number.

On raspberry pi, /dev/i2c-1 is accessible on following pins.
pin  1: 3.3vDC
pin  3: SDA1
pin  5: SCL1
pin 39: Ground

Before running i2csrv, its a good idea to scan available i2c devices on the bus using following command.
```i2cdetect -y 1```

Following command can be used to test the accessibility of PCF8574 having a address of 0x78
```i2cset -f -y 1 0x3c 0x00 0xff b``` (makes all pins of PCF8574 as high)
```i2cset -f -y 1 0x3c 0x00 0x00 b``` (makes all pins of PCF8574 as low)


