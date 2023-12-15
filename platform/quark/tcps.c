#include <stdio.h>
#include "tcps.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#ifdef _WIN32
#include <winsock.h>
#include <process.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <arpa/inet.h>
#include <sys/unistd.h>
#include <sys/ioctl.h>
#include <sys/fcntl.h>
#endif

#define MAX_BUF_SIZE 1024*512*2
#define MAX_CLIENT 16

struct tcps_t {
	int(*dispath)(int,int, char*,unsigned int);
	int exit_mark;
	int fdarray[MAX_CLIENT];
#ifdef _WIN32
	unsigned threadid;
	unsigned int mysocket;
#else
	pthread_t threadid;
	int mysocket;
#endif
};

int tcps_close(int sock) {
	if ( sock ) {
#ifdef _WIN32
		closesocket(sock);
#else
		close(sock);
#endif
	}
	return 0;
}

tcps_t *  tcps_start(const char* ip, unsigned short port, int(*function)(int, char*,unsigned)) {
	fprintf(stderr, "start web service,ip:%s port:%d\n", ip, port);
#ifdef _WIN32
	WSADATA wsdata;
	WSAStartup(0x0202, &wsdata);
#endif
	tcps_t *tcps = NULL;
	if ((tcps = (tcps_t *)malloc(sizeof(tcps_t))) == NULL) {
		goto err;
	}
	//sock
	tcps->exit_mark = 0;
	tcps->dispath = function;
	struct sockaddr_in myaddr;
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = inet_addr(ip);
	myaddr.sin_port = htons(port);
	tcps->mysocket = socket(AF_INET, SOCK_STREAM, 0);
	memset(tcps->fdarray, 0, sizeof(tcps->fdarray));
	/* Set port reuse */
	int opt = 1;
#ifdef _WIN32
	if (setsockopt(tcps->mysocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt)) < 0)
		goto err;
#else
	if (setsockopt(tcps->mysocket, SOL_SOCKET, SO_REUSEADDR, (void*)&opt, sizeof(opt)) < 0)
		goto err;
#endif
	//bind
	int ibind = bind(tcps->mysocket, (struct sockaddr*)&myaddr, sizeof(struct sockaddr));
	if (ibind != 0) {
		fprintf(stderr, "websvr bind socket error! addr=%s:%d\n", ip, port);
		goto err;
	}
	//listen
	int ilisten = listen(tcps->mysocket, MAX_CLIENT);
	if (ilisten != 0) {
		fprintf(stderr, "websvr listen error!\n");
		goto err;
	}
#ifdef _WIN32
	unsigned long nMode = 1;
	ioctlsocket(tcps->mysocket, FIONBIO, &nMode);
	tcps->threadid = _beginthreadex(NULL, 0, tcps_workthread, tcps, 0, NULL);
#else
	fcntl(tcps->mysocket, F_SETFL, O_NONBLOCK);
	pthread_create(&tcps->threadid, NULL, tcps_workthread, tcps);
	//printf(stderr, "pthread_create failed! \n");
#endif
	return tcps;
err:
	if (tcps) {
		tcps_close(tcps->mysocket);
		free(tcps);
	}
	return NULL;
}

//tcp server stop
void tcps_stop( tcps_t * s ) {
	s->exit_mark = 1;
	tcps_close(s->mysocket);
	for (unsigned int i = 0; i < MAX_CLIENT; i++) {
		if (s->fdarray[i])tcps_close(s->fdarray[i]);
	}
}


#ifdef _WIN32
unsigned __stdcall tcps_workthread(void* param) {
#else
void* tcps_workthread(void* param) {
#endif
	tcps_t * svr = (tcps_t*)param;
	while (!svr->exit_mark) {
		fd_set fdRead;
		FD_ZERO(&fdRead);
		FD_SET(svr->mysocket, &fdRead);
		for (int nLoopi = 0; nLoopi < MAX_CLIENT; nLoopi++) {
			if (svr->fdarray[nLoopi] != 0)
				FD_SET(svr->fdarray[nLoopi], &fdRead);
		}
		//select
		struct timeval tv = { 0,10000 };
		//int maxfd = svr->mysocket;
		if (select(FD_SETSIZE, &fdRead, NULL, NULL, &tv) <= 0)
			continue;
		//ACCEPT CONNECT
		if (FD_ISSET(svr->mysocket, &fdRead)) {
			struct sockaddr addrclt;
			int bAccept = 0;
			int addlen = sizeof(addrclt);
			int aptclt = accept(svr->mysocket, (struct sockaddr*)&addrclt, &addlen);
			for (int nLoopi = 0; aptclt > 0 && nLoopi < MAX_CLIENT; nLoopi++) {
				if (svr->fdarray[nLoopi] == 0) {
					svr->fdarray[nLoopi] = aptclt;
#ifdef _WIN32
					unsigned long nMode = 1;
					ioctlsocket(aptclt, FIONBIO, &nMode);
#else
					fcntl(aptclt, F_SETFL, O_NONBLOCK);
#endif
					FD_SET(svr->fdarray[nLoopi], &fdRead);
					bAccept = 1;
					break;
				}
			}
			if (!bAccept) {
				tcps_close(aptclt);
			}
			continue;
		}
		//RECV
		for (int nLoopi = 0; nLoopi < MAX_CLIENT; nLoopi++) {
			if (FD_ISSET(svr->fdarray[nLoopi], &fdRead)) {
				static char data[MAX_BUF_SIZE] = { 0 };
				memset(data, 0x00, MAX_BUF_SIZE);
				int cnt = 0;
				int tln = 0;
				do {
					cnt = recv(svr->fdarray[nLoopi], &data[tln], MAX_BUF_SIZE - tln, 0);
					tln += cnt;
				} while (cnt > 0);
				if (tln <= 0 || svr->dispath(svr, svr->fdarray[nLoopi],data,tln)==-1) {
					tcps_close(svr->fdarray[nLoopi]);
				}
				FD_CLR(svr->fdarray[nLoopi], &fdRead);
				tcps_close(svr->fdarray[nLoopi]);
				svr->fdarray[nLoopi] = 0;
			}
		}
	}
	return 0;
}

//send the buffer
int tcps_sends(int sock, const void* buf, int size) {
	int send_len = 0;
	int sent_len = 0;
	for (sent_len = 0, send_len = 0; send_len < size; send_len += sent_len) {
		sent_len = send(sock, (char*)buf + send_len, size - send_len, 0);
		if (sent_len <= 0)
			break;
	}
	return sent_len;
}

//tcps_shutdown
int tcps_shutdown(tcps_t * s, int  cs) {
	for (unsigned int i = 0; i < MAX_CLIENT && cs > 0; i++) {
		if (s->fdarray[i] == cs) {
			if (s->fdarray[i])tcps_close(s->fdarray[i]);
			s->fdarray[i] = 0;
			break;
		}
	}
	return 0;
}