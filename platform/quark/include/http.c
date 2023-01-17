#include <stdio.h>
#include <assert.h>
#include <winsock.h>
#include "http.h"
#include <time.h>
#include "map.h"
//#include <string.h>
#define BUFFER_SIZE 1024*8

static int last_time_stamp = 0;
void print_time_stamp( const char* mark ) {
	int now = GetTickCount(); // time(NULL);
	fprintf(stderr, "%s def:(%d) now:%d\n", mark, now - last_time_stamp, now);
	last_time_stamp = now;
}

//TCP_SENDS
int tcp_sends(int sock, const void* buf, int size) {
	int send_len = 0;
	int sent_len = 0;
	for (sent_len = 0, send_len = 0; send_len < size; send_len += sent_len) {
		sent_len = send(sock, (char*)buf + send_len, size - send_len, 0);
		if (sent_len <= 0)
			break;
	}
	return sent_len;
}

int tcp_close(int sock) {
#ifdef WIN32
	return closesocket(sock);
#else
	return close(sock);
#endif
}

//HTTP GET REQUES
int http_get_request(const char* uri, const char * data, char ** payload) {
	char nuri[256] = { 0 };
	strcpy(nuri, uri);
	char * host_name = NULL;
	unsigned short port = 80;
	char * path = NULL;
	if (strncmp(uri, "http", 4) == 0) {
		host_name = nuri + 4 + 3;
		char * p_slash = strchr(host_name, '/');
		if (p_slash) {
			path = p_slash + 1;
			*p_slash = '\0';
		}
		char * p_colon = strchr(host_name, ':');
		if (p_colon) {
			port = atoi(p_colon + 1);
			*p_colon = '\0';
		}
	}
	//HTTP 连接
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in sin = { 0 };
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = inet_addr(host_name);
	if (0 != connect(sockfd, (struct sockaddr*)&sin, sizeof(struct sockaddr_in))) {
		goto err;
	}
#ifdef _WIN32
	unsigned long nMode = 1;
	ioctlsocket(sockfd, FIONBIO, &nMode);
#else
	fcntl(sockfd, F_SETFL, O_NONBLOCK);
#endif
	//发送请求数据
	char post_header[2048] = { 0 };
	int nsend = sprintf(post_header, "GET /%s?%s HTTP/1.1\r\nHost: %s:%d\r\nConnection: close\r\nContent-Length: 0\r\n\r\n", path, data, host_name, port);
	if (tcp_sends(sockfd, post_header, nsend) != nsend) {
		goto err;
	}
	//select 模式接收数据
	int recv_status = 1;
	char header[1024] = { 0 };
	memset(header, 0x00, 1024);
	unsigned header_len = 0;
	unsigned header_recving = 1;
	unsigned payload_len = 0;
	fd_set fdread;
	struct timeval tv;
	tv.tv_sec = 5;
	tv.tv_usec = 0;

	while (recv_status) {
		FD_ZERO(&fdread);
		FD_SET(sockfd, &fdread);
		if (!select(sockfd, &fdread, NULL, NULL, &tv)) {
			break;
		}
		while (FD_ISSET(sockfd, &fdread)) {
			//FD_CLR(sockfd, &fdread);
			int len = 0;
			if (header_recving) {
				if ((len = recv(sockfd, &header[header_len], 1, 0)) <= 0) {
					recv_status = 0;
					break;
				}
				header_len += len;
				if (header[header_len - 4] == 0x0D && header[header_len - 3] == 0x0A &&
					header[header_len - 2] == 0x0D && header[header_len - 1] == 0x0A) {
					header_recving = 0;
				}
			}
			else {
				char data[BUFFER_SIZE] = { 0 };
				if ((len = recv(sockfd, data, BUFFER_SIZE, 0)) <= 0) {
					recv_status = 0;
					break;
				}
				*payload = realloc(*payload, payload_len + len + 1);
				memset(*payload + payload_len, 0x00, len + 1);
				memcpy(*payload + payload_len, data, len);
				payload_len += len;
			}
		}
	}
	tcp_close(sockfd);
	return payload_len;
err:
	if (sockfd) {
		tcp_close(sockfd);
		sockfd = 0;
	}
	return -1;
}

//HTTP POST REQUEST
int http_post_request(const char* uri, const char * data, int data_len, char ** payload) {
	print_time_stamp("http_post_request");
	char nuri[256] = { 0 };
	strcpy(nuri, uri);
	char * host_name = NULL;
	unsigned short port = 80;
	char * path = NULL;
	if (strncmp(uri, "http", 4) == 0) {
		host_name = nuri + 4 + 3;
		char * p_slash = strchr(host_name, '/');
		if (p_slash) {
			path = p_slash + 1;
			*p_slash = '\0';
		}
		char * p_colon = strchr(host_name, ':');
		if (p_colon) {
			port = atoi(p_colon + 1);
			*p_colon = '\0';
		}
	}

	//HTTP 连接
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	int opt = 1;
#ifdef _WIN32
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt)) < 0)
		goto err;
#else
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (void*)&opt, sizeof(opt)) < 0)
		goto err;
#endif

	struct sockaddr_in sin = { 0 };
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = inet_addr(host_name);
	print_time_stamp("connect start");
	if (-1 == connect(sockfd, (struct sockaddr*)&sin, sizeof(struct sockaddr_in))) {
		goto err;
	}

	//发送请求数据
	char post_header[1024] = { 0 };
	int nsend = sprintf(post_header, "POST /%s HTTP/1.1\r\nHost: %s:%d\r\nConnection: close\r\nContent-Length: %ld\r\n\r\n", path, host_name, port, data_len);

	print_time_stamp("send start");
	if (tcp_sends(sockfd, post_header, nsend) != nsend || tcp_sends(sockfd, data, data_len) != data_len) {
		goto err;
	}
	//select 模式接收数据
	int recv_status = 1;
	char header[1024] = { 0 };
	memset(header, 0x00, 1024);
	unsigned header_len = 0;
	unsigned header_recving = 1;
	unsigned payload_len = 0;
	fd_set fdread;
	struct timeval tv;
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	while (recv_status) {
		FD_ZERO(&fdread);
		FD_SET(sockfd, &fdread);
		if (!select(sockfd, &fdread, NULL, NULL, &tv)) {
			break;
		}
		while (FD_ISSET(sockfd, &fdread)) {
			int len = 0;
			if (header_recving) {
				if ((len = recv(sockfd, &header[header_len], 1, 0)) <= 0) {
					recv_status = 0;
					break;
				}
				header_len += len;
				if (header[header_len - 4] == 0x0D && header[header_len - 3] == 0x0A &&
					header[header_len - 2] == 0x0D && header[header_len - 1] == 0x0A) {
					header_recving = 0;
				}
			}
			else {
				char data[BUFFER_SIZE] = { 0 };
				if ((len = recv(sockfd, data, BUFFER_SIZE, 0)) <= 0) {
					recv_status = 0;
					break;
				}
				*payload = realloc(*payload, payload_len + len + 1);
				memset(*payload + payload_len, 0x00, len + 1);
				memcpy(*payload + payload_len, data, len);
				payload_len += len;
			}
		}
		FD_CLR(sockfd, &fdread);
		recv_status = 0;
	}
	tcp_close(sockfd);
	return payload_len;
err:
	if (sockfd) {
		tcp_close(sockfd);
		sockfd = 0;
	}
	return -1;
}

//HTTP POST REQUEST
int tcp_post_request(const char* uri, const char * data, int data_len, char ** payload) {
	char host_desc[64];
	char disp_name[26];
	if (2 != sscanf(uri, "tcp://%[^/]/%[^ ]*", host_desc, disp_name)) {
		return -1;
	}

	static int create_map = 1;
	static map_t(int*) tcp_clients;
	if (create_map) {
		map_init(&tcp_clients);
		create_map = 0;
	}
	
	int sockfd = 0;
	int * fd = map_get(&tcp_clients, host_desc);
	if ( !fd ) {
		char ip[15] = { 0 };
		unsigned int port = 0;
		sscanf(host_desc, "%[^:]:%d", ip, &port);
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		struct sockaddr_in sin = { 0 };
		sin.sin_family = AF_INET;
		sin.sin_port = htons((unsigned short)port);
		sin.sin_addr.s_addr = inet_addr(ip);
		if (-1 == connect(sockfd, (struct sockaddr*)&sin, sizeof(struct sockaddr_in))) {
			tcp_close(sockfd);
			return -1;
		}
		map_set(&tcp_clients, host_desc, sockfd );
	}
	else {
		sockfd = *fd;
	}

	//发送请求头数据
	char post_header[32] = { 0 };
	post_header[0] = 0x68;
	memcpy( &post_header[1], &data_len, 4 );
	memcpy( &post_header[6], disp_name, 26 );
	post_header[31] = 0x69;
	if (tcp_sends(sockfd, post_header, 32) != 32 || tcp_sends(sockfd, data, data_len) != data_len) {
		goto err;
	}

	//int a = tcp_sends(sockfd, post_header, 32);
	//int b = tcp_sends(sockfd, data, data_len);

	//select 模式接收数据
	int recv_status = 1;
	char header[32] = { 0 };
	unsigned header_len = 0;
	unsigned header_recving = 1;
	unsigned payload_len = 0;
 
	fd_set fdread;
	struct timeval tv = { 5,0 };
	while ( recv_status ) {
		FD_ZERO(&fdread);
		FD_SET(sockfd, &fdread);
		if (!select(sockfd, &fdread, NULL, NULL, &tv)) {
			break;
		}
		while (FD_ISSET(sockfd, &fdread)) {
			int len = 0;
			if (header_recving) {
				if ((len = recv(sockfd, header, 32, 0)) <= 0) {
					recv_status = 0;
					break;
				}
				if (header[0] == 0x68 && header[31] == 0x69 ){
					memcpy(&payload_len, &header[1], sizeof(int));
					header_recving = 0;
				}
			}
			else {
				char data[BUFFER_SIZE] = { 0 };
				if ((len = recv(sockfd, data, payload_len, 0)) <= 0) {
					recv_status = 0;
					break;
				}
				*payload = realloc(*payload, payload_len + len + 1);
				memset(*payload + payload_len, 0x00, len + 1);
				memcpy(*payload + payload_len, data, len);
				payload_len += len;
				recv_status = 0;
				break;
			}
		}
		FD_CLR(sockfd, &fdread);
		recv_status = 0;
	}
	return payload_len;
err:
	if (sockfd) {
		tcp_close(sockfd);
		map_remove( &tcp_clients, host_desc);
		sockfd = 0;
	}
	return -1;
}

 
