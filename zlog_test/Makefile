# 首先用函数wildcard获取工作目录下的.cpp文件列表，之后将.cpp替换为.o
OBJS:= $(patsubst %.cpp,%.o,$(wildcard *.cpp)) 
TARGET=zlog_test

INCPATH=./
LIBPATH=./

# $<:所有依赖文件，$@:目标文件，$<:代表第一个依赖文件，命令前面加@:取消屏幕回显
%.o:%.cpp
	g++ -Wall -c -g -I$(INCPATH) -I. $< -o $@ 

$(TARGET):$(OBJS)
	g++  $(OBJS) -lzlog -lpthread -o $(TARGET)
	
# PHONY伪目标
.PHONY:clean
clean:
	rm -f *.o $(TARGET)
	