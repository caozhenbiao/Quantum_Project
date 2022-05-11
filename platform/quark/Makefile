CC = gcc 
XX = g++
TARGET = ./output/quark
CFLAGS = -Wall -O -g -I./include -I./libbase -I./liblua/src -fPIC -std=c++11 
LIBS   = ./libbase/output/libbase.a ./liblua/output/libLua.a -levent -lpthread -lrt -ldl

%.o: %.c                                 
	$(CC) $(CFLAGS) -c $< -o $@          

%.o:%.cpp
	$(XX) $(CFLAGS) -c $< -o $@

SOURCES = $(wildcard *.c sha1.cpp attestor.cpp commsvr.cpp business.cpp websvr.cpp taskmgr.cpp machine.cpp main.cpp)
OBJS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))  

$(TARGET) : $(OBJS)
	$(XX) $(OBJS) -o $(TARGET) $(LIBS)
	chmod a+x $(TARGET)

clean:
	rm -rf $(OBJS) $(TARGET)
