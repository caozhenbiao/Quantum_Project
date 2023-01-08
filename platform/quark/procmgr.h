#ifndef _PROCMGR_H_
#define _PROCMGR_H_
#ifdef __cplusplus
extern "C" {
#endif
void procmgr_start();
void procmgr_stop();
void procmgr_poll (int milliseconds);
int process_add(const char* argc);
int process_remove(char* name);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _PROCMGR_H_ */