#使用这个模板可以使得某个目录下的所有带有main函数的源文件编译成为可执行的文件。
#例如目录bin下面有文件a.c, b.c, c.c，通过下面的模板make后，就生成了a,b,c三个可执行文件

#SRCS_C: 把当前目录下所有'.c'文件存入变量 SRCS_C 里	
SRCS_C = $(wildcard *.c)
OBJS_C = $(patsubst %.c, %.o, $(SRCS_C))
BUILD = $(patsubst %.c, %, $(SRCS_C))

.c:
	gcc -o $@ $< -g -Wall -lpthread -lrt 
	
all:$(BUILD)
	echo $(BUILD)

clean:
	rm -f $(BUILD)
	