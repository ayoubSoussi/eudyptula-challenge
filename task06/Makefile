ifneq ($(KERNELRELEASE),)
	obj-m := misc_driver_module.o
else
	KDIR := /usr/src/linux-headers-5.4.0-54-generic

all:
	$(MAKE) -C $(KDIR) M=$$PWD

clean:
	$(MAKE) -C $(KDIR) M=$$PWD clean	
endif
