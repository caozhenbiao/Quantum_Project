CC = gcc
XX = g++
TARGET = ./output/tinyxml.so
CFLAGS = -Wall -O -g  -I../../platform/quark/liblua/src -Wall -Wno-unknown-pragmas -Wno-format -O3 -fPIC
LIBS   = -lpthread ../../platform/quark/liblua/output/libLua.a 
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
