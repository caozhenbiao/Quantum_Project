#include "pnppr.h"
#include <string>
#include <map>
const size_t bufsize = 2048;

void cpnppr::search(const char* source,std::vector<std::string>& v){
#ifdef WIN32
	WSADATA wsdata;
	WSAStartup(0x0202,&wsdata);
#endif
	char ip[32]={0};
	unsigned int port = 0;
	sockaddr_in  bcaddr;
	unsigned char buf[bufsize]={0};
	int addlen = sizeof(sockaddr);
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	int broadcast = 1;
	setsockopt( sock, SOL_SOCKET,SO_BROADCAST,(char*)&broadcast,sizeof(broadcast));
	struct timeval tv_out;
	tv_out.tv_sec = 2;
	tv_out.tv_usec= 100000;
	setsockopt( sock, SOL_SOCKET,SO_RCVTIMEO,(char*)&tv_out,sizeof(tv_out));
	sscanf( source, "%[^:]:%d", ip, &port);
	memset(&bcaddr,0,sizeof(sockaddr_in));
	bcaddr.sin_family = AF_INET;
	bcaddr.sin_addr.s_addr = inet_addr( ip );
	bcaddr.sin_port = htons( port );

	//广播
	int lens = -1;
	while( lens == -1 ){
		unsigned char buf1[]={0xF1,0x30,0x00,0x00};
		int ns = sendto(sock,(char*)buf1,4,0,(sockaddr*)&bcaddr, sizeof(sockaddr_in));
		printf(">>sendto:%d\n",ns);
		Sleep(100);
		sockaddr_in  addr;
		memset(&buf,0x00, sizeof(buf));
		lens = recvfrom(sock,(char*)buf,bufsize,0,(sockaddr*)&addr,(socklen_t*)&addlen);
		if( lens > -1){
			char source[64]={0};
			char t[11]={0};
			unsigned short no = 0;
			char c[5] ={0};
			memcpy(t,buf+4,10);
			no = buf[14]*256 + buf[15];
			memcpy(c,buf+16,4 );
			sprintf( source, "%s:%d,%s%06d%s", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port),t,no,c);
			v.push_back( source );
		}
		Sleep(500);
	}
	closesocket(sock);
}

//监控线程启动
int cpnppr::play(bool store){
	m_status  = 0;
	m_playevt = event_create( false, false );
	m_frmevt  = event_create( false, true );
#ifdef WIN32
	threadid  = (unsigned int)_beginthreadex(NULL,0,playthread,this,0,NULL);
#else
	if( pthread_create(&threadid, NULL, playthread, this) != 0) 
		printf("pthread_create failed! \n");
#endif
	return 0;
}

//停止
int cpnppr::stop(){
	event_set(m_playevt);
	Sleep(100);
	event_timedwait(m_playevt,100000);
#ifdef WIN32
	closesocket(mysock);
	WSACleanup();
#else
	close(mysock);
#endif
	return 0;
}

//获取帧画面数据
size_t cpnppr::getframe(ftype type, std::string & frm ){
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

//IP地址查询
void cpnppr::query(){
#ifdef WIN32
	WSADATA wsdata;
	WSAStartup(0x0202,&wsdata);
#endif
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	int broadcast = 1;
	setsockopt( sock, SOL_SOCKET,SO_BROADCAST,(char*)&broadcast,sizeof(broadcast));

	struct timeval tv_out;
	tv_out.tv_sec = 4;
	tv_out.tv_usec= 0;
	setsockopt( sock, SOL_SOCKET,SO_RCVTIMEO,(char*)&tv_out,sizeof(tv_out));

	//广播地址
	char ip[32] = {0};
	char id[32] = {0};
	unsigned int port = 0;
	sscanf( m_source, "%[^:]:%d;%s", ip, &port, id);
	sockaddr_in bcaddr;
	bcaddr.sin_family = AF_INET;
	bcaddr.sin_addr.s_addr = inet_addr( ip );
	bcaddr.sin_port = htons( port );
	//广播
	int trytime = 10;
	int addlen = sizeof(sockaddr);
	unsigned char buf[bufsize]={0};
	while( trytime-- ){
		unsigned char buf1[]={0xF1,0x30,0x00,0x00};
		sendto(sock, (char*)buf1,4,0,(sockaddr*)&bcaddr, addlen);
		Sleep(100);
		if( recvfrom(sock,(char*)buf,bufsize,0,(sockaddr*)&toaddr,(socklen_t*)&addlen) >= 0x18){
			char source[64]={0};
			char t[11]={0};
			char c[6] ={0};
			memcpy(t,buf+4,10);
			memcpy(c,buf+16,5 );
			sprintf( source, "%s%06d%s", t,buf[14]*256+buf[15],c);
			if( strcmp( source, id ) == 0 ){
				closesocket(sock);
				m_status = 1;
				return;
			}
		}
		Sleep(100);
	}
	closesocket(sock);
	return ;
}

//login
void cpnppr::login(){
 	mysock  = socket (AF_INET, SOCK_DGRAM, 0);
	struct timeval  tv_out;
	tv_out.tv_sec = 10;
	tv_out.tv_usec= 0;
	int n = setsockopt( mysock, SOL_SOCKET,SO_RCVTIMEO,(char*)&tv_out,sizeof(tv_out));
	printf( "setsockopt:%d,", n );

	sockaddr_in badr;
	static unsigned short xport = 12345;
	badr.sin_family = AF_INET;
	badr.sin_addr.s_addr = INADDR_ANY;
	badr.sin_port = htons( xport++ );
	bind(mysock,(sockaddr*)&badr, sizeof(sockaddr_in));
	printf("bind socket port:%d\n", xport);
	//登陆
	int addlen = sizeof(sockaddr);
	char buf[bufsize] = {0};
	unsigned char buf2[] = { 0xf1,0x41,0x00,0x14,0x56,0x53,0x54,0x43,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	char mark1[32];
	char mark2[32];
	unsigned int  camid;
	sscanf( m_source, "%*[^;];%[a-zA-Z]%d%s",mark1,&camid,mark2);
	camid = (htonl(camid));
	memcpy( buf2+4, mark1,strlen(mark1));
	memcpy( buf2+12, &camid, sizeof(int));
	memcpy( buf2+16, mark2,strlen(mark2));
	sendto(mysock, (char*)buf2,sizeof(buf2),0,(sockaddr*)&toaddr, addlen);
	memset( &buf,0x00, sizeof(buf));
	int lens = recvfrom(mysock,buf,bufsize,0,(sockaddr*)&toaddr,(socklen_t*)&addlen);
	if( lens > 0 ){
	}

	//确认
	unsigned char buf3[] = { 0xf1,0xE0,0x00,0x00};
	lens = sendto(mysock, (char*)buf3,sizeof(buf3),0,(sockaddr*)&toaddr, addlen);
	memset( &buf,0x00, sizeof(buf));
	lens = recvfrom(mysock,buf,bufsize,0,(sockaddr*)&toaddr,(socklen_t*)&addlen);
	//
	unsigned char buf4[]= {0xf1,0xd0,0x00,0xbe,0xd1,0x00,0x00,0x00,0x01,0x0a,0x00,0x00,0x61,0x00,0x00,0x00,0x47,0x45,0x54,0x20,0x2f,0x6c,0x69,0x76,0x65,0x73,0x74,0x72,0x65,0x61,0x6d,0x2e,0x63,0x67,0x69,0x3f,0x73,0x74,0x72,0x65,0x61,0x6d,0x69,0x64,0x3d,0x31,0x30,0x26,0x73,0x75,0x62,0x73,0x74,0x72,0x65,0x61,0x6d,0x3d,0x30,0x26,0x6c,0x6f,0x67,0x69,0x6e,0x75,0x73,0x65,0x3d,0x61,0x64,0x6d,0x69,0x6e,0x26,0x6c,0x6f,0x67,0x69,0x6e,0x70,0x61,0x73,0x3d,0x38,0x38,0x38,0x38,0x38,0x38,0x26,0x75,0x73,0x65,0x72,0x3d,0x61,0x64,0x6d,0x69,0x6e,0x26,0x70,0x77,0x64,0x3d,0x38,0x38,0x38,0x38,0x38,0x38,0x26,0x01,0x0a,0x00,0x00,0x49,0x00,0x00,0x00,0x47,0x45,0x54,0x20,0x2f,0x63,0x68,0x65,0x63,0x6b,0x5f,0x75,0x73,0x65,0x72,0x2e,0x63,0x67,0x69,0x3f,0x6c,0x6f,0x67,0x69,0x6e,0x75,0x73,0x65,0x3d,0x61,0x64,0x6d,0x69,0x6e,0x26,0x6c,0x6f,0x67,0x69,0x6e,0x70,0x61,0x73,0x3d,0x38,0x38,0x38,0x38,0x38,0x38,0x26,0x75,0x73,0x65,0x72,0x3d,0x61,0x64,0x6d,0x69,0x6e,0x26,0x70,0x77,0x64,0x3d,0x38,0x38,0x38,0x38,0x38,0x38,0x26};
	lens = sendto(mysock, (char*)buf4,sizeof(buf4),0,(sockaddr*)&toaddr, addlen);
	memset( &buf,0x00, sizeof(buf));
	lens = recvfrom(mysock,buf,bufsize,0,(sockaddr*)&toaddr,(socklen_t*)&addlen);
	//确认
	unsigned char buf5[] = { 0xf1,0xE0,0x00,0x00};
	lens = sendto(mysock, (char*)buf5,sizeof(buf5),0,(sockaddr*)&toaddr, addlen);
	memset( &buf,0x00, sizeof(buf));
	lens = recvfrom(mysock,buf,bufsize,0,(sockaddr*)&toaddr,(socklen_t*)&addlen);
	m_status = 2;

	memset( &buf,0x00, sizeof(buf));
	lens = recvfrom(mysock,buf,bufsize,0,(sockaddr*)&toaddr,(socklen_t*)&addlen);
}

void cpnppr::work(){
	static unsigned char buf[bufsize] = {0};
	static sockaddr_in addr;
	static int addlen = sizeof(sockaddr);
	memset(&addr,0,sizeof(sockaddr_in));
	int len = recvfrom(mysock,(char*)buf,bufsize,0,(sockaddr*)&toaddr,(socklen_t*)&addlen);
	//static size_t disc = 0;
	static size_t echo = 0;
	if(len<=0 && echo++>0x04/* && disc++>0x09 */){
		printf("close\n");
		closesocket( mysock );
		m_status = 0;
		mysock = 0;
		echo = 0;
		return;
	}else if(len==4 && echo++>0x06){
		printf("echo8\n");
		closesocket( mysock );
		Sleep(10);
		m_status = 1;	
		echo = 0;
		return;	
	}else if(len==4 && echo>0x04){
		printf("echo4\n");
		unsigned char frm[] = {0xf1,0xE0,0x00,0x00};
		sendto(mysock,(char*)frm,sizeof(frm),0,(sockaddr*)&toaddr,addlen);	
		//Sleep(10);
		return;	
	}else if(len<8){
		return;
	}
	//以下所接收的全为帧数据
	//JPEG流第1段数据
	if( len>42 && buf[40]==0xFF && buf[41]==0xD8 ){
		vframe.clear();
		vframe[buf[6]*256+buf[7]] = std::string((char*)(&buf[40]),len-40);
		vcomfirm.push_back( buf[6] );
		vcomfirm.push_back( buf[7] );
	}
	//JPEG最后一段数据
	else if( buf[len-2]==0xFF && buf[len-1] ==0xD9 ){
		vcomfirm.push_back( buf[6] );
		vcomfirm.push_back( buf[7] );
		vframe[buf[6]*256+buf[7]] = std::string((char*)(&buf[8]),len-8);
		//vFrame接收帧数据必须是连续的帧数据
		std::map<unsigned int,std::string>::iterator itb = vframe.begin(); 
		std::map<unsigned int,std::string>::iterator ite = vframe.end(); ite--;
		unsigned char startmark = (unsigned char)itb->second[0];
		if( 0xFF== startmark && ite->first-itb->first+1==vframe.size()){
			std::string jpegstream;
			for(std::map<unsigned int,std::string>::iterator it=vframe.begin();it!=vframe.end();it++)jpegstream += it->second;
			setframe( &jpegstream[0], (int)jpegstream.size());
			static unsigned char buf3[] = { 0xf1,0xE0,0x00,0x00};
			sendto(mysock, (char*)buf3,sizeof(buf3),0,(sockaddr*)&toaddr,addlen);
		}
		vframe.clear();
	}
	//JPEG中间数据
	else{
		vframe[buf[6]*256+buf[7]] = std::string((char*)(&buf[8]),len-8);
		vcomfirm.push_back( buf[6] );
		vcomfirm.push_back( buf[7] );
	}
	//确认数据发送
	if( vcomfirm.size() > 0 ){
		std::string vdata;
		vdata.push_back(0xF1);
		vdata.push_back(0xD1);
		unsigned short n = (unsigned short)(vcomfirm.size() + 4);
		vdata.push_back( n >> 8 );
		vdata.push_back( 0xFF & n );
		vdata.push_back(0xD1);
		vdata.push_back(0x01);
		vdata.push_back(0x00);
		vdata.push_back( (char)(vcomfirm.size()/2) );
		vdata.append(vcomfirm.begin(),vcomfirm.end());
		sendto(mysock,(char*)&vdata[0],(int)vdata.size(),0,(sockaddr*)&toaddr, addlen);
		vcomfirm.clear();
	}
	echo = 0;
	//disc = 0;
	return;
}

//接收处理线
winapi  cpnppr::playthread(void* lpParam){
	cpnppr * fpr = (cpnppr*)lpParam;
	while(0!=event_timedwait(fpr->m_playevt,30)){
		switch( fpr->m_status ){
		case 0: fpr->query(); break;
 		case 1: fpr->login(); break;
		case 2: fpr->work(); break;
		} 
	}
	event_set( fpr->m_playevt );
	printf("pthread exist\r\n");
	return   0; 
}

 void cpnppr::move( int dir, int tms){
	 char buf[bufsize];
	 static unsigned char seq = 0;
	 unsigned char header[] = { 0xf1,0x0d0,0x00,0x6e,0xd1,0x00,0x00,seq++,0x01,0x0a,0x00,0x00,0x62,0x00,0x00,0x00};
	 int  headln = sizeof(header);
	 memcpy(buf,header,headln);
	 int sendln = sprintf( &buf[headln], "GET /decoder_control.cgi?command=%d&onestep=0&loginuse=admin&loginpas=888888&user=admin&pwd=888888&",dir );
	 sendto(mysock,buf,headln+sendln,0,(sockaddr*)&toaddr, sizeof(sockaddr_in));
	 vcomfirm.push_back( buf[6] );
	 vcomfirm.push_back( buf[7] );
	 //memset( &buf,0x00, sizeof(buf));
	 //int recvln = recvfrom(mysock,buf,bufsize,0, (struct sockaddr FAR*)&toaddr, (int FAR*)&length);
	 Sleep( tms );
	 header[7] = seq++;
	 memcpy(buf,header,headln);
	 sendln = sprintf( &buf[headln], "GET /decoder_control.cgi?command=%d&onestep=0&loginuse=admin&loginpas=888888&user=admin&pwd=888888&",dir+1);
	 sendto(mysock,buf,headln+sendln,0,(sockaddr*)&toaddr, sizeof(sockaddr_in));
	 vcomfirm.push_back( buf[6] );
	 vcomfirm.push_back( buf[7] );
	 //memset( &buf,0x00, sizeof(buf));
	 //recvln = recvfrom(mysock,buf,bufsize,0, (struct sockaddr FAR*)&toaddr, (int FAR*)&length);
}
