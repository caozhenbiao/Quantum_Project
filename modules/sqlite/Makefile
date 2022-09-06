CC = gcc
XX = g++
TARGET = ./tlstest.so
MYLIB  = 
CFLAGS = -I$(MYLIB) -Wall -O -g -I. -I/home/caozhenbiao/Downloads/SSL/GmSSL-develop/include
LIBS = -lpthread -lrt -ldl  /home/caozhenbiao/Downloads/SSL/GmSSL-develop/build/lib/libgmssl.a 
SHARED := --shared  

%.o: %.c                                 
	$(CC) $(CFLAGS) -c $< -o $@          

%.o:%.cpp
	$(XX) $(CFLAGS) -c $< -o $@

SOURCES = $(wildcard tlstest.c )
OBJS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))  

$(TARGET) : $(OBJS)
	$(XX) $(SHARED) $(OBJS) -o $(TARGET) $(LIBS)
	chmod a+x $(TARGET)
clean:
	rm -rf $(OBJS) $(TARGET)
