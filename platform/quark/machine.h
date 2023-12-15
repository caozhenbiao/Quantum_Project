 //thread pool
// https://github.com/sysprog21/mapreduce
// https://github.com/Pithikos/C-Thread-Pool
// https://github.com/mbrossard/threadpool

#ifndef _MACHINE_H_
#define _MACHINE_H_

struct macopt {
	char script[256];
	char webs_path[256];
	int console;
	char tcps[256]; //127.0.0.1:80
	char ssls[256]; //192.168.1.1:12313;ca.crt;server.crt;server.key
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
