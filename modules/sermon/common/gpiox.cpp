#include "gpiox.h"
#include "../kernel/Log.h"

gpiox::gpiox(int pn, int mode) : _fdValue(-1)
{
	_nMode = mode;	// 0 for DI, 1 for DO, other for reserve
	_pn = pn;
	Init();
} 

gpiox::~gpiox()
{
	Close();
}

int gpiox::GetStatus()
{
	char buff[256] = {0};
	int nRet = -1;
	int fd = Open();
	if(fd > 0)
	{
		if(1 == read(fd, buff, 1))
			nRet = (buff[0]=='0')?0:1;
		else
			perror("read");
	}
	return nRet;
}
int gpiox::SetStatus(int on)
{
	int nRet = -1;
	if(SetDirection(1) >= 0)
	{
		nRet = WriteValue(on);
	}
	return nRet;
}

	
int gpiox::Init()
{
	int fd;
	int nRet = -1;
	char buff[256] = {0};
	sprintf(buff, "%d", _pn);
	fd = open("/sys/class/gpio/export", O_WRONLY);

	if(fd > 0)
	{
		if(write(fd, buff, strlen(buff)) > 0)
			nRet = 0;
		close(fd);
		fd = -1;
		if(_nMode == 1) //DO
		{

		}
	}
	else
	{
		perror("open export:");
	}		
	return nRet;
}
int gpiox::Open()
{
	//if(_fdValue <= 0)
	{
		char buff[256] = {0};
		sprintf(buff, "/sys/class/gpio/gpio%d/value", _pn);
		_fdValue = open(buff, O_RDWR);
		if(_fdValue <=0 )
			perror("buff");
	}
	return _fdValue;
}


int gpiox::SetDirection(int out)
{
	char buff[256];
	int fd = -1;
	int nRet = -1;
	sprintf(buff, "/sys/class/gpio/gpio%d/direction" , _pn);
	fd = open(buff, O_RDWR);
	if(fd > 0)
	{
		if(write(fd, out?"out":"in",out?3:2)  > 0) 
			nRet = 0;
	}
	return nRet;
}
int gpiox::WriteValue(int out)
{
	char buff[256];
	int fd = -1;
	int nRet = -1;
	sprintf(buff, "/sys/class/gpio/gpio%d/value" , _pn);
	fd = open(buff, O_RDWR);
	if(fd > 0)
	{
		if(write(fd, out?"1":"0",1)  > 0) 
			nRet = 0;
	}
	return nRet;
}
int gpiox::Close()
{
	if(_fdValue > 0)
		close(_fdValue);
	_fdValue = -1;
	return _fdValue;
}

