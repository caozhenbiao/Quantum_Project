#include "tcpc.h"
#ifdef WIN32
#pragma comment( lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/unistd.h>
#include <sys/ioctl.h>
#include <sys/fcntl.h>
#endif

// params bindIP:port;destIP:port
bool tcpc::open( const char* param ){
#ifdef WIN32
	WSADATA wsdata;
	WSAStartup(0x0202,&wsdata );
#endif
	//����
	printf("TCP %s\n", param);
	char srcIP[256], destIP[256];
	int srcPort = 0;
	int destPort = 0;
	int argv = sscanf(param, "%[^:]:%d;%[^:]:%d", srcIP, &srcPort, destIP, &destPort);
	if (argv < 2)
		return false;
	//server addr
	mysocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	//��
	if (argv == 4) {
		sockaddr_in srcaddr;
		srcaddr.sin_family = AF_INET;
		srcaddr.sin_addr.s_addr = inet_addr(srcIP);
		srcaddr.sin_port = htons(srcPort);
		int ibind = bind( mysocket, (sockaddr*)&srcaddr, sizeof(sockaddr_in));
		if( ibind != 0 ){
			printf("bind addr(%s:%d) fails.\n", srcIP, srcPort);
		}
	}
	printf("socker ok");

	sockaddr_in svraddr;
	svraddr.sin_family = AF_INET;
	svraddr.sin_addr.s_addr = inet_addr(destIP);
	svraddr.sin_port = htons(destPort);
	int iConnect = connect( mysocket, (sockaddr*)&svraddr, sizeof(sockaddr_in));
	if( iConnect != 0 ){
		close();
		return false;
	}

	printf("connect success\n");

	//����������
#ifdef WIN32
	unsigned long nMode = 1;
	ioctlsocket(mysocket,FIONBIO,&nMode);
#else
	fcntl(mysocket,F_SETFL,O_NONBLOCK);
#endif
	return true;
}

bool tcpc::close(){
#ifdef WIN32
	closesocket( mysocket );
	WSACleanup();
#else
	close(mysocket);
#endif
	return true;
}

//��ǰ�ͻ���ʵʱ����
int tcpc::readdata( char* data, int len ){
	int cnt = recv( mysocket, data, len,0);
	printf(data);
	if( cnt > 0 ){
	}
	return cnt;
}

//����ǰ�ͻ��˷������� 
int tcpc::writedata(const char* data , int len )
{
	int cnt = send( mysocket, data, len, 0 );
	if( cnt > 0){
		exports += cnt;
	}
	return cnt;
}

