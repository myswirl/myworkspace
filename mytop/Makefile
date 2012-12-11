TARGET=mytop
OBJS=main.o m_linux.o utils.o
INC=
LIBPATH=

%.o:%.c
	g++ -Wall -c -g -I$(INC) -I. $< -o $@ 

$(TARGET):$(OBJS)
	g++  $(OBJS) -o $(TARGET)

.PHONY:clean

clean:
	rm *.o $(TARGET)
	
	
