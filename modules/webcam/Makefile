CC = gcc
XX = g++
TARGET = ./output/webcam.so
CFLAGS = -Wall -O -g -I../../platform/quark/liblua/src -Wall -Wno-unknown-pragmas -Wno-format -O3 -fPIC
LIBS = -lpthread -lrt -ldl ../../platform/quark/liblua/output/libLua.a -lopencv_core -lopencv_imgproc  -lopencv_highgui 
SHARED := --shared  

%.o: %.c                                 
	$(CC) $(CFLAGS) -c $< -o $@          

%.o:%.cpp
	$(XX) $(CFLAGS) -c $< -o $@

SOURCES = $(wildcard cammgr.cpp camera.cpp pnppr.cpp onvifpr.cpp snaps.cpp interface.cpp event.cpp)
OBJS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))  

$(TARGET) : $(OBJS)
	$(XX) $(SHARED) $(OBJS) -o $(TARGET) $(LIBS)
	chmod a+x $(TARGET)
clean:
	rm -rf $(OBJS) $(TARGET)
