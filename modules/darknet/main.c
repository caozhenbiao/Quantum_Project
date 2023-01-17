#include <stdio.h>
#include <assert.h>
#ifdef WIN32
#include <unistd.h>
#include <winsock.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include  <sys/unistd.h>
#include <sys/ioctl.h>
#include <sys/fcntl.h>
#endif

#include "./src/network.h"
#include "./src/utils.h"
#include "./src/network.h"
#include "./src/region_layer.h"
#include "./src/cost_layer.h"
#include "./src/utils.h"
#include "./src/parser.h"
#include "./src/box.h"
#include "./src/demo.h"
#include "./src/option_list.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "stb_image.h"

//EATH NETWORK
#define MAX_CLIENT 16
#define MAX_BUF_SIZE  204800
int exit_mark;
int(*business_dispath)(char*, char*, unsigned,char**);
int fdarray[MAX_CLIENT];
unsigned threadid;
int mysocket;

//YOLO NETWORK
network mynet;
char**    mynames;

static char * response_header_style = "HTTP/1.1 200 OK \r\n \
Date: %s GMT\r\n \
Expires: %s GMT\r\n \
Cache-Control: private, max-age=31536000\r\n \
X-Content-Type-Options: nosniff\r\n \
Server: smartx\r\n \
Content-Type: %s\r\n \
Content - Length: %ld\r\n\r\n";

//http GMTIME
static char* get_gmtime(char* szBuf, int delay) {
	time_t tnow = time(NULL) + delay;
	strftime(szBuf, 127, "%a,%d %b %Y %H:%M:%S", gmtime(&tnow));
	return szBuf;
}

//YOLO SETUP
static int yolo_setup(const char* nf, const char* cf, const char* wf){
   printf("darknet setup names:%s   cfg:%s  weight:%s\n", nf, cf, wf);
   int names_size = 0;
   mynames = get_labels_custom((char*)nf, &names_size );
  mynet = parse_network_cfg_custom((char*)cf, 1, 1); // set batch=1
   load_weights(&mynet, (char*)wf);
   //set_batch_network(mynet, 1);
   fuse_conv_batchnorm( mynet );
   calculate_binary_weights( mynet );
   if (mynet.layers[mynet.n - 1].classes != names_size) {
		printf("\n Error: in the file %s number of names %d that isn't equal to classes=%d in the file %s \n",nf, names_size, mynet.layers[mynet.n - 1].classes, cf);
	}
   srand(2222222);
   printf("steup finish!\n");
   return 0;
}

//YOLO FRAM DETECT
static int framedetect(char*jpgdata, __int64_t jpgsize, char** boxlist){
	float thresh = 0.24;
	float hier_thresh = 0.4;
	float nms=.45;
	int c = 3;
	int w,h,comp;
	unsigned char * data  = stbi_load_from_memory((unsigned char*)jpgdata,jpgsize,&w,&h,&comp,3);
	if( !data ){
		printf("error image data.\n");
		return 0;
	}
	image im = make_image(w, h, c);
	int ia,ja,k;
	for(k = 0; k < c; ++k){
		for(ja = 0; ja < h; ++ja){
			for(ia = 0; ia < w; ++ia){
				im.data[ia+w*ja+w*h*k] = (float)data[c*ia+c*w*ja]/255.;
			}
		}
	}
	free(data);
	//
	image sized = letterbox_image(im, mynet.w, mynet.h);
	layer l = mynet.layers[mynet.n - 1];
	float* out = network_predict(mynet, sized.data);
	int nboxes = 0;
	detection *dets = get_network_boxes(&mynet, im.w, im.h, thresh, hier_thresh, 0, 1, &nboxes, 1);
	if (nms) {
		if (l.nms_kind == DEFAULT_NMS) do_nms_sort(dets, nboxes, l.classes, nms);
		else diounms_sort(dets, nboxes, l.classes, nms, l.nms_kind, l.beta_nms);
	}
	//
	memset( *boxlist, 0x00,1204);
	strcat(*boxlist, "[");
	int ndetcnt = 0;
	for (int i = 0; i < nboxes; ++i) {
		for (int j = 0; j < l.classes; ++j) {
			if (dets[i].prob[j] > thresh) {
				box b = dets[i].bbox;
				if (b.x < 0) b.x = 0.0;
				if (b.y < 0) b.y = 0.0;
				if( strlen(*boxlist) > 1 ){
					strcat(*boxlist, ",");
				}
				char szText[256] = { 0 };
				int n = sprintf(szText, "{\"n\":\"%s\",\"s\":%f ,\"x\":%f,\"y\":%f,\"w\":%f,\"h\":%f}", mynames[j], dets[i].prob[j] * 100, b.x, b.y, b.w, b.h);
				strcat(*boxlist, szText);
			}
		}
	}
	strcat(*boxlist, "]");
	//
	if( ndetcnt>0 ){
		free_detections(dets, nboxes);
	}
    free_image(im);
	free_image(sized);
	return 1;
}

// TCP SERVER DISPATH INTERFACE
int dispath(char* uri, char* data, unsigned data_len, char** data_out) {
	printf("dispath  uri:%s   data:%s len:%d\n", uri, data, data_len);
	memset(*data_out, 0x00, 1024);
	framedetect(data, data_len, data_out);
	return strlen(*data_out);
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

//http ������
int http_request_handle(int cs,  char* uri, char* data, unsigned len) {
	char* out_data = (char*)malloc(sizeof(char*) * 1024);
	int out_data_len = business_dispath(uri, data, len, &out_data);
	char response_header[1024];
	memset(response_header, 0x00, sizeof(response_header));
	char inspire[128] = { 0 };
	char expires[128] = { 0 };
	//get_gmtime(inspire, 0);
	//get_gmtime(expires, 3600);
	int hdr_len = sprintf(response_header, response_header_style, inspire, expires, "LUA", len);
	tcps_sends(cs, response_header, hdr_len);
	tcps_sends(cs, out_data, out_data_len);
	printf("http_request_handle:%s\n", out_data );
	free(out_data);
	return 0;
}

//HTTP WORK THREAD
unsigned  http_workthread(void* param) { 
	char global_data[MAX_BUF_SIZE] = { 0 };
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
		int maxfd = mysocket;
		if (select(FD_SETSIZE, &fdRead, NULL, NULL, &tv) <= 0)
			continue;

		//ACCEPT CONNECT
		if (FD_ISSET(mysocket, &fdRead)) {
			struct sockaddr addrclt;
			int bAccept = 0;
			int  addlen = sizeof(addrclt);
			int aptclt = accept(mysocket, (struct sockaddr*)&addrclt, &addlen);

			for (int nLoopi = 0; aptclt > 0 && nLoopi < MAX_CLIENT; nLoopi++) {
				if (fdarray[nLoopi] == 0) {
					fdarray[nLoopi] = aptclt;
					#ifdef WIN32
					unsigned long nMode = 1;
					ioctlsocket(aptclt, FIONBIO, &nMode);
					#else
					fcntl(mysocket,F_SETFL,O_NONBLOCK);
					#endif
					FD_SET(fdarray[nLoopi], &fdRead);
					bAccept = 1;
					break;
				}
			}
			if (!bAccept) {
				close(aptclt);
			}
			//continue;
		}
		//RECV
		for (int nLoopi = 0; nLoopi < MAX_CLIENT; nLoopi++) {
			memset(global_data, 0x00, MAX_BUF_SIZE);
			char header[2048] = {0};
			int header_len = 0;
			int  header_recving = 1;
			int data_len = 0;
			int recv_status = 0;
			int  data_recv_len = 0;

			char uri[256] = { 0 };
			char args[256] = { 0 };
			char methord[32] = { 0 };

			while(FD_ISSET(fdarray[nLoopi], &fdRead)) {
				if( header_recving){
					int cnt = recv(fdarray[nLoopi], &header[header_len],1, 0);
					if( cnt <=0 ){
						recv_status = 0;
						break;
					} 
					header_len += cnt;
					if(  header[header_len-4] == 0x0D && header[header_len-3]==0x0A && header[header_len-2]==0x0D && header[header_len-1]==0x0A){
						printf("start recv  image data.\n");
						header_recving = 0;
						int i =0;
						for(  i = 0; i< header_len; i++ ){
							if( 15  == strspn(header+i,"Content-Length:") ){
								sscanf(global_data, "%[^ ] %[^( |?)]?%[^ ]*", methord, uri, args);
								break;
							}
						}
						data_len = atoi( &header[i+16] );
						header_recving = 0;
					}
				}
				if(  header_recving  ==  0  ){ 
					while( 1 ){
						int cnt = recv(fdarray[nLoopi], &global_data[data_recv_len], 1024, 0);
						data_recv_len += cnt;
						if(  data_recv_len == data_len || cnt <= 0 ){
							break;
						}
					}
					printf("recv image data finish.\n");
					if ( http_request_handle(fdarray[nLoopi], uri, global_data, data_recv_len) == 0) {
						close(fdarray[nLoopi]);
					}
					FD_CLR(fdarray[nLoopi], &fdRead);
					fdarray[nLoopi] = 0;
				}
			}
		}
	}
	return 0;
}

int  tcps_start(const char* ip, short port, int(*function)(char*, char*, unsigned, char**)) {
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
	if (pthread_create(&threadid, NULL, http_workthread, 0) != 0)
		printf("pthread_create failed! \n");
#endif
	return 1;
err:
	if (mysocket) {
		close(mysocket);
		#ifdef _WIN32
		WSACleanup();
		#endif
	}
	return 0;
}

//tcp server stop
int tcps_stop() {
	exit_mark = 1;
	close(mysocket);
	for (unsigned int i = 0; i < MAX_CLIENT; i++) {
		if (fdarray[i])close(fdarray[i]);
	}
	#ifdef _WIN32
	WSACleanup();
	#endif
	return 0;
}

#undef main
int main(int argc, char* argv[]) {
	char nf[256] = { 0 };
	char cf[256] = { 0 };
	char wf[256] = { 0 };
	unsigned short server_port = 9000;
	int c = 0;
	while ((c = getopt(argc, argv, "n:c:w:p")) != -1) {
		switch (c) {
		case 'n': strncpy(nf, optarg, 256); break;
		case 'c': strncpy(cf, optarg, 256); break;
		case 'w': strncpy(wf, optarg, 256); break;
		case 'p': server_port = atoi(optarg); break;
		default:break;
		}
	}
	yolo_setup( nf, cf, wf );
	tcps_start("0.0.0.0", server_port, dispath);
	while (1) {
		static char sz[256] = { 0 };
		if (fgets(sz, 256, stdin) == NULL)
			continue;
		if (strcmp(sz, "exit\n") == 0)
			break;
	}
	return 0;
}

