#ifndef _TCPS_H_
#define _TCPS_H_
#ifdef __cplusplus
extern "C" {
#endif

typedef struct tcps_t tcps_t;
tcps_t *  tcps_start(const char* ip, unsigned short port, int(*function)(int, char*,int));
void tcps_stop(tcps_t * s);
int tcps_sends(int sock, void* buf, int size);
int tcps_close(int sock);
unsigned __stdcall tcps_workthread(void* lpParam);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _TCPS_H_ */