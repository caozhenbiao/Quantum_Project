//COMMSVR
#ifndef _COMMSVR_H_
#define _COMMSVR_H_
#include <string>
#include "taskmgr.h"

class ccommsvr{
public:
	ccommsvr() {};
	virtual void oncomtask(sockaddr* adds, std::string data)=0;
public:
	int start( const char * ip ,unsigned short port = 10000);
	void stop();
	void write(sockaddr* addr, std::string data);
	void recvproc(base::TimeDelta interval);
private:
	int mysocket;
	struct sockaddr myuiaddr;
};
#endif


