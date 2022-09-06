#include "commsvr.h"
#include <errno.h>

//启动UDP通信服务
int ccommsvr::start( const char * ip, unsigned short port){
	printf("start commnicate service,ip:%s port:%d\n",ip,port);
#ifdef _WIN32
	WSADATA wsdata;
	WSAStartup(0x0202,&wsdata);
#endif
	mysocket = socket (AF_INET, SOCK_DGRAM, 0);

	int timeout = 5;
	setsockopt( mysocket, SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(timeout));

	/* Set port reuse */
	int opt = 1;
	if (setsockopt(mysocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt)) < 0)
		return -1;

	/*bind address*/
	sockaddr_in  addr;
	addr.sin_family = AF_INET;   
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);   
	if(bind(mysocket,(sockaddr*)&addr,sizeof(addr))==-1){  
		return -1;   
	}
	return 0;
}

//stop
void ccommsvr::stop(void){
#ifdef _WIN32
	closesocket(mysocket);
	WSACleanup();
#else
	close(mysocket);
#endif
}

//发送到GUI
void ccommsvr::write(sockaddr* addr, std::string data){
	int sent = 0;
	int len = data.length();
	static unsigned char buf[10248] = {0};
	static unsigned char seq = 0;
	unsigned char nPicec = (unsigned char)((len + 10239) / 10240);
	memset(buf, 0x00, sizeof(buf));
	for( unsigned char i = 0; i< nPicec; i++ ){
		size_t datalen = (i<nPicec-1)?(10240):(len-(i*10240));
		buf[0] = 0xF1;
		buf[1] = (unsigned char)(datalen >> 8);
		buf[2] = (datalen & 0x00FF);
		buf[3] = seq;
		buf[4] = nPicec;
		buf[5] = i;
		memcpy( &buf[6], &data[i*10240],datalen );
		buf[datalen+6] = 0xF2;
		sent += sendto(mysocket,(char*)buf, datalen+7,0,(struct sockaddr*)addr, sizeof(sockaddr));
		//if( nPicec> 1 ) Sleep(10);		
	}
	seq++;
	return;
}

void ccommsvr::recvproc(base::TimeDelta interval) {
	static char buf[10240] = { 0 };
	#ifdef _WIN32
	int length = sizeof(sockaddr);
	#else
	socklen_t length = sizeof(sockaddr);
	#endif
	memset(buf, 0x00, sizeof(buf));
	struct sockaddr addr;
	int len = recvfrom(mysocket, buf, 2048, 0, &addr, &length);
	if (len >= 7) {
		std::string data(buf);
		taskmgr::posttask(taskmgr::LUA, base::Bind(&ccommsvr::oncomtask, base::Unretained(this), &addr,data));
	}
	taskmgr::postdelayedtask(taskmgr::NET,
		base::Bind(&ccommsvr::recvproc, base::Unretained(this), interval),
		interval);
}
 
