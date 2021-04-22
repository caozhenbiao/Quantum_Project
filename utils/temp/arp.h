#ifndef _NETCAPTURE_H_
#define _NETCAPTURE_H_
#include "define.h""
#include <pcap.h>
class netcapture{
public:
	int  doArp(const char* ip, unsigned char* mac);
	char         m_netcard[256];
	unsigned int m_timeout;
	unsigned int m_usedtime;
	unsigned char m_macAddr[6];
private:
	int sendpkg(pcap_t* hdr, unsigned int nhost);
//#ifdef WIN32
//	SOCKET m_socket;
//#endif
};
#endif