#ifndef _WEBSVR_H_
#define _WEBSVR_H
#include <string>
#include <map>
#ifdef _WIN32
#include <winsock2.h>
#endif
#define MAX_CLIENT 512

class tcps{
public:
	virtual bool dispath(int sock,char* buf, unsigned int size) = 0;
	int start(const char* ip, short port);
	void stop();
	int  sends(int sock, void* buf,int size);
private:
	#ifdef _WIN32
	static unsigned int __stdcall workthread(void* lpParam);
	#else
	static void* workthread(void* lpParam);
	#endif
	bool   m_bExit;
private:
	int    fdarray[MAX_CLIENT];
	#ifdef _WIN32
	unsigned int threadid;
	SOCKET mysocket;
	#else
	pthread_t threadid;
	int mysocket;
	#endif
};

//websvr
class cwebsvr : public tcps{
public:
	cwebsvr() :mytranfile(NULL), upfilesize(0), recvbuflen(0), markidLen(0){}
	int start(const char* ip, short port,const char* path);
	virtual void onwebtask(int sock, std::string ifunc, std::string data)=0;

private:
	void analysehead(std::stringstream& ssHeader, std::map<std::string, std::string>& v, int& httphl);
	bool uploadfiletrans(int sock, char* buf, unsigned int size);
	bool dispath(int sock, char* buf, unsigned int size);
	std::string description(std::string val);
	std::string getcontent(std::string ftype);
	std::string getgmtime();

private:
	char  webpath[256];
	FILE  * mytranfile;
	unsigned int upfilesize;
	unsigned int recvbuflen;
	unsigned int markidLen;

public:
	void setwebpath(const char* path);
	void replay(int sock, std::string data);
	void request(int sock, std::string data);
};
#endif


