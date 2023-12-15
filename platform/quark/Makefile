CC = gcc
XX = g++
TARGET = ./output/quark
CFLAGS = -Wall -O -g -I../include -I./liblua/src -Wall -Wno-unknown-pragmas -Wno-format -O3 -fPIC
LIBS = ./liblua/output/libLua.a -lpthread -lrt -ldl -llibssl

%.o: %.c                                 
	$(CC) $(CFLAGS) -c $< -o $@          

%.o:%.cpp
	$(XX) $(CFLAGS) -c $< -o $@

SOURCES = $(wildcard  ./include/map.c ./include/threadpool.c http.c  tcps.c ssls.c luadisp.c machine.c procmgr.c webui.c standalone.c )
OBJS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))  

$(TARGET) : $(OBJS)
	$(XX) $(OBJS) -o $(TARGET) $(LIBS)
	chmod a+x $(TARGET)

clean:
	rm -rf $(OBJS) $(TARGET)

