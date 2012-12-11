TARGET=MyLog
OBJS=main.o MyLog.o
INC=
LIBPATH=

%.o:%.cpp
	g++ -Wall -c -g -I$(INC) -I. $< -o $@ 

$(TARGET):$(OBJS)
	g++  $(OBJS) -o $(TARGET)

.PHONY:clean

clean:
	rm *.o $(TARGET)
	
	
