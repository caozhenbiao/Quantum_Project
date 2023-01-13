#include <stdio.h>
#include "sha1.h"
#include "tcps.h"
#include "websock.h"

extern int responsetolua( unsigned int act, char* data, int len );
#define MAX_CLIENT 256
static tcps_t * tcps = NULL;

//base64_decode
static int base64_decode(char *indata, unsigned inlen, char *outdata, unsigned *outlen){
	static const unsigned char base64_suffix_map[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	62,        // '+'
	0, 0, 0,
	63,        // '/'
	52, 53, 54, 55, 56, 57, 58, 59, 60, 61,        // '0'-'9'
	0, 0, 0, 0, 0, 0, 0,
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
	13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,        // 'A'-'Z'
	0, 0, 0, 0, 0, 0,
	26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
	39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,        // 'a'-'z'
	};
	int ret = 0;
	int t = 0, x = 0, y = 0, i = 0;
	unsigned char c = 0;
	int g = 3;
	if (indata == NULL || inlen <= 0 || outdata == NULL || outlen == NULL) {
		return ret = -1;
	}
	if (inlen % 4 != 0) {
		return ret = -1;
	}
	while (indata[x] != 0) {
		c = base64_suffix_map[(unsigned char)indata[x++]];
		if (c == 255) return -1;
		if (c == 253) continue;
		if (c == 254) { c = 0; g--; }
		t = (t << 6) | c;
		if (++y == 4) {
			outdata[i++] = (unsigned char)((t >> 16) & 0xff);
			if (g > 1) outdata[i++] = (unsigned char)((t >> 8) & 0xff);
			if (g > 2) outdata[i++] = (unsigned char)(t & 0xff);
			y = t = 0;
		}
	}
	if (outlen != NULL) {
		*outlen = i;
	}
	return ret;
}

static char* base64_encode(unsigned char* data, unsigned int in_len) {
	static const char * base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	static char ret[128] = {0};
	memset(ret, 0x00, 128);
	int out_len = 0;
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];
	while (in_len--) {
		char_array_3[i++] = *(data++);
		if (i == 3) {
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;
			for (i = 0; (i < 4); i++) ret[out_len++] = base64_chars[char_array_4[i]];
			i = 0;
		}
	}
	if (i) {
		for (j = i; j < 3; j++) char_array_3[j] = '\0';
		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;
		for (j = 0; (j < i + 1); j++) ret[out_len++] = base64_chars[char_array_4[j]];
		while ((i++ < 3)) ret[out_len++] = '=';
	}
	return ret;
}

//websocket握手
static int websock_handshake(const char* key, char* rsp){
	char keys[1024]={0};
	unsigned char RetDigest[20];
	int n = sprintf(keys,"%s%s",key,"258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
	SHA1_CTX context;
	SHA1Init(&context);
	SHA1Update(&context, (unsigned char*)keys, n);
	SHA1Final(RetDigest, &context);
	return sprintf(rsp,"HTTP/1.1 101 Switching Protocols\r\nUpgrade: WebSocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept: %s\r\n\r\n",base64_encode(RetDigest, 20));
}

//wesocket数据解析
static int decodeframe(const char * frm, unsigned len, char** payload){
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
     unsigned char opcode = (unsigned char)(frm[0] & 0x0f);
     if (opcode == WS_TEXT_FRAME){
         // 处理utf-8编码的文本帧  
         payloadLength = (unsigned char)(frm[1] & 0x7f);
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
		 *payload = (char*)malloc(payloadLength + 1);
         memset(*payload, 0, payloadLength + 1);
         memcpy(*payload, &frm[2 + payloadFieldExtraBytes + 4], payloadLength);
         for (int i = 0; i < payloadLength; i++){
             (*payload)[i] = (*payload)[i] ^ maskingKey[i % 4];
         }
     }
     return ret;
 }

//web 请求处理
int websock_dispath(int sock, char* data, int len) {
	int disconn = 0;
	//0:disconnect
	if ( len == 0 ) {
		responsetolua(0,"{}", 2);
		return disconn;
	}

	//2: websock request
	if ( (unsigned char)(data[0]) == 0x81 ) {
		char * payload_data = NULL;
		int data_length = decodeframe(data, len, &payload_data);
		disconn = responsetolua(1, payload_data, data_length);
		free(payload_data);
		return disconn;
	}

	//1:handsake
	char web_key[64] = { 0 };
	int web_key_len = 200;
	while (web_key_len < len) {
		unsigned  count = strspn(data + web_key_len, "Sec-WebSocket-Key:");
		if ( 18 == count ) {
			unsigned  end = strcspn(data + web_key_len + count, "\r\n");
			memcpy(web_key, data + web_key_len + 19, end - 1);
			break;
		}
		web_key_len += (count + 1);
	}
	char send_buf[1024] = { 0 };
	int buflen = websock_handshake(web_key, send_buf);
	if (tcps_sends(sock, (void*)send_buf, buflen) == buflen) {
		disconn = 1;
	}
	return disconn;
}

void websock_start(const char* ip, unsigned short port) {
	if (!tcps) {
		tcps = tcps_start(ip, port, websock_dispath);
	}
}

void websock_stop() {
	if (tcps) {
		tcps_stop(tcps);
		tcps = NULL;
	}
}

//发送文本数据
int websock_send_text(SOCKET cs, const char* text){
	int nsent = 0;
	unsigned int   len = strlen(text);
	unsigned char szhead[4] = {0}; 
	szhead[0] = (unsigned char)(0x80|WS_TEXT_FRAME);
	if ( len <= 0x7d){
		szhead[1] = (unsigned char)(len) ;
		nsent =send(cs,(char*)szhead,2,0);
	}else{
		szhead[1] = 0x7e;
		szhead[2] = (unsigned char)(len>>8);
		szhead[3] = (unsigned char)(len);
		nsent=send(cs,(char*)szhead,4,0); 
	}
	nsent += send(cs,(char*)text,len,0);
	return nsent;
}

//发送二进制数据
int websock_send_bin(SOCKET cs, unsigned char* buf, unsigned int len ){
	int nsent = 0;
	unsigned char szhead[10] = {0};
	szhead[0] = (unsigned char)(0x80|WS_BINARY_FRAME);
	if ( len <= 0x7E){
		szhead[1] = (unsigned char)(len) ;
		nsent = send(cs,(char*)szhead,2,0);
	}else if(len < 0xFFFF){
		szhead[1] = 0x7E;
		szhead[2] = (unsigned char)(len>>8);
		szhead[3] = (unsigned char)(len);
		nsent = send(cs,(char*)szhead,4,0); 
	}else {
		szhead[1] = 0x7F;
		szhead[2] = 0;
		szhead[3] = 0;
		szhead[4] = 0;
		szhead[5] = 0;
		szhead[6] = (unsigned char)(len >> 24);
		szhead[7] = (unsigned char)(len >> 16);
		szhead[8] = (unsigned char)(len >> 8);
		szhead[9] = (unsigned char)(len & 0xFF);
		nsent = send(cs, (char*)szhead, 10, 0);
	}
	nsent += send(cs, (char*)buf,len,0); 
	return nsent;
}

int websock_getip(const char* nc, char* ip){
#ifdef WIN32
	char host_name[255]={0};
    if(gethostname(host_name,sizeof(host_name))==SOCKET_ERROR){  
        fprintf(stderr,"Error %d when getting local host name\n",WSAGetLastError());  
        return -1;  
    }  
    printf("host name:%s\n",host_name);  
    struct hostent *phe=gethostbyname(host_name);  
    if(phe==0){  
        fprintf(stderr,"Error host lookup\n");  
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
