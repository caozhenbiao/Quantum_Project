CC = gcc
XX = g++
TARGET = ./bin/dlib.so
CFLAGS = -Wall -O -g -I../../smartx/libs/lua/src -I./dlib-19.17 -Wall -Wno-unknown-pragmas -Wno-format -O3 -fPIC -std=c++11
LIBS = -lpthread -lrt -ldl ../../smartx/libs/lua/src/liblua.a ./dlib-19.17/dlib/libdlib.a -lopencv_core -lopencv_imgproc -lopencv_highgui
SHARED := --shared  

%.o: %.c                                 
	$(CC) $(CFLAGS) -c $< -o $@          

%.o:%.cpp
	$(XX) $(CFLAGS) -c $< -o $@


SOURCES = $(wildcard ../../smartx/include/event.cpp ../../smartx/include/define.cpp interface.cpp dlibwraper.cpp)
OBJS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))  

$(TARGET) : $(OBJS)
	$(XX) $(SHARED) $(OBJS) -o $(TARGET) $(LIBS)
	chmod a+x $(TARGET)
clean:
	rm -rf $(OBJS) $(TARGET)

