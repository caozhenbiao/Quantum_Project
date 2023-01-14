#include <stdio.h>
#include <assert.h>
#ifdef WIN32
#include <winsock.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <arpa/inet.h>
#include <sys/unistd.h>
#include <sys/ioctl.h>
#include <sys/fcntl.h>
#endif


#include <string.h>
#include "http.h"
#define MAX_CLIENT 64
#define MAX_BUF_SIZE 10240

int exit_mark;
int(*business_dispath)(char*, char*, unsigned,char*);
int fdarray[MAX_CLIENT];
unsigned threadid;
SOCKET mysocket;

static char * response_header_style = "HTTP/1.1 200 OK \r\n \
Date: %s GMT\r\n \
Expires: %s GMT\r\n \
Cache-Control: private, max-age=31536000\r\n \
X-Content-Type-Options: nosniff\r\n \
Server: smartx\r\n \
Content-Type: %s\r\n \
Content - Length: %ld\r\n\r\n";

//http GMTIME
char* get_gmtime(char* szBuf, int delay) {
	time_t tnow = time(NULL) + delay;
	strftime(szBuf, 127, "%a,%d %b %Y %H:%M:%S", gmtime(&tnow));
	return szBuf;
}

//send the buffer
int tcps_sends(int sock, void* buf, int size) {
	int send_len = 0;
	int sent_len = 0;
	for (sent_len = 0, send_len = 0; send_len < size; send_len += sent_len) {
		sent_len = send(sock, (char*)buf + send_len, size - send_len, 0);
		if (sent_len <= 0)
			break;
	}
	return sent_len;
}

//http 请求处理
int http_request_handle(int cs, char* data, unsigned len) {
	char uri[256] = { 0 };
	char args[256] = { 0 };
	char methord[32] = { 0 };
	sscanf(data, "%[^ ] %[^( |?)]?%[^ ]*", methord, uri, args);
	char * data_ptr = NULL;
	size_t data_len = 0;
	size_t head_len = 20;
	while (head_len < len) {
		size_t count = strspn(data + head_len, "\r\n\r\n");
		if (4 == count) {
			data_ptr = data + head_len + 4;
			data_len = len - head_len - 3;
			break;
		}
		head_len += (count + 1);
	}
	char* out_data = (char*)malloc(sizeof(char*));
	int out_data_len = business_dispath(uri, data_ptr, data_len, &out_data);
	char response_header[1024];
	memset(response_header, 0x00, sizeof(response_header));
	char inspire[128] = { 0 };
	char expires[128] = { 0 };
	get_gmtime(inspire, 0);
	get_gmtime(expires, 3600);
	int hdr_len = sprintf(response_header, response_header_style, inspire, expires, "LUA", len);
	tcps_sends(cs, response_header, hdr_len);
	tcps_sends(cs, out_data, out_data_len);
	free(out_data);
	return 0;
}

//tcp 服务端处理线程 
unsigned __stdcall tcps_workthread(void* param) { 
	while (!exit_mark) {
		fd_set fdRead;
		FD_ZERO(&fdRead);
		FD_SET(mysocket, &fdRead);
		for (int nLoopi = 0; nLoopi < MAX_CLIENT; nLoopi++) {
			if (fdarray[nLoopi] != 0)
				FD_SET(fdarray[nLoopi], &fdRead);
		}
		//select
		struct timeval tv = { 0,10000 };
#ifdef _WIN32
		SOCKET maxfd = mysocket;
#else
		int maxfd = mysocket;
#endif
		if (select(FD_SETSIZE, &fdRead, NULL, NULL, &tv) <= 0)
			continue;
		//ACCEPT CONNECT
		if (FD_ISSET(mysocket, &fdRead)) {
			struct sockaddr addrclt;
			int bAccept = 0;
#ifdef _WIN32
			int  addlen = sizeof(addrclt);
			SOCKET aptclt = accept(mysocket, (struct sockaddr*)&addrclt, &addlen);
#else
			socklen_t addlen = sizeof(addrclt);
			int aptclt = accept(mysocket, (struct sockaddr*)&addrclt, &addlen);
#endif	
			for (int nLoopi = 0; aptclt > 0 && nLoopi < MAX_CLIENT; nLoopi++) {
				if (fdarray[nLoopi] == 0) {
					fdarray[nLoopi] = aptclt;
					unsigned long nMode = 1;
					ioctlsocket(aptclt, FIONBIO, &nMode);
					FD_SET(fdarray[nLoopi], &fdRead);
					bAccept = 1;
					break;
				}
			}
			if (!bAccept) {
				closesocket(aptclt);
			}
			continue;
		}
		//RECV
		for (int nLoopi = 0; nLoopi < MAX_CLIENT; nLoopi++) {
			if (FD_ISSET(fdarray[nLoopi], &fdRead)) {
				static char data[MAX_BUF_SIZE] = { 0 };
				memset(data, 0x00, MAX_BUF_SIZE);
				int cnt = 0;
				int tln = 0;
				do {
					cnt = recv(fdarray[nLoopi], &data[tln], MAX_BUF_SIZE - tln, 0);
					tln += cnt;
				} while (cnt > 0);
				if (tln < 0 || http_request_handle(fdarray[nLoopi], data, tln) == 0) {
					printf("close\n");
					closesocket(fdarray[nLoopi]);
				}
				FD_CLR(fdarray[nLoopi], &fdRead);
				fdarray[nLoopi] = 0;
			}
		}
	}
	return 0;
}

int  tcps_start(const char* ip, short port, int(*function)(char*, char*, unsigned, char*)) {
	printf("start web service,ip:%s port:%d\n", ip, port);
#ifdef _WIN32
	WSADATA wsdata;
	WSAStartup(0x0202, &wsdata);
#endif
	//sock
	exit_mark = 0;
	business_dispath = function;
	struct sockaddr_in myaddr;
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = inet_addr(ip);
	myaddr.sin_port = htons(port);
	mysocket = socket(AF_INET, SOCK_STREAM, 0);
	memset(fdarray, 0, sizeof(fdarray));
	/* Set port reuse */
	int opt = 1;
#ifdef _WIN32
	if (setsockopt(mysocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt)) < 0)
		goto err;
#else
	if (setsockopt(mysocket, SOL_SOCKET, SO_REUSEADDR, (void*)&opt, sizeof(opt)) < 0)
		goto err;
#endif
	//bind
	int ibind = bind(mysocket, (struct sockaddr*)&myaddr, sizeof(struct sockaddr));
	if (ibind != 0) {
		printf("websvr bind socket error! addr=%s:%d\n", ip, port);
		goto err;
	}
	//listen
	int ilisten = listen(mysocket, MAX_CLIENT);
	if (ilisten != 0) {
		printf("websvr listen error!\n");
		goto err;
	}
#ifdef _WIN32
	unsigned long nMode = 1;
	ioctlsocket(mysocket, FIONBIO, &nMode);
	threadid = _beginthreadex(NULL, 0, tcps_workthread, 0, 0, NULL);
#else
	fcntl(mysocket, F_SETFL, O_NONBLOCK);
	if (pthread_create(&threadid, NULL, tcps_workthread, 0) != 0)
		printf("pthread_create failed! \n");
#endif
	return 1;
err:
	if (mysocket) {
		closesocket(mysocket);
		WSACleanup();
	}
	return 0;
}

//tcp server stop
int tcps_stop() {
	exit_mark = 1;
	closesocket(mysocket);
	for (unsigned int i = 0; i < MAX_CLIENT; i++) {
		if (fdarray[i])closesocket(fdarray[i]);
	}
	WSACleanup();
	return 0;
}