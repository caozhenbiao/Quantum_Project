#ifndef _CTCP_H_
#define _CTCP_H_
#include "../kernel/xbase.h"
#ifdef WIN32
#include <winsock.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include "../define.h"
class tcpc : public xbase{
public:
	virtual bool open(const char* param);
	virtual bool close();
	virtual int  readdata( char* data, int len );
	virtual int  writedata( const char* data , int len );
private:
	SOCKET mysocket;

};
#endif
