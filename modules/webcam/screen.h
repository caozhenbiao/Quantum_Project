#ifndef _SCREEN_H_
#define _SCREEN_H_
#ifdef _WIN32
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
#include "cammgr.h"
#include <opencv2/opencv.hpp>

class mbase;
class cscreen : public mbase{
public:
	virtual int play(bool store);
	virtual int stop();
	virtual size_t getframe(ftype type, std::string& frm);
private:
	void work();
private:
	IplImage*  myimage;
#ifdef _WIN32
	HDC myScreenDC;
	HDC myCompDC;
#endif 
	std::vector<unsigned char> myimgbuf;
	int myfrmtop;
	int myfrmleft;
	int myfrmwidth;
	int myfrmheight;
private:
	static winapi playthread(void* lpParam);
	pthread_t threadid;
	bool m_bExit;
};
#endif
