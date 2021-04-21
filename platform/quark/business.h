#ifndef _BUSINESS_H_
#define _BUSINESS_H_
#include <string>
#include "taskmgr.h"

//业务处理逻辑
class cbusiness {
public:
	cbusiness(){};
	virtual void onluatrans(sockaddr* addr,std::string data)=0;

public:
	bool start(const char* file);
	void stop();
	std::string execute(std::string ifunc, std::string data);

public:
	unsigned int timerdelay;

public:
	void dotimer(base::TimeDelta interval);
	void settimer(unsigned int n);
};

#endif


