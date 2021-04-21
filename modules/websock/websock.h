#ifndef _WEBSOCK_H_
#define _WEBSOCK_H_
#include <string>
#include <map>
#include <vector>
#ifdef WIN32
#include <process.h>
#include <winsock2.h>
#define WIN32_LEAN_AND_MEAN  //winsock.h winsock2.h
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
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <arpa/inet.h>
#include <sys/unistd.h>
#include <sys/ioctl.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <net/if.h>
#define winapi void*
#define SOCKET int
#define closesocket close
#endif

 
#ifdef WIN32
#include <time.h>
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <arpa/inet.h>
#include <sys/unistd.h>
#include <sys/ioctl.h>
#include <sys/fcntl.h>
#define MAX_PATH 256
#endif
#define MAX_CLIENT 512
#define MAX_BUF_SIZE 1024*256


#ifdef WIN32
#include <process.h>
#endif

#include "event.h"

//websocket
enum WS_FrameType{
    WS_EMPTY_FRAME = 0xF0,
    WS_ERROR_FRAME = 0xF1,
    WS_TEXT_FRAME = 0x01,
    WS_BINARY_FRAME = 0x02,
    WS_PING_FRAME = 0x09,
    WS_PONG_FRAME = 0x0A,
    WS_OPENING_FRAME = 0xF3,
    WS_CLOSING_FRAME = 0x08
};

class websock{
public:
	int decodeframe(const char* frm, size_t len, std::string &outMessage);
	std::string base64_encode(unsigned char* data, unsigned int in_len);
	int handshake(const char* key, char* rsp );
	bool anyhttphead(char* buf, size_t len, std::map<std::string,std::string>& argv);
	int getip(const char* nc, char* ip);

public:
	int start(const char* ip, short port);
	int stop();
	int sends(unsigned char* buf,size_t len);
	int sends(const char* str);
	
private:
	SOCKET  myserver;
	SOCKET  myclient;
	SOCKET  mysocket;

	static winapi sending(void* param);
	pthread_t threadid;
	event_handle mytaskcs;
	event_handle workevent;
	bool m_bExit;
	std::vector<std::string> vsendbuf;
	int gettrans( std::vector<std::string> &tv );
	int fdarray[MAX_CLIENT];

private:
	static winapi workthread(void* lpParam);
	bool dispath(SOCKET sock, char* buf, long len);
};
#endif
