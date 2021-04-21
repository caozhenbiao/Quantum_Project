CC = gcc
XX = g++
TARGET = ./bin/webcam.so
CFLAGS = -Wall -O -g -I../../smartx/libs/lua/src -Wall -Wno-unknown-pragmas -Wno-format -O3 -fPIC
LIBS = -lpthread -lrt -ldl ../../smartx/libs/lua/src/liblua.a -lopencv_core -lopencv_imgproc  -lopencv_highgui 
SHARED := --shared  

%.o: %.c                                 
	$(CC) $(CFLAGS) -c $< -o $@          

%.o:%.cpp
	$(XX) $(CFLAGS) -c $< -o $@

SOURCES = $(wildcard cammgr.cpp camera.cpp pnppr.cpp onvifpr.cpp snaps.cpp interface.cpp ../../smartx/include/event.cpp)
OBJS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))  

$(TARGET) : $(OBJS)
	$(XX) $(SHARED) $(OBJS) -o $(TARGET) $(LIBS)
	chmod a+x $(TARGET)
clean:
	rm -rf $(OBJS) $(TARGET)
