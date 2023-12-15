#include <stdio.h>
#include "ssls.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/ssl.h>
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

struct ssls_t {
	int(*dispath)(int, SSL_CTX *, char*,unsigned int);
	int exit_mark;
	int fdarray[MAX_CLIENT];
	int sslarray[MAX_CLIENT];
#ifdef _WIN32
	unsigned threadid;
	unsigned int mysocket;
#else
	pthread_t threadid;
	int mysocket;
#endif
	SSL_CTX * sslcontext;
};

int ssls_close(int sock) {
	if ( sock ) {
#ifdef _WIN32
		closesocket(sock);
#else
		close(sock);
#endif
	}
	return 0;
}

ssls_t *  ssls_start(const char* ip, unsigned short port, int(*function)(int, char*,unsigned), const char*ca, const char *sa, const char* pk) {
	fprintf(stderr, "start web service,ip:%s port:%d\n", ip, port);
#ifdef _WIN32
	WSADATA wsdata;
	WSAStartup(0x0202, &wsdata);
#endif
	ssls_t *ssls = NULL;
	if ((ssls = (ssls_t *)malloc(sizeof(ssls_t))) == NULL) {
		goto err;
	}
	//sock
	ssls->exit_mark = 0;
	ssls->dispath = function;
	ssls->sslcontext = NULL;
	struct sockaddr_in myaddr;
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = inet_addr(ip);
	myaddr.sin_port = htons(port);
	ssls->mysocket = socket(AF_INET, SOCK_STREAM, 0);
	memset(ssls->fdarray, 0, sizeof(ssls->fdarray));
	memset(ssls->sslarray, 0, sizeof(ssls->sslarray));

	//SSL
	SSL_library_init();
	ssls->sslcontext = SSL_CTX_new(SSLv23_server_method());
	assert(ssls->sslcontext);
	// 不校验客户端证书
	SSL_CTX_set_verify(ssls->sslcontext, SSL_VERIFY_NONE, NULL);
	// 加载CA的证书  
	if (!SSL_CTX_load_verify_locations(ssls->sslcontext, ca, NULL)) {
		printf("SSL_CTX_load_verify_locations error!\n");
		goto err;
	}
	// 加载自己的证书  
	if (SSL_CTX_use_certificate_file(ssls->sslcontext, sa, SSL_FILETYPE_PEM) <= 0) {
		printf("SSL_CTX_use_certificate_file error!\n");
		goto err;
	}
	// 加载私钥
	if (SSL_CTX_use_PrivateKey_file(ssls->sslcontext, pk, SSL_FILETYPE_PEM) <= 0) {
		printf("SSL_CTX_use_PrivateKey_file error!\n");
		goto err;
	}
	// 判定私钥是否正确  
	if (!SSL_CTX_check_private_key(ssls->sslcontext)) {
		printf("SSL_CTX_check_private_key error!\n");
		goto err;
	}
	printf("CA证书、本地证书、私钥均已加载完毕\n");
	printf("server is listening...\n");
	
	/* Set port reuse */
	int opt = 1;
#ifdef _WIN32
	if (setsockopt(ssls->mysocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt)) < 0)
		goto err;
#else
	if (setsockopt(ssls->mysocket, SOL_SOCKET, SO_REUSEADDR, (void*)&opt, sizeof(opt)) < 0)
		goto err;
#endif
	//bind
	int ibind = bind(ssls->mysocket, (struct sockaddr*)&myaddr, sizeof(struct sockaddr));
	if (ibind != 0) {
		fprintf(stderr, "websvr bind socket error! addr=%s:%d\n", ip, port);
		goto err;
	}
	//listen
	int ilisten = listen(ssls->mysocket, MAX_CLIENT);
	if (ilisten != 0) {
		fprintf(stderr, "websvr listen error!\n");
		goto err;
	}

#ifdef _WIN32
	unsigned long nMode = 1;
	ioctlsocket(ssls->mysocket, FIONBIO, &nMode);
	ssls->threadid = _beginthreadex(NULL, 0, ssls_workthread, ssls, 0, NULL);
#else
	fcntl(ssls->mysocket, F_SETFL, O_NONBLOCK);
	pthread_create(&ssls->threadid, NULL, tsls_workthread, ssls);
	//printf(stderr, "pthread_create failed! \n");
#endif
	return ssls;
err:
	if (ssls) {
		ssls_close(ssls->mysocket);
		free(ssls);
	}
	return NULL;
}

//tcp server stop
void ssls_stop( ssls_t * s ) {
	s->exit_mark = 1;
	for (unsigned int i = 0; i < MAX_CLIENT; i++) {
		if (s->fdarray[i])ssls_close(s->fdarray[i]);
		if (s->sslarray[i]) {
			SSL_shutdown(s->sslarray[i]);
			SSL_free(s->sslarray[i]);
		}
	}
	ssls_close(s->mysocket);
	SSL_CTX_free(s->sslcontext);
#ifdef _WIN32
	WSACleanup();
#endif
}

#ifdef _WIN32
unsigned __stdcall ssls_workthread(void* param) {
#else
void* ssls_workthread(void* param) {
#endif
	ssls_t * svr = (ssls_t*)param;
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
					SSL *aptssl = SSL_new(svr->sslcontext);
					SSL_set_fd(aptssl, aptclt);
					int code;
					int retryTimes = 0;
					//unsigned long n1Mode = 1;
					//ioctlsocket(aptclt, FIONBIO, &n1Mode);	
					// 防止客户端连接了但不进行ssl握手, 单纯的增大循环次数无法解决问题，本地大概循环4000次，chrome连接会循环20000多次
					while ((code = SSL_accept(aptssl)) <= 0 && retryTimes++ < 100) {
						if (SSL_get_error(aptssl, code) != SSL_ERROR_WANT_READ) {
							printf("ssls accept new socket:%d but ssl is Error.\n", aptclt);
							break;
						}
						Sleep(1);
					}
					if (code != 1) {
						ssls_close(aptclt);
						SSL_shutdown(aptssl);
						SSL_free(aptssl);
						break;
					}
					printf("ssls accept new client ssl:%d socket:%d\n", aptssl, aptclt);
					svr->sslarray[nLoopi] = aptssl;
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
				ssls_close(aptclt);
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
					cnt = SSL_read(svr->sslarray[nLoopi], &data[tln], MAX_BUF_SIZE - tln, 0);
					tln += cnt;
				} while (cnt > 0);
				if (tln <= 0 || svr->dispath(svr, svr->sslarray[nLoopi], data,tln)==-1) {
					ssls_close(svr->fdarray[nLoopi]);
				}
				FD_CLR(svr->fdarray[nLoopi], &fdRead);
				ssls_close(svr->fdarray[nLoopi]);
				SSL_shutdown(svr->sslarray[nLoopi]);
				SSL_free(svr->sslarray[nLoopi]);
				svr->fdarray[nLoopi] = 0;
				svr->sslarray[nLoopi] = 0;
			}
		}
	}
	return 0;
}

//send the buffer
int ssls_sends(int cs, const void* buf, int size) {
	printf("ssls sends  ssl:%d datalen:%d\n", cs, size);
	int send_len = 0;
	int sent_len = 0;
	for (sent_len = 0, send_len = 0; send_len < size; send_len += sent_len) {
		sent_len = SSL_write(cs, (char*)buf + send_len, size - send_len);
		if (sent_len <= 0)
			break;
	}
	return sent_len;
}

//send the buffer
int ssls_shutdown(ssls_t * s, int  cs) {
	for (unsigned int i = 0; i < MAX_CLIENT && cs > 0 ; i++) {
		if (s->sslarray[i] == cs) {
			if (s->fdarray[i])ssls_close(s->fdarray[i]);
			if (s->sslarray[i]) {
				SSL_shutdown(s->sslarray[i]);
				SSL_free(s->sslarray[i]);
			}
			s->fdarray[i] = 0;
			s->sslarray[i] = 0;
			break;
		}
	}
	return 0;
}
