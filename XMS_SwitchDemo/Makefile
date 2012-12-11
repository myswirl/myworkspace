TARGET=switchdemo
OBJS=cini.o tools.o myncurses.o MyLog.o $(CASPATH)/CAS_Common_Cfg.o $(ANAPATH)/Analog_Common_Code.o $(ANAPATH)/ITPDataStandard.o XMS_SwitchDemo_String.o XMS_SwitchDemo_Event.o XMS_SwitchDemo_Sub.o
INC=../../inc
LIBPATH=../../dll
CASPATH=../CAS_Common_Code
ANAPATH=../Analog_Common_Code

%.o:%.cpp
	g++ -c -g -I$(INC) -I. -I$(CASPATH)/CAS_Common_cfg.o $< -o $@ 

$(TARGET):$(OBJS)
	g++ $(OBJS) -L$(LIBPATH) -lDJITPCom -lDJAcsAPI -lcurses -o $(TARGET)

clean:
	rm *.o
	rm $(TARGET)
	
