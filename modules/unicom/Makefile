CC = gcc
XX = g++
TARGET = ./output/unicom.so
MYLIB  = ./include
CFLAGS = -I$(MYLIB) -Wall -O -g -I./ -I../sermon/libctb/include -I../sermon/libctb/include/ctb-0.16 -I../../platform/quark/liblua/src -fPIC
LIBS = ../../platform/quark/liblua/output/libLua.a  -lpthread -lrt -ldl 
SHARED := --shared  

%.o: %.c                                 
	$(CC) $(CFLAGS) -c $< -o $@          

%.o:%.cpp
	$(XX) $(CFLAGS) -c $< -o $@

SOURCES = $(wildcard ./kernel/*.cpp ./db/*.cpp ./db/*.c ./module/*.cpp ../sermon/libctb/src/fifo.cpp ../sermon/libctb/src/getopt.cpp ../sermon/libctb/src/iobase.cpp ../sermon/libctb/src/portscan.cpp ../sermon/libctb/src/serportx.cpp ../sermon/libctb/src/linux/serport.cpp ../sermon/libctb/src/linux/timer.cpp define.cpp event.cpp interface.cpp disksvr.cpp machine.cpp )
OBJS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))  

$(TARGET) : $(OBJS)
	$(XX) $(SHARED) $(OBJS) -o $(TARGET) $(LIBS)
	chmod a+x $(TARGET)
clean:
	rm -rf $(OBJS) $(TARGET)

