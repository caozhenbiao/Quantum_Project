#pragma once
#include <string>
#include <vector>
#ifdef _WIN32
#include <winsock.h>
#include <Iphlpapi.h>
#include <Mprapi.h>
#include <Netlistmgr.h>
#else
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <arpa/inet.h>
#include <sys/unistd.h>
#include <sys/ioctl.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <error.h>
#include <net/route.h>
#endif
#include <iostream>


typedef struct _netcardinfo {
	char name[256];
	char desc[256];
	unsigned char mac[6];
	char ip[50];
	char mask[50];
	char gate[50];
}netcardinfo;



class cnetcard
{
public:
	int getnetcards(std::vector<netcardinfo>& v );
	int setip(const char * card, const char* ip, const char* mask, const char* gate);
	int IsNetwork();
};



