#include "websvr.h"
#include "taskmgr.h"
#include <sstream>
#include <time.h>

#ifdef _WIN32
#include <direct.h>
#include <process.h>
#include <io.h>
#define ACCESS _access   
#define MKDIR(a) _mkdir((a))   
#else
#define MAX_PATH 256
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <arpa/inet.h>
#include <sys/unistd.h>
#include <sys/ioctl.h>
#include <sys/fcntl.h>
#endif

#define MAX_BUF_SIZE 1024*512*2

//web server 
void setpath(const char* path) {
	char folder[256] = { 0 };
	sprintf_s(folder, sizeof(folder), "%s/", path);
	for (size_t i = 1; i<strlen(folder); i++) {
		if (folder[i] == '/' || folder[i] == '\\') {
			folder[i] = 0;
			if (ACCESS(folder, 0) != 0) {
				if (MKDIR(folder) == -1) {
					perror("mkdir error");
					return;
				}
			}
			folder[i] = '/';
		}
	}
}
 
int tcps::start(const char* ip, short port){
	printf("start web service,ip:%s port:%d\n",ip,port);
#ifdef _WIN32
	WSADATA wsdata;
	WSAStartup(0x0202,&wsdata );
#endif
	//sock
	m_bExit = false;
	sockaddr_in myaddr;
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = inet_addr( ip );
	myaddr.sin_port = htons( port );
	mysocket = socket( AF_INET, SOCK_STREAM, 0 );
	memset(fdarray,0,sizeof(fdarray));
	/* Set port reuse */
	int opt = 1;
#ifdef _WIN32
	if (setsockopt(mysocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt)) < 0)
		return -1;
#else
	if (setsockopt(mysocket, SOL_SOCKET, SO_REUSEADDR, (void*)&opt, sizeof(opt)) < 0)
		return -1;
#endif
	//bind
	int ibind = bind( mysocket, (sockaddr*)&myaddr, sizeof(sockaddr_in));
	if( ibind != 0 ){
		printf("websvr bind socket error! addr=%s:%d\n",ip,port);
		return ibind;
	}
	//listen
	int ilisten = listen(mysocket, MAX_CLIENT);  
	if( ilisten != 0  ){
		printf("websvr listen error!\n");
		return ilisten;
	}
#ifdef _WIN32
	unsigned long nMode = 1;
	ioctlsocket(mysocket,FIONBIO,&nMode);
	threadid = _beginthreadex(NULL,0,workthread,this,0,NULL);
#else
	fcntl(mysocket,F_SETFL,O_NONBLOCK);
	if( pthread_create(&threadid, NULL, workthread, this) != 0) 
		printf("pthread_create failed! \n");
#endif
	return 0;
}

//tcp server stop
void tcps::stop(){
	m_bExit = true;
#ifdef _WIN32
	closesocket(mysocket);
	for(unsigned int i=0; i< MAX_CLIENT; i++ ){
		if( fdarray[i] )closesocket(fdarray[i]);
	}
	WSACleanup();
#else
	close(mysocket);
	for(unsigned int i=0; i< MAX_CLIENT; i++ ){
		if( fdarray[i] )close(fdarray[i]);
	}
#endif
}

#ifdef _WIN32
unsigned int tcps::workthread(void* lpParam){
#else
void* tcps::workthread(void* lpParam){
#endif
	tcps * svr = (tcps*)lpParam; 
	while( !svr->m_bExit ){
		fd_set fdRead;
		FD_ZERO(&fdRead);
		FD_SET( svr->mysocket, &fdRead );  
		for( int nLoopi = 0; nLoopi < MAX_CLIENT; nLoopi++ ){
			if( svr->fdarray[nLoopi] !=0 )
				FD_SET( svr->fdarray[nLoopi], &fdRead );
		}
		//select
		timeval tv={0,100000};
#ifdef _WIN32
		static SOCKET maxfd = svr->mysocket; 
#else
		static int maxfd = svr->mysocket;
#endif
		if( select( maxfd + 1, &fdRead, NULL, NULL, &tv ) <= 0 )
			continue;
	    //ACCEPT CONNECT
		if( FD_ISSET( svr->mysocket, &fdRead ) ){
			sockaddr_in addrclt;
			bool bAccept = false;
			#ifdef _WIN32
			int  addlen = sizeof(addrclt);
			SOCKET aptclt = accept(svr->mysocket, (struct sockaddr*)&addrclt, &addlen);
			#else
			socklen_t addlen = sizeof(addrclt);
			int aptclt = accept(svr->mysocket, (struct sockaddr*)&addrclt, &addlen);
			#endif	
			for(int nLoopi=0; aptclt>0 && nLoopi<MAX_CLIENT; nLoopi++ ){
				if( svr->fdarray[nLoopi] == 0 ){
					svr->fdarray[nLoopi] = aptclt;
					#ifdef _WIN32
					unsigned long nMode = 1;
					ioctlsocket(aptclt, FIONBIO, &nMode);
					#else
					fcntl(aptclt, F_SETFL, O_NONBLOCK);
					#endif
					FD_SET(svr->fdarray[nLoopi] , &fdRead);  
					if (maxfd<aptclt)maxfd = aptclt; 
					bAccept = true;
					break;
				}
			}
			if( !bAccept ){
				#ifdef _WIN32
				closesocket( aptclt );
				#else
				close( aptclt );
				#endif
			}
			continue;
		}
		//RECV
		for( int nLoopi = 0; nLoopi < MAX_CLIENT; nLoopi++ ){
			if( FD_ISSET(svr->fdarray[nLoopi], &fdRead) ){
				static char data[MAX_BUF_SIZE] = {0};
				memset(data, 0x00, MAX_BUF_SIZE);
				int cnt = 0;
				int tln = 0;
				do {
					cnt = recv(svr->fdarray[nLoopi], &data[tln], MAX_BUF_SIZE -tln, 0);
					tln += cnt;
				} while (cnt > 0);
				if (tln<0  || !svr->dispath(svr->fdarray[nLoopi], data, tln)){
					#ifdef _WIN32
					closesocket(svr->fdarray[nLoopi]);
					#else
					close(svr->fdarray[nLoopi]);
					#endif
				}
				FD_CLR( svr->fdarray[nLoopi], &fdRead );
				svr->fdarray[nLoopi] = 0;
			}
		}
	}
	return 0;
}

//send the buffer
//int tcps::sends(SOCKET sock, void* buf,int size){
int tcps::sends(int sock, void* buf,int size){
	int sendlen = 0;
	int sent = 0;
	for( sent=0,sendlen=0;sendlen<size; sendlen+= sent ){
		sent = send( sock, (char*)buf+sendlen, size-sendlen, 0);
		if( sent <= 0 )
			break;
	}
	return sent;
}

int cwebsvr::start(const char* ip, short port, const char* path) {
	strncpy(webpath, path, MAX_PATH);
	return tcps::start(ip, port);
}

//web directory
void cwebsvr::setwebpath(const char* path){
	strncpy(webpath,path,MAX_PATH);
}

//HTTP contentType
std::string cwebsvr::getcontent(std::string ftype){
	std::string contentType;
	if(ftype=="png")
		contentType="image/png";
	else if(ftype=="jpeg" || ftype == "jpg")
		contentType = "image/jpeg";
	else if(ftype=="json" )
		contentType = "application/json";
	else if(ftype=="gif")
		contentType = "image/gif";
	else if(ftype=="html" || ftype=="htm")
		contentType="text/html";
	else if(ftype=="php")
		contentType = "text/html";
	else if(ftype=="js")
		contentType = "application/x-javascript";
	else if(ftype=="css")
		contentType = "text/css";
	else 
		contentType = "text/html";
	return contentType;
}

//http GMTIME
std::string cwebsvr::getgmtime(){
	static char szBuf[128]={0}; 
	time_t tnow = time(NULL);        
	strftime(szBuf ,127 ,"%a,%d %b %Y %H:%M:%S",gmtime( &tnow ) ); 
	return szBuf;
}

void cwebsvr::analysehead(std::stringstream& ssHeader, std::map<std::string, std::string>& v, int& httphl) {
	char szline[256] = { 0 };
	ssHeader.getline(szline, sizeof(szline), '\r');
	httphl += strlen(szline) + 1;
	char sztype[32] = { 0 };
	char szurl[256] = { 0 };
	char szargs[256] = { 0 };
	char szfileex[256] = { 0 };
	sscanf(szline, "%[^ ] %[^( |?)]?%[^ ]*", sztype, szurl, szargs);
	sscanf(szurl, "%*[^.].%s", szfileex);
	v["METHORD"] = sztype;
	v["TARGET"] = szurl;
	v["ARGS"] = szargs;
	v["TYPE"] = szfileex;
	//httpcontext
	for (;; ) {
		std::string skey;
		std::string sval;
		ssHeader.getline(szline, sizeof(szline), '\r');
		httphl += strlen(szline) + 1;
		size_t count = strspn(szline, "\r\n");
		char * start = szline + count;
		if (0 == strlen(start))
			break;
		char *p = strchr(start, ':');
		if (p - start > 0)
			skey.assign(start, p - start);
		else
			break;
		count = strspn(p, ": ");
		sval.assign(p + count);
		v[skey] = sval;
	}
}

void ReplaceAll(std::string& strSource, const std::string& strOld, const std::string& strNew){
	size_t nPos = 0;
	while ((nPos = strSource.find(strOld, nPos)) != strSource.npos){
		strSource.replace(nPos, strOld.length(), strNew);
		nPos += strNew.length();
	}
}

std::string cwebsvr::description( std::string val ) {
	val += ';';
	ReplaceAll(val, "\"", "");
	ReplaceAll(val, " ", "");
	std::map < std::string, std::string > vcontext;
	int npos = 0;
	std::string skey;
	std::string sitem;
	for (size_t i = 0; i < val.size(); i++) {
		if (val[i] == ';') {
			sitem = val.substr(npos, i - npos);
			npos = i + 1;
			if (skey.size() > 0) {
				vcontext[skey] = sitem;
			}
		}
		if (val[i] == '=') {
			skey = val.substr(npos, i - npos);
			npos = i + 1;
		}
	}
	return vcontext["filename"];
}

//uploadfiletrans
bool cwebsvr::uploadfiletrans(int sock, char* buf, unsigned int size) {
	if (!mytranfile)
		return false;
	if (upfilesize <= recvbuflen + size) {
		fwrite(buf, size - markidLen - 1, 1, mytranfile);
		fclose(mytranfile);
		mytranfile = NULL;
		recvbuflen = 0;
		upfilesize = 0;
		markidLen  = 0;
		static char send_buf[1024] = { 0 };
		memset(send_buf, 0x00, sizeof(send_buf));
		int len = sprintf(send_buf, "HTTP/1.1 200 OK\r\n \
						Date: %s GMT\r\n \
						Server: smartx\r\n \
						Content-Length: 0\r\n \
						Connection: keep-alive\r\n \
						Keep-Alive: timeout=5, max=100\r\n \
						Content-Type: text/html; charset=UTF-8\r\n\r\n",
						getgmtime().c_str());
		sends(sock, send_buf, len);
#ifdef _WIN32
		closesocket(sock);
#else
		close(sock);
#endif
		return true;
	}
	recvbuflen += size;
	fwrite(buf, size, 1, mytranfile);
	return false;
}

//web quest dipath
bool cwebsvr::dispath(int sock,char* buf, unsigned int len){
	if ( mytranfile ) {
		return uploadfiletrans(sock, buf, len);
	}
	return taskmgr::posttask(taskmgr::WEB, 
		base::Bind(&cwebsvr::request,
		base::Unretained(this), sock, std::string(buf,len+1)));
}

void cwebsvr::replay(int sock, std::string data) {
	std::string now = getgmtime();
	static char send_buf[MAX_BUF_SIZE];
	memset(send_buf, 0x00, sizeof(send_buf));
	int bufLen = sprintf(send_buf, "HTTP/1.1 200 OK \r\n \
								   Date: %s GMT\r\n \
								   Expires: %s GMT\r\n \
								   Cache-Control: private, max-age=31536000\r\n \
								   X-Content-Type-Options: nosniff\r\n \
								   Server: smartx\r\n \
								   Content-Type: application/json\r\n",
								   now.c_str(),
								   now.c_str());
	 bufLen += sprintf(send_buf + bufLen, "Content-Length: %ld\r\n\r\n%s", data.length(),data.c_str());
	 sends(sock, send_buf, bufLen);
	 #ifdef _WIN32
	 closesocket(sock);
	 #else
	 close(sock);
	 #endif
}

void cwebsvr::request(int sock, std::string data) {
	int httpheadlen = 0;
	std::map<std::string, std::string> v;
	std::stringstream ssHeader(data);
	analysehead(ssHeader, v, httpheadlen );
	//web request methord
	if("UPFILE" == v["METHORD"]) {
		char szline[256] = { 0 };
		ssHeader.getline(szline, sizeof(szline), '\r');
		int postheadlen = strlen(szline) + 1;
		std::string sval(szline);
		sval.erase(std::remove(sval.begin(), sval.end(), '\n'), sval.end());
		sval.erase(std::remove(sval.begin(), sval.end(), '-'), sval.end());
		v["FILE_ID"] = sval;
		for (;;) {
			std::string skey;
			std::string sval;
			char szline[256] = { 0 };
			ssHeader.getline(szline, sizeof(szline), '\r');
			postheadlen += strlen(szline) + 1;
			size_t count = strspn(szline, "\r\n");
			char * start = szline + count;
			if (0 == strlen(start))
				break;
			char *p = strchr(start, ':');
			if (p - start > 0)
				skey.assign(start, p - start);
			else
				continue;
			count = strspn(p, ": ");
			sval.assign(p + count);
			v[skey] = sval;
		}
		postheadlen += 1;
		//upload file
		if (v.find("Content-Disposition") != v.end()) {
			markidLen = v["FILE_ID"].size() + 4;
			upfilesize = atoi(v["Content-Length"].c_str()) - markidLen - postheadlen;
			std::string filepath = "." + v["TARGET"];
			std::string filename = filepath + description(v["Content-Disposition"]);
			printf("upfile id:%s  name:%s\n", v["FILE_ID"].c_str(), filename.c_str());
			setpath(filepath.c_str());
			mytranfile = fopen(filename.c_str(), "wb");
			int datpos = httpheadlen + postheadlen;
			uploadfiletrans(sock, &data[datpos], data.size() - datpos);
		}
		return;
	}
	else if ("LUAGET" == v["METHORD"]) {
		std::string arg = v["TARGET"];
		int n = arg.find_first_of('=');
		std::string ifunc = arg.substr(1, n-1);
		std::string args  = arg.substr(n+1);
		onwebtask(sock, ifunc, args);
		return;
	}
	else if ("LUAPOST" == v["METHORD"]) {
		std::string ifunc = v["TARGET"].substr(1);
		std::string args  = data.substr(httpheadlen + 1).c_str();
		onwebtask(sock, ifunc, args);
		return;
	}
    //static file transport
	std::string cont = getcontent(v["TYPE"]);
	std::string nowt = getgmtime();
	static char send_buf[MAX_BUF_SIZE];
	memset(send_buf, 0x00, sizeof(send_buf));
	int bufLen = sprintf(send_buf, "HTTP/1.1 200 OK \r\n \
									Date: %s GMT\r\n \
									Expires: %s GMT\r\n \
									Cache-Control: private, max-age=31536000\r\n \
									X-Content-Type-Options: nosniff\r\n \
									Server: smartx\r\n \
									Content-Type: %s\r\n",
									nowt.c_str(),
									nowt.c_str(),
									cont.c_str());
	//File trans
	std::string respath = webpath;
	respath += v["TARGET"];
	FILE* f = fopen(respath.c_str(), "rb");
	if (f == NULL) {
		static std::string defPage = "<html><b><center>404 not find!</center></b></html>";
		bufLen += sprintf(send_buf + bufLen, "Content-Length: %ld\r\n\r\n%s", defPage.size(), defPage.c_str());
		sends(sock, send_buf, bufLen);
		#ifdef _WIN32
		closesocket(sock);
		#else
		close(sock);
		#endif
		return;
	}
	fseek(f, 0, SEEK_END);
	int pos = ftell(f);
	bufLen += sprintf(send_buf + bufLen, "Content-Length: %d\r\n\r\n", pos);
	sends(sock, send_buf, bufLen);
	fseek(f, 0, SEEK_SET);
	int nbs = MAX_BUF_SIZE;
	int npieces = (pos + nbs - 1) / nbs;
	for (int i = 0; i < npieces; i++) {
		int fl = fread(send_buf, 1, nbs, f);
		sends(sock, send_buf, fl);
	}
	fclose(f);
	#ifdef _WIN32
	closesocket(sock);
	#else
	close(sock);
	#endif
}
