内核符号导出，calculate内核的两个函数，hello用到了，首先要加载calculate，然后加载hello

insmod calculate.ko
insmod hello.ko
