SOURCES := littleWire_util.c micronucleus.c micronucleus_lib.c
HEADERS := littleWire_util.h micronucleus_lib.h
LFLAGS += `pkg-config --libs --cflags libusb-1.0`

all:	 micronucleus 

micronucleus:	$(SOURCES) $(HEADERS)
	$(CROSS_COMPILE)gcc $(I386_FLAG) -Wall -DUSE_HOSTCC $(SOURCES) $(LFLAGS) -o micronucleus

.PHONY: version

clean:
	rm -f micronucleus

