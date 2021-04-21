CC = gcc
XX = g++
TARGET = ./output/sermon.so
MYLIB  = ./include
CFLAGS = -I$(MYLIB) -Wall -O -g -I./libctb/include -I./libctb/include/ctb-0.16 -I./libcap/linux -I../../platform/quark/liblua/src -fPIC
LIBS = ../../platform/quark/liblua/output/libLua.a  ./libcap/linux/libpcap.a -lpthread -lrt -ldl 
SHARED := --shared  

%.o: %.c                                 
	$(CC) $(CFLAGS) -c $< -o $@          

%.o:%.cpp
	$(XX) $(CFLAGS) -c $< -o $@

SOURCES = $(wildcard ./kernel/*.cpp ./db/*.cpp ./db/*.c ./common/*.cpp ./module/*.cpp ./libctb/src/fifo.cpp ./libctb/src/getopt.cpp ./libctb/src/iobase.cpp ./libctb/src/portscan.cpp ./libctb/src/serportx.cpp ./libctb/src/linux/serport.cpp ./libctb/src/linux/timer.cpp interface.cpp disksvr.cpp machine.cpp )
OBJS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))  

$(TARGET) : $(OBJS)
	$(XX) $(SHARED) $(OBJS) -o $(TARGET) $(LIBS)
	chmod a+x $(TARGET)
clean:
	rm -rf $(OBJS) $(TARGET)

