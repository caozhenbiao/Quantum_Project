#ifndef _HTTP_H_
#define _HTTP_H_
#ifdef __cplusplus
extern "C" {
#endif
typedef struct http_request_t {
	char * package;
	int package_len;
	char * data;
	int  data_len;
} http_request_t;

int tcps_start(const char* ip, short port, int(*function)(char*, char*, unsigned,char*));
int tcps_stop();


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _HTTP_H_ */
