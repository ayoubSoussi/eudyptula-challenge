ifneq ($(KERNELRELEASE),)
	obj-m := linked_list_mod.o
else
	KDIR := /usr/src/linux-headers-5.4.0-58-generic

all:
	$(MAKE) -C $(KDIR) M=$$PWD

clean:
	$(MAKE) -C $(KDIR) M=$$PWD clean	
endif
