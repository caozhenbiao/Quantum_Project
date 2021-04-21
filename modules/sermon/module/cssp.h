#ifndef _CSSP_H_
#define _CSSP_H_
#include "../kernel/xbase.h"
#include "../event.h"
#include "../db/RawDataDB.h"
#include "../common/serial.h"
#include <vector>

class cssp : public xbase{
public:
	cssp() {};
	~cssp() {};
private:
	virtual void dotask(task * tsk);
	virtual void dowork();
private:
	int flyback(task * tsk);
private:
	virtual int setparam(char* args, int len);
	virtual int readdata(char* data, int len);
	virtual int writedata(char* data, int len);
	bool open( char* param );
	bool close();
private:
	CMySerial    device;
};
#endif

