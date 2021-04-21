/*
������˫ͨ�����ڼ�����
������
*/
#ifndef _CDSP_H_
#define _CDSP_H_
#include "../kernel/xbase.h"
#include "../kernel/serial.h"
#include "event.h"

class cdsp : public xbase {
public:
	cdsp() {};
	~cdsp() {};
private:
	virtual void dotask(task * tsk);
	virtual void dowork();
private:
	virtual int setparam(char* args, int len);
	virtual int readdata(char* data, int len);
	virtual int writedata(char* data, int len);
	bool open(char* param);
	bool close();
private:
	int flyback(task * tsk);
private:
	CMySerial   device[2];
};
#endif

