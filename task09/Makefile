ifneq ($(KERNELRELEASE),)
	obj-m := sysfs_module.o
else
	KDIR := /usr/src/linux-headers-5.4.0-58-generic

all:
	$(MAKE) -C $(KDIR) M=$$PWD

clean:
	$(MAKE) -C $(KDIR) M=$$PWD clean
endif
