#include "icmp.h"
#ifdef WIN32
#include <process.h>
#endif
//extern void Output(const char * ftm, ... );



//计算校验值
unsigned short checksum(unsigned short  *buffer, int size)
{
	unsigned long cksum = 0;
	while (size > 1) {
		cksum += *buffer++;
		size -= sizeof(unsigned short);
	}
	if (size) {
		cksum += *(unsigned char *)buffer;
	}
	cksum = (cksum >> 16) + (cksum & 0xffff);
	cksum += (cksum >> 16);
	return (unsigned short)(~cksum);
}

#ifndef WIN32
unsigned int GetTickCount() {
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}
#endif


CICMP::CICMP(void){
	m_bufsize = 32;
	m_timeout = 1000;
	m_seq = 0;
#ifdef WIN32
	WSADATA wsdata;
	WSAStartup(0x0202,&wsdata);
#endif
}

CICMP::~CICMP(void){
#ifdef WIN32
	WSACleanup();
#endif
}

//tms 返回的是时间
PINGRET CICMP::doPing(const char * ip, bool bBroadcast){
	m_usedtime = 0;
	SOCKET sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&m_timeout, sizeof(m_timeout));
	setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char*)&m_timeout, sizeof(m_timeout));
	struct sockaddr_in dest;
	memset(&dest, 0, sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_port = htons(0);
	dest.sin_addr.s_addr = inet_addr( ip );
	const int datasize  = sizeof(IcmpHeader) + m_bufsize;
	//char  icmp_data[datasize];
	char * icmp_data = new char[ datasize ];
	memset(icmp_data,0,datasize);
#ifdef WIN32
	unsigned long   starttick = GetTickCount();
#else
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC,&ts);
	unsigned long   starttick =(ts.tv_sec * 1000 + ts.tv_nsec/1000000);
#endif
	//ICMP头打包
	IcmpHeader *icmp_hdr = NULL;
	icmp_hdr = (IcmpHeader*)icmp_data;
	icmp_hdr->i_type = ICMP_ECHO; 
	icmp_hdr->i_code = 0;
	icmp_hdr->i_id  =  GetCurrentProcessId();
	icmp_hdr->i_cksum = 0;
	icmp_hdr->i_seq = m_seq;
	m_seq++;
	icmp_hdr->timestamp = starttick;
	memset(icmp_data + sizeof(IcmpHeader),'E',datasize-sizeof(IcmpHeader));
	icmp_hdr->i_cksum = checksum((unsigned short *)icmp_data,datasize);
	int nsend = sendto(sock,(const char*)icmp_data, datasize, 0, (struct sockaddr*)&dest, sizeof(sockaddr));
	delete[] icmp_data;
	if( nsend <=0 ){
		closesocket(sock);
		//Output("can not send");
		return UNABLE_SEND;
	}
	//接收
	struct sockaddr_in from;
	char recvbuf[1024];
	int fromlen = sizeof(sockaddr_in);
	int bytes = 1024;
	int nrecv = recvfrom(sock,(char*)recvbuf, bytes,0, (struct sockaddr*)&from,(socklen_t*)&fromlen);
	if( nrecv > 0 ){
		IpHeader       *iphdr = NULL;
		IcmpHeader     *icmphdr = NULL;
		unsigned short  iphdrlen;
		iphdr = (IpHeader *)recvbuf;
		iphdrlen = iphdr->h_len * 4;;
		int n = iphdr->ttl;
		if(bytes < iphdrlen + ICMP_MIN){
			//Output("Too few bytes from %s \n", inet_ntoa(from.sin_addr));
			closesocket(sock);
			return UNREACH;
		}
		icmphdr = (IcmpHeader*)(recvbuf + iphdrlen);
		if(icmphdr->i_type != ICMP_ECHOREPLY){
			//Output("non-echo type %d recvd", icmphdr->i_type);
			closesocket(sock);
			return UNREACH;
		}else{
			m_usedtime = GetTickCount() - starttick;
			//Output("%d bytes from %s icmp_seq = %d. time: %d ms", bytes, 
			//	inet_ntoa(from.sin_addr),
			//	icmphdr->i_seq,
			//	m_usedtime);
			closesocket(sock);
			return REACH;
		}
	}
	closesocket(sock);
	return UNABLE_RECV;
}
//
////ICMP头解包
void CICMP::unpackicmp(unsigned char *buf, int bytes, struct sockaddr_in *from){
	IpHeader       *iphdr = NULL;
	IcmpHeader     *icmphdr = NULL;
	unsigned short  iphdrlen;
	unsigned long   tick;
	iphdr = (IpHeader *)buf;
	iphdrlen = iphdr->h_len * 4;
	char szbuf[1024] = {0};
	tick = GetTickCount();
	if(bytes < iphdrlen + ICMP_MIN){
		sprintf(szbuf,"Too few bytes from %s \n", inet_ntoa(from->sin_addr));
		return;
	}
	icmphdr = (IcmpHeader*)(buf + iphdrlen);
	if(icmphdr->i_type != ICMP_ECHOREPLY) // echo type must be 0
	{
		sprintf(szbuf,"non-echo type %d recvd \n", icmphdr->i_type);
		return;
	}
	if(icmphdr->i_id != (unsigned short)GetCurrentProcessId()){
		sprintf(szbuf,"someone else's packet! \n");
		return ;
	}
	sprintf(szbuf, "insert:data= %d bytes from %s icmp_seq = %d. time: %d ms", bytes, 
		inet_ntoa(from->sin_addr),
		icmphdr->i_seq,
		int(tick - icmphdr->timestamp));
	//Output(szbuf);
	return;
}

void CICMP::SetTTL(const unsigned int ttl){

}
void CICMP::SetTimeout(const unsigned int t ){
	m_timeout = t;
}
void CICMP::SetBufSize(const unsigned short n ){
	m_bufsize = n;
}
void CICMP::SetSeq(const unsigned short n){
	m_seq = n;
}
