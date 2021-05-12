#include "websock.h"
#include "sha1.h"

extern int responsetolua( unsigned int act, char* data, int len );

//websocket握手
int websock::handshake(const char* key, char* rsp){
	char keys[1024]={0};
	unsigned char RetDigest[20];
	int n = sprintf(keys,"%s%s",key, "258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
	SHA1_CTX context;
	CSha1::SHA1Init(&context);
	CSha1::SHA1Update(&context, (unsigned char*)keys, n);
	CSha1::SHA1Final(RetDigest, &context);
	int bufLen = sprintf(rsp,"HTTP/1.1 101 Switching Protocols\r\nUpgrade: WebSocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept: %s\r\n\r\n",
									base64_encode((unsigned char*)RetDigest,20).c_str());
	return bufLen;
}

//wesocket数据解析
int websock::decodeframe(const char * frm, size_t len, std::string &outMessage){
     int ret = 0;
     if (len < 2){
         ret = WS_ERROR_FRAME;
     }
     // 检查扩展位并忽略  
     if ((frm[0] & 0x70) != 0x0){
         ret = WS_ERROR_FRAME;
     }
     // fin位: 为1表示已接收完整报文, 为0表示继续监听后续报文  
     ret = (frm[0] & 0x80);
     if ((frm[0] & 0x80) != 0x80){
         ret = WS_ERROR_FRAME;
     }
     // mask位, 为1表示数据被加密  
     if ((frm[1] & 0x80) != 0x80){
         ret = WS_ERROR_FRAME;
     }
     //操作码  
     unsigned short payloadLength = 0;
     unsigned char payloadFieldExtraBytes = 0;
     unsigned char opcode = static_cast<unsigned char>(frm[0] & 0x0f);
     if (opcode == WS_TEXT_FRAME){
         // 处理utf-8编码的文本帧  
         payloadLength = static_cast<unsigned short>(frm[1] & 0x7f);
         if (payloadLength == 0x7e){
             unsigned short payloadLength16b = 0;
             payloadFieldExtraBytes = 2;
             memcpy(&payloadLength16b, &frm[2], payloadFieldExtraBytes);
             payloadLength = ntohs(payloadLength16b);
         }else if (payloadLength == 0x7f){  // 数据过长,暂不支持  
             ret = WS_ERROR_FRAME;
         }
     }else if (opcode == WS_BINARY_FRAME || opcode == WS_PING_FRAME || opcode == WS_PONG_FRAME){ // 二进制/ping/pong帧暂不处理 
     }
     else if (opcode == WS_CLOSING_FRAME){
         ret = WS_CLOSING_FRAME;
     }else{
         ret = WS_ERROR_FRAME;
     }
     // 数据解码  
     if ((ret != WS_ERROR_FRAME) && (payloadLength > 0)){
         // header: 2字节, masking key: 4字节  
         const char *maskingKey = &frm[2 + payloadFieldExtraBytes];
         char *payloadData = new char[payloadLength + 1];
         memset(payloadData, 0, payloadLength + 1);
         memcpy(payloadData, &frm[2 + payloadFieldExtraBytes + 4], payloadLength);
         for (int i = 0; i < payloadLength; i++){
             payloadData[i] = payloadData[i] ^ maskingKey[i % 4];
         }
         outMessage = payloadData;
         delete[] payloadData;
     }
     return ret;
 }

//base64_encode
std::string websock::base64_encode(unsigned char* data, unsigned int in_len) {
	static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	std::string ret;
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];
	while(in_len--) {
		char_array_3[i++] = *(data++);
		if (i == 3) {
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;
			for(i = 0; (i <4) ; i++) ret += base64_chars[char_array_4[i]];
			i = 0;
		}
	}
	if(i){
		for(j = i; j < 3; j++) char_array_3[j] = '\0';
		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;
		for (j = 0; (j < i + 1); j++) ret += base64_chars[char_array_4[j]];
		while((i++ < 3)) ret += '=';
	}
	return ret;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//tcp服务端
int websock::start(const char* ip, short port){
#ifdef WIN32
	WSADATA wsdata;
	WSAStartup(0x0202, &wsdata);
#endif
	//创建SOCKET
	m_bExit = false;
	sockaddr_in myaddr;
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = inet_addr(ip);
	myaddr.sin_port = htons(port);
	mysocket = socket(AF_INET, SOCK_STREAM, 0);
	memset(fdarray, 0, sizeof(fdarray));
	myclient = 0;
	/* Set port reuse */
	int opt = 1;
#ifdef WIN32
	if (setsockopt(mysocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt)) < 0)
		return -1;
#else
	if (setsockopt(mysocket, SOL_SOCKET, SO_REUSEADDR, (void*)&opt, sizeof(opt)) < 0)
		return -1;
#endif
	//梆定
	int ibind = bind(mysocket, (sockaddr*)&myaddr, sizeof(sockaddr_in));
	if (ibind != 0) {
		printf("websock bind socket error! addr=%s:%d\n", ip, port);
		return ibind;
	}
	//设置监听
	int ilisten = listen(mysocket, MAX_CLIENT/2);
	if (ilisten != 0) {
		printf("websock listen error!\n");
		return ilisten;
	}
	//启动监听线程
#ifdef WIN32
	unsigned long nMode = 1;
	ioctlsocket(mysocket, FIONBIO, &nMode);
	threadid = _beginthreadex(NULL, 0, workthread, this, 0, NULL);
#else
	fcntl(mysocket, F_SETFL, O_NONBLOCK);
	if (pthread_create(&threadid, NULL, workthread, this) != 0)
		printf("pthread_create failed! \n");
#endif
	return 0;
}

//tcps服务停止
int websock::stop(){
	m_bExit = true;
	closesocket(mysocket);
	for (unsigned int i = 0; i< MAX_CLIENT; i++) {
		if (fdarray[i])closesocket(fdarray[i]);
	}
#ifdef WIN32
	WSACleanup();
#endif
	return 0;
}


//接受连结和接收数据
winapi websock::workthread(void* lpParam) {
	websock * svr = (websock*)lpParam;
	while (!svr->m_bExit) {
		fd_set fdRead;
		FD_ZERO(&fdRead);
		FD_SET(svr->mysocket, &fdRead);
		for (int nLoopi = 0; nLoopi < MAX_CLIENT; nLoopi++) {
			if (svr->fdarray[nLoopi] != 0)
				FD_SET(svr->fdarray[nLoopi], &fdRead);
		}
		//调用select模式进行监听
		timeval tv = { 0,100000 };
		static SOCKET maxfd = svr->mysocket;
		if (select(maxfd + 1, &fdRead, NULL, NULL, &tv) <= 0)
			continue;
		//ACCEPT CONNECT
		if (FD_ISSET(svr->mysocket, &fdRead)) {
			sockaddr_in addrclt;
			bool bAccept = false;
			int  addlen = sizeof(addrclt);
			SOCKET aptclt = accept(svr->mysocket, (struct sockaddr*)&addrclt, (socklen_t*)&addlen);
			for (int nLoopi = 0; aptclt>0 && nLoopi<MAX_CLIENT; nLoopi++) {
				if (svr->fdarray[nLoopi] == 0) {
					svr->fdarray[nLoopi] = aptclt;
#ifdef WIN32
					unsigned long nMode = 1;
					ioctlsocket(aptclt, FIONBIO, &nMode);
#else
					fcntl(aptclt, F_SETFL, O_NONBLOCK);
#endif
					FD_SET(svr->fdarray[nLoopi], &fdRead);
					if (maxfd<aptclt)maxfd = aptclt;
					bAccept = true;
					break;
				}
			}
			if (!bAccept) {
				closesocket(aptclt);
			}
			continue;
		}
		//RECV 数据
		for (int nLoopi = 0; nLoopi < MAX_CLIENT; nLoopi++) {
			if (FD_ISSET(svr->fdarray[nLoopi], &fdRead)) {
				char data[MAX_BUF_SIZE] = { 0 };
				int cnt = recv(svr->fdarray[nLoopi], data, sizeof(data), 0); 
				if(cnt <= 0 || !svr->dispath(svr->fdarray[nLoopi], data, cnt)){   //如何区分断开和超时
					printf("websocket close socket\n");
					closesocket(svr->fdarray[nLoopi]);
					FD_CLR(svr->fdarray[nLoopi], &fdRead);
					svr->fdarray[nLoopi] = 0;
					svr->myclient = -1;
				}
			}
		}
	}
	return 0;
}

//web 请求处理
bool websock::dispath(SOCKET sock, char* buf, long len) {
	if( myclient != sock ){
		std::map<std::string, std::string> arghttp;
		anyhttphead(buf, len, arghttp);
		//对websocket请求处理
		std::map<std::string, std::string>::iterator ifnd = arghttp.find("Upgrade");
		if (ifnd != arghttp.end() && ifnd->second.find("websocket") != size_t(-1)) {
			std::string skey = arghttp["Sec-WebSocket-Key"];
			skey.erase(0, skey.find_first_not_of(" "));
			char send_buf[1024] = { 0 };
			int buflen = handshake(skey.c_str(), send_buf);
			if( send(sock,(const char*)send_buf,buflen,0) == buflen) {
				char * protocol = (char*)arghttp["Sec-WebSocket-Protocol"].c_str();
				myclient = sock;
				if( strlen(protocol) > 0 )
					responsetolua(1, protocol, strlen(protocol) );
			}
		}
		return true;
	}
	//recv from
	std::string data;
	decodeframe(buf, len, data);
	int ret = responsetolua(2, &data[0], data.length());
	return (ret==0)?true:false;
}

//发送文本数据
int websock::sends(const char* str){
	if (!myclient) 
		return -1;
	int nsent = 0;
	size_t len = strlen( str );
	unsigned char szhead[4] = {0}; 
	szhead[0] = static_cast<unsigned char>(0x80|WS_TEXT_FRAME);
	if ( len <= 0x7d){
		szhead[1] = static_cast<unsigned char>(len) ;
		nsent =send(myclient,(char*)szhead,2,0);
	}else{
		szhead[1] = 0x7e;
		szhead[2] = static_cast<unsigned char>(len>>8);
		szhead[3] = static_cast<unsigned char>(len);
		nsent=send(myclient,(char*)szhead,4,0); 
	}
	nsent += send(myclient,(char*)str,len,0); 
	if (nsent <= 0) { myclient = 0; }
	return nsent;
}

//发送二进制数据
int websock::sends( unsigned char* buf,size_t len ){
	if (!myclient)
		return -1;
	int nsent = 0;
	unsigned char szhead[10] = {0};
	szhead[0] = static_cast<unsigned char>(0x80|WS_BINARY_FRAME);
	if ( len <= 0x7E){
		szhead[1] = static_cast<unsigned char>(len) ;
		nsent = send(myclient,(char*)szhead,2,0);
	}else if(len < 0xFFFF){
		szhead[1] = 0x7E;
		szhead[2] = static_cast<unsigned char>(len>>8);
		szhead[3] = static_cast<unsigned char>(len);
		nsent = send(myclient,(char*)szhead,4,0); 
	}else {
		szhead[1] = 0x7F;
		szhead[2] = 0;
		szhead[3] = 0;
		szhead[4] = 0;
		szhead[5] = 0;
		szhead[6] = static_cast<unsigned char>(len >> 24);
		szhead[7] = static_cast<unsigned char>(len >> 16);
		szhead[8] = static_cast<unsigned char>(len >> 8);
		szhead[9] = static_cast<unsigned char>(len & 0xFF);
		nsent = send(myclient, (char*)szhead, 10, 0);
	}
	nsent += send(myclient, (char*)buf,len,0); 
	if (nsent <= 0){myclient = 0;}
	return nsent;
}

//获取传输任务
int websock::gettrans( std::vector<std::string> &tv ){
	if( 0 != event_wait( mytaskcs ) )
		return 0;
	event_reset( mytaskcs );
	std::swap( tv, vsendbuf );
	event_set(mytaskcs);
	return tv.size();
}

//http解析
bool websock::anyhttphead(char* buf, size_t len, std::map<std::string,std::string>& argv){
	char *p=buf;
	char* end = buf+len-2;
	char* pname = NULL;
	char* pval  = NULL;
	for( ;p<end-1; p++){
		if(*p=='\r' && *(p+1)=='\n') {
			p+=2;
			pname = p;
			break;
		}
	}
 	for(;p<end-1;p++){   
		if( !pval && *p ==':' ){
			*p = 0;
			pval = p+1;
			continue;
		}
		if(*p=='\r' && *(p+1)=='\n'){
			*p = 0;
			argv[pname] = pval;
			pval = NULL;
			pname = p + 2;
			continue;
		}
	}
	return true;
}

int websock::getip( const char* nc, char* ip ){
#ifdef WIN32
	char host_name[255]={0};
    if(gethostname(host_name,sizeof(host_name))==SOCKET_ERROR){  
        printf("Error %d when getting local host name\n",WSAGetLastError());  
        return -1;  
    }  
    printf("host name:%s\n",host_name);  
    struct hostent *phe=gethostbyname(host_name);  
    if(phe==0){  
        printf("Error host lookup\n");  
        return -1;  
    }  
    struct in_addr addr;  
    memcpy(&addr,phe->h_addr_list[0],sizeof(struct in_addr));  
	sprintf(ip,"%s",inet_ntoa(addr)); 
#else
	int sockfd;
	struct ifconf ifconf;
	char buf[512]={0};
	struct ifreq *ifreq;
	ifconf.ifc_len = 512;
	ifconf.ifc_buf = buf;
	if((sockfd = socket(AF_INET,SOCK_DGRAM,0))<0){
		return -1;
	}  
	ioctl(sockfd, SIOCGIFCONF, &ifconf);
	ifreq = (struct ifreq*)buf;  
	for(int i=(ifconf.ifc_len/sizeof(struct ifreq)); i>0; i--){
		if( strcmp( ifreq->ifr_name, nc ) == 0 ){
			sprintf(ip, "%s",inet_ntoa(((struct sockaddr_in*)&(ifreq->ifr_addr))->sin_addr));
			break;
		}
		ifreq++;
	}
	close(sockfd);
#endif
    return 0;  
} 

//异步发送线程
winapi websock::sending(void* lpParam){
	websock * fpr = (websock*)lpParam;
	while( !fpr->m_bExit ){
		event_timedwait(fpr->workevent, 30);
		std::vector<std::string> tv;
		int nsize = fpr->gettrans( tv );
		for( int i = 0; i<nsize; i++ ){
			fpr->sends( (unsigned char*)(&tv[i][0]), tv[i].size());
		}
		tv.clear();
	}
	event_set(fpr->workevent);
	printf("pthread exist\n");
	return   0; 
}


