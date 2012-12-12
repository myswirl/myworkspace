KERNELDIR = /opt/FriendlyARM/mini2440/linux-2.6.29
PWD := $(shell pwd)
INSTALLDIR = /home/lmy/HelloWorld
CROSS_COMPILE=arm-linux-
CC=$(CROSS_COMPILE)gcc

obj-m := hello.o

.PHONY: modules modules_install clean

modules:

$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

modules_install:

cp hello.ko $(INSTALLDIR)

clean:

rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions