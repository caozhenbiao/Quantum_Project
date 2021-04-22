#ifndef _ICMP_H_
#define _ICMP_H_

#ifndef WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <arpa/inet.h>
#include <sys/unistd.h>
#include <sys/ioctl.h>
#include <sys/fcntl.h>
typedef int SOCKET;
#endif

#include <string>
#include <vector>

typedef enum _PINGRESULT{
	REACH,
	ERROR_DES_ADDR,
	ERROR_SRC_ADDR,
	UNABLE_SEND,
	UNABLE_RECV,
	UNREACH
} PINGRET;

#define ICMP_ECHO        8
#define ICMP_ECHOREPLY   0
#define ICMP_MIN         8 // Minimum 8-byte ICMP packet (header)
#define DEF_PACKET_SIZE  32        // Default packet size
#define MAX_PACKET       1024      // Max ICMP packet size
#define MAX_IP_HDR_SIZE  60        // Max IP header size w/options
#define USHORT unsigned short
#define BYTE   unsigned char
#define ULONG  unsigned int
#define DWORD  unsigned int

class CICMP{
public:
	CICMP(void);
	~CICMP(void);
public:
	PINGRET doPing(const char* ip ,bool bBroadcast = false); //广播地址特别处理
	void SetSrcAddr(const in_addr ip );
	void SetTTL(const unsigned int ttl);
	void SetTimeout(const unsigned int t );
	void SetBufSize(const unsigned short n );
	void SetSeq(const unsigned short n);
	void start(const char * ip, int tms); //广播地址特别处理
public:
	unsigned int m_ttl;       //生存时间
	unsigned int m_timeout;   //接收超时时间
	unsigned int m_bufsize;   //发送缓冲区
	char         m_destip[32];
	char         m_srctip[32];
	unsigned int m_times;
	unsigned int m_seq;
	unsigned int m_usedtime;
private:
	void unpackicmp(unsigned char *buf, int bytes, struct sockaddr_in *from);
};
#endif
