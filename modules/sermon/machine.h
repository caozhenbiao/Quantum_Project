#ifndef _MANAGER_H_
#define _MANAGER_H_
#include "kernel/xbase.h"
#include "disksvr.h"
#include <map>
#include <string>
using namespace std; 
 
//MACHINE
class machine : public xbase
{
public:
	void start();
	void stop();
public:
	xbase* createmodule(const char* name, const char* path, int ht);
	bool   removemodule(const char* name);
	xbase* getmodulebyname(const char* name);
	void   addtask(const char* name, task * tsk, bool wait);
	std::string getmodulename(xbase* dev);
	void   transhistorydata(const char * name, const char* timestamp);
	int    ping(const char* ip, int bfs, int timeout, int& usedtime);

public:
	virtual void dotask(task * tsk);
	virtual void dowork();
public:
	map<string, xbase*>  mymodules;    //模块列表
	disksvr  diskscan;
	xbase * printer;                   //数据打印输出接口
};
#endif
