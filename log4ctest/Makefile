TARGET=log4ctest
OBJS=main.o other.o mylog.o
INC=./
LOG4CINC=/usr/local/include/log4c
LIBPATH=./

%.o:%.c
	g++ -c -I$(LOG4CINC) -I$(INC) $< -o $@

%.o:%.cpp 
	g++ -Wall -c -g -I$(INC) $< -o $@ 

$(TARGET):$(OBJS)
	g++  $(OBJS) -llog4c -o $(TARGET)

.PHONY:clean

clean:
	rm *.o $(TARGET)
	
	
