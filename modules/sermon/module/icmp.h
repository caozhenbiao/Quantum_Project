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


#ifdef WIN32
#include <Winsock2.h>
#include <process.h>
#include <direct.h>
#include <io.h>
#include "windows.h"
#define ACCESS _access   
#define MKDIR(a) _mkdir((a))   
#define localtime_x(x,y) localtime_s(x,y)
#define BASETIME 116444736000000000L
#define mssleep(x) Sleep(x)
#define winapi unsigned int __stdcall
#define pthread unsigned int
#include <winsock2.h>
#include <WS2tcpip.h>
#else
#include <stdarg.h>   
#include <sys/stat.h> 
#define ACCESS access   
#define MKDIR(a) mkdir((a),0755)   
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define sprintf_s snprintf
#define localtime_x(x,y) localtime_r(y,x)
#define BASETIME 116444736000000000ll
#define __int64 long long
#define mssleep(x) usleep(x * 1000)
#include <sys/vfs.h>
#include <mntent.h>
#include <pthread.h>
#define winapi void*
#define pthread pthread_t
typedef int SOCKET;
#define closesocket close
#include <netdb.h>   
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/unistd.h>
#include <sys/fcntl.h>
#include <time.h>
#endif

////////////////////////////////////////////////////////////////
#define WIN32_LEAN_AND_MEAN
#define IP_RECORD_ROUTE  0x7

#define ICMP_ECHO        8
#define ICMP_ECHOREPLY   0
#define ICMP_MIN         8 // Minimum 8-byte ICMP packet (header)

#define DEF_PACKET_SIZE  32        // Default packet size
#define MAX_PACKET       1024      // Max ICMP packet size
#define MAX_IP_HDR_SIZE  60        // Max IP header size w/options

//�ֽڶ��������1 
#pragma pack (1) 

// IP header structure
typedef struct _iphdr {
	unsigned int   h_len : 4;        // Length of the header
	unsigned int   version : 4;      // Version of IP
	unsigned char  tos;            // Type of service
	unsigned short total_len;      // Total length of the packet
	unsigned short id;             // Unique identification
	unsigned short frag_offset;    // Fragment offset
	unsigned char  ttl;            // Time to live
	unsigned char  protocol;       // Protocol (TCP, UDP etc)
	unsigned short checksum;       // IP checksum
	unsigned int   sourceIP;       // Source IP
	unsigned int   destIP;         // Destination IP
} IpHeader;


// ICMP header structure
typedef struct _icmphdr {
	unsigned char   i_type;
	unsigned char   i_code;                 // Type sub code
	unsigned short i_cksum;
	unsigned short  i_id;
	unsigned short  i_seq;
	unsigned long   timestamp;
} IcmpHeader;

// IP option header - use with socket option IP_OPTIONS
typedef struct _ipoptionhdr {
	unsigned char code;        // Option type
	unsigned char len;         // Length of option hdr
	unsigned char ptr;         // Offset into options
	unsigned long addr[9];     // List of IP addrs
} IpOptionHeader;

//ethernet ��̫����ͷ
struct ethernet_head
{
	unsigned char dest_mac[6];    //Ŀ������MAC��ַ 
	unsigned char source_mac[6];  //Դ��MAC��ַ 
	unsigned short eh_type;       //��̫������ 
};

//arp��ͷ
struct arp_head {
	unsigned short hardware_type; //Ӳ�����ͣ���̫���ӿ�����Ϊ1 
	unsigned short protocol_type; //Э�����ͣ�IPЭ������Ϊ0X0800 
	unsigned char add_len; //Ӳ����ַ���ȣ�MAC��ַ����Ϊ6B 
	unsigned char pro_len; //Э���ַ���ȣ�IP��ַ����Ϊ4B 
	unsigned short option; //������ARP����Ϊ1��ARPӦ��Ϊ2 
	unsigned char sour_addr[6]; //ԴMAC��ַ�����ͷ���MAC��ַ 
	unsigned int sour_ip; //ԴIP��ַ�����ͷ���IP��ַ 
	unsigned char dest_addr[6]; //Ŀ��MAC��ַ��ARP�����и��ֶ�û�����壻ARP��Ӧ��Ϊ���շ���MAC��ַ 
	unsigned int dest_ip; //Ŀ��IP��ַ��ARP������Ϊ���������IP��ַ��ARP��Ӧ��Ϊ���շ���IP��ַ
	unsigned char padding[18];
};
//arp���ṹ 
struct arp_packet {
	ethernet_head eth; //��̫��ͷ�� 
	arp_head arp; //arp���ݰ�ͷ�� 
};
#pragma pack () 

unsigned short checksum(unsigned short  *buffer, int size);

class xbase;

typedef struct _devstatus {
	unsigned char protype;
	unsigned int  ipaddr;
	unsigned char macaddr[6];
	long long     lasttime;
}devstatus;

#ifndef WIN32
unsigned int GetTickCount();
#define GetCurrentProcessId getpid
#endif











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
	PINGRET doPing(const char* ip ,bool bBroadcast = false); //�㲥��ַ�ر���
	void SetSrcAddr(const in_addr ip );
	void SetTTL(const unsigned int ttl);
	void SetTimeout(const unsigned int t );
	void SetBufSize(const unsigned short n );
	void SetSeq(const unsigned short n);
	void start(const char * ip, int tms); //�㲥��ַ�ر���
public:
	unsigned int m_ttl;       //����ʱ��
	unsigned int m_timeout;   //���ճ�ʱʱ��
	unsigned int m_bufsize;   //���ͻ�����
	char         m_destip[32];
	char         m_srctip[32];
	unsigned int m_times;
	unsigned int m_seq;
	unsigned int m_usedtime;
private:
	void unpackicmp(unsigned char *buf, int bytes, struct sockaddr_in *from);
};
#endif
