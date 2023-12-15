#ifndef _SSLS_H_
#define _SSLS_H_
#ifdef __cplusplus
extern "C" {
#endif

typedef struct ssls_t ssls_t;
ssls_t *  ssls_start(const char* ip, unsigned short port, int(*function)(int, char*,unsigned),const char*ca, const char *sa, const char* pk);
void ssls_stop(ssls_t * s);
int ssls_shutdown(ssls_t * s, int  cs);
int ssls_sends(int cs, const void* buf, int size);
int ssls_close(int sock);
int ssls_shutdown(ssls_t * s, int  cs);

#ifdef _WIN32
unsigned __stdcall ssls_workthread(void* lpParam);
#else
void* ssls_workthread(void* lpParam);
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _SSLS_H_ */
