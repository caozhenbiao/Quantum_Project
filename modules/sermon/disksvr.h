#ifndef _DISKSVR_H_
#define _DISKSVR_H_
#include "define.h"
#include "event.h"
#include <map>
#include <string>
//diskver
class disksvr{
public:
	void start();
	void stop();

public:
	void addmonitor(const char* path);
	void delmonitor(const char* path);

private:
	std::map<std::string, std::string> workdir;
	static winapi disksvrthread(void * argv);
	event_handle myevent;
	pthread		 threadid; 
	bool         exitthread;
};
#endif
