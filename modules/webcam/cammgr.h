#ifndef _CAMMGR_H_
#define _CAMMGR_H_
#include <string>
#include <vector>
#include <map>
#include "event.h"

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

enum ftype{
	UNKNOWN = -1,
	BMP = 0,
	JPG = 1,
	YUY2 = 2
};

//视频模块基类
class mbase{
public:
	mbase();
	virtual ~mbase();
	virtual int play(bool store) = 0;
	virtual int stop() = 0;
public:
	virtual size_t getframe(ftype type,std::string & frm); 
	virtual void   move(int dir, int tms);
public:
	void setframe(const char* frm, int size);
	void setsave(bool save);
protected:
	std::string  m_frmdata; 
	FILE * m_pstorefile;
	event_handle m_playevt;
	event_handle m_frmevt;
public:
	char store_path[256];
	char identify[256];
	bool m_bsave;
	bool bdisplay;
	unsigned short width;
	unsigned short hight;
	char m_source[256];
	unsigned char m_status;
};

//摄像头管理
class ccammgr : public std::map<std::string,mbase*> {
public:
	int add(const char* mark, const char* url, int type);
	int del(const char* mark);
	int play(const char* mark, bool store);
	int stop(const char* mark);
	size_t getfrm(const char* mark,ftype type, std::string& frm);
	int move(const char* mark,int x, int y);
	int storage(const char* mark,bool save);
private:
	std::map<std::string,mbase*> m_devs;
};
#endif
