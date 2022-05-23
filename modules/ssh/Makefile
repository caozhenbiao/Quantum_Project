CC = gcc
XX = g++
TARGET = ./output/ssh.so
CFLAGS = -I$(MYLIB) -Wall -O -g -I. -I../platform/quark/liblua/src -I../../platform/quark/libbase  -Wall -Wno-unknown-pragmas -Wno-format -O3 -fPIC -std=c++11 
LIBS =  -lrt -ldl ../../platform/quark/liblua/output/libLua.a  ../../platform/quark/libbase/output/libbase.a -lssh2 -levent
SHARED := --shared  

%.o: %.c                                 
	$(CC) $(CFLAGS) -c $< -o $@          

%.o:%.cpp
	$(XX) $(CFLAGS) -c $< -o $@

SOURCES = $(wildcard *.c *.cpp)
OBJS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))  

$(TARGET) : $(OBJS)
	$(XX) $(SHARED) $(OBJS) -o $(TARGET) $(LIBS)
	chmod a+x $(TARGET)
clean:
	rm -rf $(OBJS) $(TARGET)

