CONFIG_MODULE_SIG=n
MODULENAME=i2c-devantech-iss
INCLDIR=$(PWD)/include/linux
obj-m += kmod/$(MODULENAME).o

ifeq ("$(Cross)","yes")
	KDIR := /lib/modules/$(shell uname -r)/build #/home/adav/tmp/kernels/linux-omap-5f0a6e2
	ARCH := arm
	CROSS_COMPILE := /opt/brbox-raspi1-arm/usr/bin/arm-linux-
	MAKEARCH := make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE)
else
	KDIR := /lib/modules/$(shell uname -r)/build
	MAKEARCH := make 
endif

module:
	$(MAKEARCH) -C $(KDIR) M=$(PWD) modules
	#make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	$(MAKEARCH) -C $(KDIR) M=$(PWD) clean
	#make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

install:
	sudo insmod $(MODULENAME).ko
	sudo chmod 666 /dev/$(MODULENAME)

uninstall:
	sudo rmmod $(MODULENAME)

