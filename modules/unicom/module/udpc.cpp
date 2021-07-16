
#include "udpc.h"
#ifdef WIN32
#pragma comment( lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/unistd.h>
#endif

bool udpc::open(const char* param )
{
#ifdef WIN32
	WSADATA wsdata;
	WSAStartup(0x0202,&wsdata );
#endif
	//解析参数
	char binIP[256], destIP[256];
	int binPort = 0;
	int destPort = 0;
	int argv = sscanf(param, "%[^:]:%d;%[^:]:%d", destIP, &destPort, binIP, &binPort);
	if (argv < 2)
		return false;
	fromaddr.sin_family = AF_INET;
	fromaddr.sin_addr.s_addr = inet_addr(destIP);
	fromaddr.sin_port = htons(destPort);
	mysocket = socket(AF_INET, SOCK_DGRAM, 0);;
	//梆定
	if (argv == 4) {
		sockaddr_in srcaddr;
		srcaddr.sin_family = AF_INET;
		srcaddr.sin_addr.s_addr = inet_addr(binIP);
		srcaddr.sin_port = htons(binPort);
		int ibind = bind(mysocket, (sockaddr*)&srcaddr, sizeof(sockaddr_in));
		if (ibind != 0) {
			printf("bind addr(%s:%d) fails.\n", destIP, destPort);
		}
	}
#ifdef WIN32
	int timeout = 1;
	setsockopt( mysocket,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(timeout));
#else
	struct timeval tv_out;
    tv_out.tv_sec = 0;
    tv_out.tv_usec = 100000;//等待100毫秒
	setsockopt(mysocket,SOL_SOCKET,SO_RCVTIMEO,&tv_out, sizeof(tv_out));
#endif
	return true;
}

bool udpc::close(){
#ifdef WIN32
	closesocket(mysocket);
	WSACleanup();
#else
	::close(mysocket);
#endif
	return true;
}

//读数据
int udpc::readdata( char* data, int len )
{
	int cnt = 0;
	sockaddr_in addr;
	memset( &addr, 0, sizeof(sockaddr_in));
#ifdef WIN32
	int length = sizeof(SOCKADDR);
	cnt = recvfrom(mysocket,data,len,0,(struct sockaddr FAR*)&addr,(int FAR*)&length);
#else
	int length = sizeof(sockaddr);
	cnt = recvfrom(mysocket,data,len,0,(struct sockaddr*)&addr,(socklen_t*)&length);
#endif
	if( cnt > 0 ){
		inports += cnt;
		//lastcommtime = time(NULL);
		//memcpy( &fromaddr, &addr, sizeof(sockaddr_in));
		//process(data, cnt);
	}
	return cnt;
}

//写数据
int udpc::writedata( const char* data , int len )
{
	int cnt = sendto( mysocket, data, len, 0, (sockaddr*)&fromaddr,sizeof(sockaddr_in) );
	if( cnt > 0){
		exports += cnt;
	}
	return cnt;
}
