��mingyangworkspace��test�ļ��У����LinuxС����С���ӣ�С֪ʶ�������ʵ��
----------------------------------------------------------------------------------
1.compile.sh:
	�����ļ����µ�����Դ�ļ���
2.fileOperByC.c:
	C��API���ļ����������ӣ����ļ���д�����ݣ���ȡ���ݣ��ر��ļ���
3.fileOperByLinuxApi.c:
	Linux�µ�API����д�ļ������ӣ�
4.wordCount.c:
	ͳ�Ƶ��������С���ӣ�
5.pthread_example_1.c:
	��򵥵Ĵ����߳����ӣ�
6.pthread_example_2.c:
	�����̣߳����ȴ��߳̽�����
7.pthread_example_3.c:
	���ֽ����̵߳ķ�ʽ��
8.pthread_example_4.c:
	���̳߳�ͻ���������Ǵ��������̣߳����԰�counter����5000�Σ�
	������������counterӦ�õ���10000������ʵ��ÿ�����иó���Ľ������һ����
	��ʱ������5000�࣬��ʱ������6000�ࣻ
9.pthread_example_5.c:
	mutex������̹߳������ݷ��ʳ�ͻ��
10.pthread_example_6.c:
	Condition Variable,��ʾ��һ��������-�����ߵ����ӣ�
	����������һ���ṹ�崮������ı�ͷ�ϣ������ߴӱ�ͷȡ�߽ṹ�壻
11.pthread_example_7.c:
	Semaphore,���ڹ̶���С�Ļ��ζ�����д������-�����ߵ����ӣ�
12.showgdb.c:
	��ʾgdb��ʹ�á�
	gdb showgdb����������showgdb
	run�����г���
	kill���رճ���
	quit���˳�gdb
	i b����ʾ�ϵ���Ϣ
	b doloop�����öϵ�
	disable 2�����öϵ�2
	enable 1��ʹ�ܶϵ�1
	delete 1��ɾ���ϵ�1
	delete��ɾ�����жϵ�
	step������ִ�У����뺯�����൱�� step into
	stepi����෽ʽ������ִ�У����뺯�����൱�� step into
	next������ִ�У������뺯�����൱��step over
	nexti����෽ʽ������ִ�У������뺯�����൱��step over
	finish��������ǰ����
	continue������ִ�е���һ���ϵ㣻
	objdump -d showgdb > src:�����
	bt:�鿴��ջ��Ϣ
	info register����ǰ�Ĵ�����ֵ
	x/[n]x $esp���鿴�Ĵ���esp��ֵ����16����������ʾn��
	x/[n]d $esp���鿴�Ĵ���esp��ֵ����10����������ʾn��
	x/s 0x80481234����ַ0x80481234��ֵ�����ַ�����ʾ
	x/[n]d ($esp+2)���鿴�Ĵ���($esp+2)��ֵ����10����������ʾn��
	wh���г�Դ�룬��ʾ��ǰ����λ�ã����¼�ͷˢ��Դ���б��
	where����ʾ��ǰ����λ��
13.bitCount.c:
	�����޷������ֵ�bitΪ1 �ĸ�����
14.reboot.c:
	����Linux����ϵͳ��
15.nestedComments:
	�жϱ������Ƿ�֧��Ƕ��ע�͡�
16.bubble.c:
	ð�������㷨��ʾ��
17.optimize.c:
	��ʾ�����Ż�����ѡ��
	-O �Գ�������Ż����롢����
	-O2 ��-O���õ��Ż����롢����
	gcc optimize.c -o optimize
	time ./optimize
	gcc -O optimize.c -o optimize
	time ./optimize
18.fileCopyByLinuxApi.c:
	�ļ�������C����ʵ�֣�ͨ��Linux API
19.getCurPath.c:
	��ȡ��ǰ����·����
20.timeView.c:
	ʱ����ʾ��غ�����
21.timeUse:
	���㺯������ʱ�䡣
22.fork1.c:
	�����ӽ���fork()
23.fork2.c:
	��ʾfork�������ӽ���ӵ���Լ������ݶ�
24.vfork.c:
	��ʾvfork�������ӽ��̣�
25.forkWait.c:
	���̵ȴ�
26.pipe_rw.c:
	����Ե��ϵ����ͨѶ�������ܵ�pipe��
27.fifo_write.c:
   fifo_read.c:
	�����ܵ�FIFO�����̼�ͨѶ��fifo_write.c��FIFOд���ݣ�fifo_read.c��FIFO�����ݣ�
	�ܵ��е����ݶ�����û���ˡ�
28.signal.c:
	IPC��ʽ���źš�
	1.����./signal;
	2.����signal���̺ţ�ps -ef|grep signal
	3.��signal�����źţ�kill -s SIGINT 3899
29.shareMemory.c:
	IPC��ʽ,�����ڴ棬���̼乲�����ݵ�һ�����ķ�����
30.msgQueue.c:
	IPC��ʽ����Ϣ���С�
31.tcpClient.c:
	Socketʵ��tcpͨѶ���ͻ��ˡ�
32.tcpServer.c:
	Socketʵ��tcpͨѶ������ˡ�
33.udpClient.c:
	socketʵ��udpͨѶ���ͻ��ˡ�
34.udpServer.c:
	socketʵ��udpͨѶ������ˡ�
35.fileSizeOperByC.c:
	��ȡ�ļ���С��
36.MemoryAssign.c:
	��ʾC�����ڴ���䡣
	gcc MemoryAssign.c  -g -Wall -o MemoryAssign
	readelf -a MemoryAssign			//�鿴��ִ���ļ��ڴ�������, .text; .rodata; .data; .bss;

37.pipe_readtest.c:
	IPC��ʽ���ܵ�pipe��ʾ����pipe_rw.c����
	�ܵ���ϸ�����ĵ���IBM
	http://www.ibm.com/developerworks/cn/linux/l-ipc/part1/

38.msgQueue_Property.c:
	IPC��ʽ,��Ϣ�������ԡ�

39.bitOper.c:
	bit���������λ������������λ������<<(��>>)���������� 
	unsigned char ���� unsigned short ���͵Ĳ�����ʱ��
	�м����㲽��Ľ������������ʽǿ��ת��ΪԤ�ڵĶ������������͡�

40.sysconf.c:
	�鿴CPU���ڴ���Ϣ

41.sysVersion.c:
	��ȡ����ϵͳ�汾��Ϣ

42.GetIPAddrList.c:
	��ȡIP��ַ�б�
43.GetMAC.c:
	��ȡeth0��MAC��ַ

44.sm_read.c��sm_write.c:
	�����ڴ���ԣ�ע�����ѡ��-lrt
45.mytop.c:
	Linux�¼���CPU��ʹ����
46.semaphore.c:
	IPC�����̼�ͬ����ʽ���źŵ���ʾ
47.map_normalfile1.c:
   map_normalfile2.c:
	�����ڴ棬map_normalfile1.cд���ݣ�map_normalfile2.c�����ݡ�
	./map_normalfile1 ttt
	./map_normalfile2 ttt
48.currentPlatform.c:
	���Ե�ǰϵͳƽ̨
----------------------------------------------------------------------------------

