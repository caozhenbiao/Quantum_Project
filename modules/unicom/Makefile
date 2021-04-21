CC = gcc
XX = g++
TARGET = ./bin/unicom.so
MYLIB  = ./include
CFLAGS = -I$(MYLIB) -Wall -O -g -I./libctb/include -I./libctb/include/ctb-0.16 -I../../platform/smartx/include -I../../platform/smartx/libs/lua/src -fPIC
LIBS = ../../platform/smartx/libs/libLua.a  -lpthread -lrt -ldl 
SHARED := --shared  

%.o: %.c                                 
	$(CC) $(CFLAGS) -c $< -o $@          

%.o:%.cpp
	$(XX) $(CFLAGS) -c $< -o $@

SOURCES = $(wildcard ../../platform/smartx/include/*.cpp ./kernel/*.cpp ./db/*.cpp ./db/*.c ./common/*.cpp ./module/*.cpp ./libctb/src/fifo.cpp ./libctb/src/getopt.cpp ./libctb/src/iobase.cpp ./libctb/src/portscan.cpp ./libctb/src/serportx.cpp ./libctb/src/linux/serport.cpp ./libctb/src/linux/timer.cpp interface.cpp disksvr.cpp machine.cpp )
OBJS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))  

$(TARGET) : $(OBJS)
	$(XX) $(SHARED) $(OBJS) -o $(TARGET) $(LIBS)
	chmod a+x $(TARGET)
clean:
	rm -rf $(OBJS) $(TARGET)

