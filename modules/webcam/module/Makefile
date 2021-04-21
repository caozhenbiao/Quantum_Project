CC = gcc
XX = g++
TARGET = ../bin/module.so
MYLIB  = ../include
CFLAGS = -I$(MYLIB) -Wall -O -g -I.. -I./libs -I../include -I../libs/lua/src -Wall -Wno-unknown-pragmas -Wno-format -O3 -fPIC
LIBS = -lpthread -lrt -ldl  ../libs/lua/src/liblua.a 
SHARED := --shared  

%.o: %.c                                 
	$(CC) $(CFLAGS) -c $< -o $@          

%.o:%.cpp
	$(XX) $(CFLAGS) -c $< -o $@

SOURCES = $(wildcard interface.cpp)
OBJS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))  

$(TARGET) : $(OBJS)
	$(XX) $(SHARED) $(OBJS) -o $(TARGET) $(LIBS)
	chmod a+x $(TARGET)
clean:
	rm -rf $(OBJS) $(TARGET)
