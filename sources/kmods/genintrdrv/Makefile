CONFIG_MODULE_SIG=n
MODULENAME=genintrdrv
INCLDIR=$(PWD)/include/linux
obj-m += kmod/$(MODULENAME).o

#ccflags-y := -I$(INCLDIR)
#ifeq ("$(Debug)","yes")
#ccflags-y := -DGENINTRDRV_DRV_DEBUG
#endif

module:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
#debug: MY_CFLAGS+=-DGENINTRDRV_DRV_DEBUG
#debug:
#	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules EXTRA_CFLAGS="$(MY_CFLAGS)"

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

install:
	sudo insmod $(MODULENAME).ko
	sudo chmod 666 /dev/$(MODULENAME)

uninstall:
	sudo rmmod $(MODULENAME)
