#ifndef _LUADISP_H_
#define _LUADISP_H_
#ifdef __cplusplus
extern "C" {
#endif
void luadisp_dotimer(void*); 
void luadisp_doclose(void);
char* luadisp_execute(char* iface, char* data, unsigned len);
int luadisp_start(const char* file);
int luadisp_stop(void);
int luadisp_ticktime();
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _LUADISP_H_ */
