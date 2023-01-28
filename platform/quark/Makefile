CC = gcc
XX = g++
TARGET = ./output/quark
CFLAGS = -Wall -O -g -I../include -I./liblua/src -Wall -Wno-unknown-pragmas -Wno-format -O3 -fPIC
LIBS = ./liblua/output/libLua.a -lpthread -lrt -ldl

%.o: %.c                                 
	$(CC) $(CFLAGS) -c $< -o $@          

%.o:%.cpp
	$(XX) $(CFLAGS) -c $< -o $@

SOURCES = $(wildcard ./include/http.c ./include/map.c ./include/tcps.c ./include/threadpool.c luadisp.c machine.c procmgr.c webui.c standalone.c )
OBJS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))  

$(TARGET) : $(OBJS)
	$(XX) $(OBJS) -o $(TARGET) $(LIBS)
	chmod a+x $(TARGET)

clean:
	rm -rf $(OBJS) $(TARGET)

