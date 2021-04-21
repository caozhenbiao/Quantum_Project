#ifndef _SNAPC_H_
#define _SNAPC_H_
#ifdef WIN32
#include <process.h>
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <arpa/inet.h>
#include <sys/unistd.h>
#include <sys/ioctl.h>
#include <sys/fcntl.h>
#endif
#include <deque>
#include <vector>
#include <string>
#include <map>

#include "event.h"
#include <opencv2/opencv.hpp>

#ifdef WIN32
#include <process.h>
#define WIN32_LEAN_AND_MEAN  //这很重要，解决winsock.h winsock2.h冲突
//#include <windows.h> 
#define MKDIR(a) _mkdir((a))   
#define winapi unsigned int __stdcall
#define socklen_t int
#define pthread_t unsigned int
#else
#include <time.h>
#include <stdarg.h>   
#include <sys/stat.h> 
#define ACCESS access   
#define MKDIR(a) mkdir((a),0755)   
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define __int64 long long
#define Sleep(x) usleep(x * 1000)
#include <sys/vfs.h>
#include <mntent.h>
#include <dirent.h>
#include <sys/types.h>
#define winapi void*
#define SOCKET int
#define closesocket close
#endif

class csnapc{
public:
	bool play( const char * args );
	bool stop();
private:
	void dowork();
private:
	SOCKET myserver;
	SOCKET myclient;
	sockaddr_in mydestaddr;
private:
	static winapi playthread(void* lpParam);
	pthread_t mythreadid;
	event_handle mysnapevt;
	event_handle myplayevt;
	bool mybExit;
	bool mybConn;
private:
	IplImage*  myimage;
	HDC myScreenDC;
	HDC myCompDC;
	std::vector<unsigned char> myimgbuf;
	int myfrmtop;
	int myfrmleft;
	int myfrmwidth;
	int myfrmheight;
};
#endif
