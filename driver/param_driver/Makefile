#
# makefile for hello deiver
#
ifneq ($(KERNELRELEASE),)

EXTRA_CFLAGS := -I/$(src)/../inc 
obj-m := param.o
param-objs := param_driver.o 

else
PWD  := $(shell pwd)
KVER ?= $(shell uname -r)
KDIR := /lib/modules/$(KVER)/build

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

install:
	insmod param.ko

clean:
	rm -rf .*.cmd *.o *.mod.c *.ko .tmp_versions
endif

