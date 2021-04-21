#ifndef _XBASE_H_
#define _XBASE_H_
#include "taskdef.h"
#include "datamgr.h"
#include "../define.h"
#include "../event.h"
#include <vector>

//模块抽象基类
class xbase : public datamgr {
public:
	xbase();
	virtual ~xbase(){};
protected:
	virtual void dotask(task * tsk);
	virtual void dowork() = 0;
	virtual int setparam(char* args, int len) { return 0; };
	virtual int readdata(char* data, int len) { return 0; };
	virtual int writedata(char* data, int len){ return 0; };

public:
	void addtask(task * tsk ,bool wait=false);   //添加任务
	void inject(const char * name, xbase * obsvr, const char* path);//注入模块
	void setdelay(unsigned int tms);             //延时设置
	bool start();                                //启动
	void stop();                                 //停止  

public:
	std::vector<task> mytasks;     //任务列表
	xbase *           observer;

public:
	char          myname[100];     //逻辑模块名称
	char          myparams[256];   //模块物理参数
	char          myprotocal[100]; //通信规约
	unsigned int  mystatus;        //子模块状态，最多4个子模块
	unsigned int  mydelay;         //延迟时间
	char          myclass[100];

private:
    static winapi workthread(void * argv);
	pthread		 threadid; 
	bool         exitthread;
	event_handle myevent;
	event_handle myreccs;
};
#endif
