#ifndef _CSNP_H_
#define _CSNP_H_
#include "../kernel/xbase.h"
#include "../event.h"
#include "../define.h"
#include "../db/RawDataDB.h"
#include "../common/serial.h"
#include "pcap.h"
#include <vector>

class csnp : public xbase{
public:
	csnp() {};
	~csnp() {};
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
	pcap_t*   adhandle;
};
#endif

