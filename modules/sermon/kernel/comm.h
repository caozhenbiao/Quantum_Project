#ifndef _COMM_H_
#define _COMM_H_
#include "../common/serial.h"
class comm {
public:
	comm() { isopen = false; }
	virtual ~comm() {}
public:
	int open(char* param);
	int close();
public:
	int read(char* data, int len);
	int write(char* data, int len);
private:
	CMySerial    device;
	bool         isopen;
};
#endif

