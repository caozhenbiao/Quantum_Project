 //thread pool
// https://github.com/sysprog21/mapreduce
// https://github.com/Pithikos/C-Thread-Pool
// https://github.com/mbrossard/threadpool

#ifndef _MACHINE_H_
#define _MACHINE_H_

struct macopt {
	char runkey[41];
	char script[256];
	char webs_path[256];
	char webs_ip[32];
	unsigned short webs_port;
	char option[256];
	int console;
};

#ifdef __cplusplus
extern "C" {
#endif
void machine_start(struct macopt* opt);
void machine_stop();
void console_request_dispath(char* data);
int    webui_request_dispath(int cs, char* data, unsigned len);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MACHINE_H_ */
