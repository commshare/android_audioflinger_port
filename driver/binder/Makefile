ifneq ($(KERNELRELEASE),)
obj-m           := binder_driver.o
binder_driver-y	:= deps.o binder.o
else
KDIR	?= /usr/src/linux-2.6.39/

all:
	$(MAKE) -C $(KDIR) V=0 M=$$PWD

clean:
	rm -rf *.o *.ko *.mod.c *.symvers *.order .*.cmd .tmp_versions
endif
