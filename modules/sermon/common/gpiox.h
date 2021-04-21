#ifndef _GPIOX_H_
#define _GPIOX_H_
#include "../kernel/xbase.h"


#include <stdio.h>

//#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
//#include <poll.h>
//#include <sys/time.h>
//#include <sys/times.h>
//#include <bits/time.h>
#include <time.h>


class gpiox
{
public:
	gpiox(int pn, int mode=0);
	virtual ~gpiox();
	int GetStatus();
	int SetStatus(int on);
protected:
	virtual int Init();
	int Open();
	int _pn;
private:
	int _nMode;
	int _nStatus;
	int _fdValue;
	int SetDirection(int out);
	int WriteValue(int out);
	int Close();
};
#endif

