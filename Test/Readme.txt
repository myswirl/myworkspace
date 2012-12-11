在mingyangworkspace的test文件夹，存放Linux小程序，小例子，小知识点的例子实现
----------------------------------------------------------------------------------
1.compile.sh:
	编译文件夹下的所有源文件；
2.fileOperByC.c:
	C的API对文件操作的例子，打开文件，写入内容，读取内容，关闭文件；
3.fileOperByLinuxApi.c:
	Linux下的API，读写文件的例子；
4.wordCount.c:
	统计单词数码的小例子；
5.pthread_example_1.c:
	最简单的创建线程例子；
6.pthread_example_2.c:
	创建线程，并等待线程结束；
7.pthread_example_3.c:
	三种结束线程的方式；
8.pthread_example_4.c:
	多线程冲突触发，我们创建两个线程，各自把counter增加5000次，
	正常情况下最后counter应该等于10000，但事实上每次运行该程序的结果都不一样，
	有时候数到5000多，有时候数到6000多；
9.pthread_example_5.c:
	mutex解决多线程共享数据访问冲突；
10.pthread_example_6.c:
	Condition Variable,演示了一个生产者-消费者的例子，
	生产者生产一个结构体串在链表的表头上，消费者从表头取走结构体；
11.pthread_example_7.c:
	Semaphore,基于固定大小的环形队列重写生产者-消费者的例子；
12.showgdb.c:
	演示gdb的使用。
	gdb showgdb：调试运行showgdb
	run：运行程序
	kill：关闭程序
	quit：退出gdb
	i b：显示断点信息
	b doloop：设置断点
	disable 2：禁用断点2
	enable 1：使能断点1
	delete 1：删除断点1
	delete：删除所有断点
	step：单步执行，进入函数，相当于 step into
	stepi：汇编方式，单步执行，进入函数，相当于 step into
	next：单步执行，不进入函数，相当于step over
	nexti：汇编方式，单步执行，不进入函数，相当于step over
	finish：结束当前函数
	continue：继续执行到下一个断点；
	objdump -d showgdb > src:反汇编
	bt:查看堆栈信息
	info register：当前寄存器的值
	x/[n]x $esp：查看寄存器esp的值，以16进制连续显示n个
	x/[n]d $esp：查看寄存器esp的值，以10进制连续显示n个
	x/s 0x80481234：地址0x80481234的值，以字符串显示
	x/[n]d ($esp+2)：查看寄存器($esp+2)的值，以10进制连续显示n个
	wh：列出源码，显示当前调试位置，上下箭头刷新源码列表框
	where：显示当前调试位置
13.bitCount.c:
	计算无符号数种的bit为1 的个数。
14.reboot.c:
	重启Linux操作系统。
15.nestedComments:
	判断编译器是否支持嵌套注释。
16.bubble.c:
	冒泡排序算法演示。
17.optimize.c:
	演示程序优化编译选项
	-O 对程序进行优化编译、链接
	-O2 比-O更好的优化编译、链接
	gcc optimize.c -o optimize
	time ./optimize
	gcc -O optimize.c -o optimize
	time ./optimize
18.fileCopyByLinuxApi.c:
	文件拷贝，C语言实现，通过Linux API
19.getCurPath.c:
	获取当前工作路径。
20.timeView.c:
	时间显示相关函数。
21.timeUse:
	计算函数运行时间。
22.fork1.c:
	创建子进程fork()
23.fork2.c:
	演示fork创建的子进程拥有自己的数据段
24.vfork.c:
	演示vfork创建的子进程；
25.forkWait.c:
	进程等待
26.pipe_rw.c:
	有亲缘关系进程通讯，无名管道pipe。
27.fifo_write.c:
   fifo_read.c:
	有名管道FIFO，进程间通讯，fifo_write.c向FIFO写数据，fifo_read.c从FIFO读数据，
	管道中的数据读过就没有了。
28.signal.c:
	IPC方式，信号。
	1.运行./signal;
	2.查找signal进程号，ps -ef|grep signal
	3.向signal发送信号，kill -s SIGINT 3899
29.shareMemory.c:
	IPC方式,共享内存，进程间共享数据的一种最快的方法。
30.msgQueue.c:
	IPC方式，消息队列。
31.tcpClient.c:
	Socket实现tcp通讯，客户端。
32.tcpServer.c:
	Socket实现tcp通讯，服务端。
33.udpClient.c:
	socket实现udp通讯，客户端。
34.udpServer.c:
	socket实现udp通讯，网络端。
35.fileSizeOperByC.c:
	获取文件大小。
36.MemoryAssign.c:
	演示C程序内存分配。
	gcc MemoryAssign.c  -g -Wall -o MemoryAssign
	readelf -a MemoryAssign			//查看可执行文件内存分配情况, .text; .rodata; .data; .bss;

37.pipe_readtest.c:
	IPC方式，管道pipe演示。与pipe_rw.c相似
	管道详细介绍文档，IBM
	http://www.ibm.com/developerworks/cn/linux/l-ipc/part1/

38.msgQueue_Property.c:
	IPC方式,消息队列属性。

39.bitOper.c:
	bit操作，如果位操作符～和移位操作符<<(或>>)联合作用于 
	unsigned char 或者 unsigned short 类型的操作数时，
	中间运算步骤的结果必须立刻显式强制转换为预期的短整型数据类型。

40.sysconf.c:
	查看CPU及内存信息

41.sysVersion.c:
	获取操作系统版本信息

42.GetIPAddrList.c:
	获取IP地址列表
43.GetMAC.c:
	获取eth0的MAC地址

44.sm_read.c和sm_write.c:
	共享内存测试，注意编译选型-lrt
45.mytop.c:
	Linux下计算CPU的使用率
46.semaphore.c:
	IPC，进程间同步方式，信号灯演示
47.map_normalfile1.c:
   map_normalfile2.c:
	共享内存，map_normalfile1.c写数据，map_normalfile2.c读数据。
	./map_normalfile1 ttt
	./map_normalfile2 ttt
48.currentPlatform.c:
	测试当前系统平台
----------------------------------------------------------------------------------

