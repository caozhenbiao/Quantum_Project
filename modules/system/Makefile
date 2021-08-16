CC = gcc
XX = g++
TARGET = ./output/system.so
CFLAGS =  -Wall -O -g  -I../platform/quark/liblua/src -I../../platform/quark/libbase -fPIC -std=c++11 
LIBS = -lpthread -lrt -ldl  ../../platform/quark/liblua/output/libLua.a ../../platform/quark/libbase/out/Release/obj.target/libbase.a -levent
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

