#ifndef _MACHINE_H_
#define _MACHINE_H_
#include "websvr.h"
#include "business.h"
#include "commsvr.h"
#include <string>

struct macopt{
	char runkey[41];
	char script[256];
	char commsvrip[32];
	unsigned short commsvrport;
	char webpath[256];
	char websvrip[32];
	unsigned short websvrport;
	char option[256];
	bool console;
};

class cmachine : public ccommsvr, public cwebsvr, public cbusiness {
public:
	bool poweron(macopt& opt);
	bool poweroff();
public:
	virtual void oncomtask(sockaddr* addr, std::string data); 
	virtual void onluatrans(sockaddr* addr, std::string data); 
	virtual void onwebtask(int sock, std::string ifunc, std::string data);
};
#endif




