#ifndef _XBASE_H_
#define _XBASE_H_
#include "../define.h"
#include <vector>

//模块抽象基类
class xbase {
public:
	virtual int writedata(const char* data, int len)=0;
	virtual int readdata(char* data, int len)=0;
	virtual bool open(const char* param)=0;
	virtual bool close()=0;
public:
	char          myname[100];     //逻辑模块名称
	char          myparams[256];   //模块物理参数
	char          myprotocal[100]; //通信规约
	unsigned int  mystatus;        //子模块状态，最多4个子模块
	unsigned int  mydelay;         //延迟时间
	char          myclass[100];
	int          inports;
	int          exports;
};
#endif
