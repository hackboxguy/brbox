CONFIG_MODULE_SIG=n
MODULENAME=genintrdrv
INCLDIR=$(PWD)/include/linux
obj-m += kmod/$(MODULENAME).o

ccflags-y := -I$(INCLDIR)

module:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

install:
	sudo insmod $(MODULENAME).ko
	sudo chmod 666 /dev/$(MODULENAME)

uninstall:
	sudo rmmod $(MODULENAME)
