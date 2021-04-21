#ifndef _PNPPR_H_
#define _PNPPR_H_
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
class mbase;
class cpnppr : public mbase{
public:
	static void search(const char* source,std::vector<std::string>& v);
	virtual int play(bool store);
	virtual int stop();
	virtual size_t getframe(ftype type, std::string& frm);
	virtual void move( int dir, int tms);
private:
	void query();
	void login();
	void work();
private:
	SOCKET mysock;
	sockaddr_in toaddr;
	std::string vcomfirm;
	std::map<unsigned int,std::string> vframe;
private:
	static winapi playthread(void* lpParam);
	pthread_t threadid;
	bool m_bExit;
};
#endif
