#!/bin/sh

gcc fileOperByC.c -g -Wall  -o fileOperByC

gcc fileSizeOperByC.c -g -Wall  -o fileSizeOperByC

gcc fileOperByLinuxApi.c  -g -Wall -o fileOperByLinuxApi

gcc fileCopyByLinuxApi.c  -g -Wall -o fileCopyByLinuxApi

gcc keywordCount.c  -g -Wall -o keywordCount

gcc wordCount.c  -g -Wall -o wordCount

gcc pthread_example_1.c  -g -Wall -lpthread -o pthread_example_1

gcc pthread_example_2.c  -g -Wall -lpthread -o pthread_example_2

gcc pthread_example_3.c  -g -Wall -lpthread -o pthread_example_3

gcc pthread_example_4.c  -g -Wall -lpthread -o pthread_example_4

gcc pthread_example_5.c  -g -Wall -lpthread -o pthread_example_5

gcc pthread_example_6.c  -g -Wall -lpthread -o pthread_example_6

gcc pthread_example_7.c  -g -Wall -lpthread -o pthread_example_7

gcc showgdb.c -g -Wall -o showgdb

gcc bitCount.c -g -Wall -o bitCount

gcc reboot.c -g -Wall -o reboot

gcc nestedComments.c -g -Wall -o nestedComments

gcc bubble.c -g -Wall -o bubble

gcc optimize.c -o optimize
#gcc -O optimize.c -o optimize

gcc getCurPath.c  -g -Wall -o getCurPath

gcc timeView.c  -g -Wall -o timeView

gcc timeUse.c  -g -Wall -o timeUse

gcc fork1.c  -g -Wall -o fork1

gcc fork2.c  -g -Wall -o fork2

gcc vfork.c  -g -Wall -o vfork

gcc forkWait.c  -g -Wall -o forkWait

gcc pipe_rw.c  -g -Wall -o pipe_rw
gcc pipe_readtest.c -g -Wall -o pipe_readtest

gcc fifo_read.c  -g -Wall -o fifo_read

gcc fifo_write.c  -g -Wall -o fifo_write

gcc signal.c  -g -Wall -o signal

gcc shareMemory.c  -g -Wall -o shareMemory

gcc msgQueue.c  -g -Wall -o msgQueue

gcc tcpClient.c  -g -Wall -o tcpClient

gcc tcpServer.c  -g -Wall -o tcpServer

gcc udpClient.c  -g -Wall -o udpClient

gcc udpServer.c  -g -Wall -o udpServer

gcc MemoryAssign.c  -g -Wall -o MemoryAssign

gcc msgQueue_Property.c -g -Wall -o msgQueue_Property

gcc bitOper.c -g -Wall -o bitOper

gcc sysconf.c -g -Wall -o sysconf

gcc sysVersion.c -g -Wall -o sysVersion

gcc GetIPAddrList.c -g -Wall -o GetIPAddrList

gcc GetMAC.c -g -Wall -o GetMAC

gcc -Wall sm_read.c -lrt -o sm_read

gcc -Wall sm_write.c -lrt -o sm_write

gcc -Wall environment.c -o environment

