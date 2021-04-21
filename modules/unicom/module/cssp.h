#ifndef _CSSP_H_
#define _CSSP_H_
#include "../kernel/xbase.h"
#include "../kernel/serial.h"
#include "event.h"

class cssp : public xbase{ 
private:
	virtual int writedata(const char* data, int len);
	virtual int readdata(char* data, int len);
	virtual bool open(const char* param );
	virtual bool close();
private:
	CMySerial    device;
};
#endif

