#Comment/uncomment the following line to disable/enable debugging
DEBUG = y

ifeq ($(DEBUG),y)  
	DEBFLAGS = -O -g -DSCULL_DEBUG 
else  
	DEBFLAGS = -O2
endif

# 如果已定义KERNELRELEASE，则说明是从内核构造系统调用的，因此可以利用其内建语句
ifneq ($(KERNELRELEASE),)
	obj-m := scull.o 
	
# 否则，是直接从命令行调用的，这时要调用内核构造系统
else
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd) 
	
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	
clean:
	rm -rf .*.cmd *.o *.mod.c *.ko .tmp_versions	

endif 

