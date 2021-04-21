#include "snaps.h"
#include <string>
#include <map>
const size_t bufsize = 2048;

//监控线程启动
int csnaps::play(bool store){
#ifdef WIN32
	WSADATA wsdata;
	WSAStartup(0x0202, &wsdata);
#endif
	char ip[32] = { 0 };
	unsigned short port = 0;
	sscanf(m_source, "%[^:]:%hd", ip, &port);
	mysock = socket(AF_INET, SOCK_STREAM, 0);
	//printf("snaps addr %s:%d\n", ip, port);
	//struct timeval tv_out;
	//tv_out.tv_sec = 2;
	//tv_out.tv_usec = 100000;
	//setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv_out, sizeof(tv_out));
	memset(&svraddr, 0, sizeof(sockaddr_in));
	svraddr.sin_family = AF_INET;
	svraddr.sin_addr.s_addr = inet_addr(ip);
	svraddr.sin_port = htons(port);
	m_bConn = false;
	m_status  = 0;
	m_playevt = event_create( false, false );
	m_frmevt  = event_create( false, true );
#ifdef WIN32
	threadid  = _beginthreadex(NULL,0,playthread,this,0,NULL);
#else
	if( pthread_create(&threadid, NULL, playthread, this) != 0) 
		printf("pthread_create failed! \n");
#endif
	return 0;
}

//停止
int csnaps::stop(){
	event_set(m_playevt);
	event_set(m_frmevt);
	Sleep(100);
	event_timedwait(m_playevt,10000);
#ifdef WIN32
	closesocket(mysock);
	WSACleanup();
#else
	close(mysock);
#endif
	return 0;
}

//获取帧画面数据
size_t csnaps::getframe(ftype type, std::string & frm ){
	if( 0 != event_wait( m_frmevt ) )
		return 0;
	event_reset(m_frmevt);
	switch( type ){
	case JPG: frm += m_frmdata; break;
	case BMP: break;
	default: break;
	}
	event_set(m_frmevt);
	return frm.size();
}

void csnaps::work(){
	if (!m_bConn) {
		if (0 == connect(mysock, (struct sockaddr *)&svraddr, sizeof(struct sockaddr))) {
			printf("scaner connected.\n");
			m_bConn = true;
		}
		return;
	}
	unsigned int nsize = 0;
	if ( -1 == recv(mysock, (char*)&nsize, 4, 0) ) {
		printf("maybe scaner disconnect!\n");
		closesocket(mysock);
		mysock = socket(AF_INET, SOCK_STREAM, 0);
		m_bConn = false;
		return;
	}
	std::string newframe;
	while( nsize ){
		char buf[1024] = { 0 };
		int nrecv = recv(mysock, buf, 1024, 0);
		if (nrecv <= 0)
			break;
		newframe.append(buf, nrecv);
		nsize -= nrecv;
	}
	setframe(&newframe[0], newframe.size());
	return;
}

//接收处理线
winapi  csnaps::playthread(void* lpParam){
	csnaps * fpr = (csnaps*)lpParam;
	while(0!=event_timedwait(fpr->m_playevt,30)){
		fpr->work();
	}
	event_set( fpr->m_playevt );
	printf("pthread exist\n");
	return   0; 
}
