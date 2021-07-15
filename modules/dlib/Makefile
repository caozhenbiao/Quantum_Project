CC = gcc
XX = g++
TARGET = ./output/dlib.so
CFLAGS = -Wall -O -g -I../../platform/quark/liblua/src -Wall -Wno-unknown-pragmas -Wno-format -O3 -fPIC -std=c++11
LIBS = -lpthread -lrt -ldl ../../platform/quark/liblua/output/libLua.a -ldlib -lopencv_core -lopencv_imgproc -lopencv_highgui
SHARED := --shared  -DLIB_NO_GUI_SUPPORT

%.o: %.c                                 
	$(CC) $(CFLAGS) -c $< -o $@          

%.o:%.cpp
	$(XX) $(CFLAGS) -c $< -o $@


SOURCES = $(wildcard event.cpp define.cpp interface.cpp dlibwraper.cpp)
OBJS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))  

$(TARGET) : $(OBJS)
	$(XX) $(SHARED) $(OBJS) -o $(TARGET) $(LIBS)
	chmod a+x $(TARGET)
clean:
	rm -rf $(OBJS) $(TARGET)

