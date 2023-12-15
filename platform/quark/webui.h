#ifndef _WEBUI_H_
#define _WEBUI_H_
#ifdef __cplusplus
extern "C" {
#endif
void webui_start_http(int(*function)(int, char*,unsigned), char*dir, char* ip, unsigned short port);
void webui_start_https(int(*function)(int, char*, unsigned), char*dir, char* ip, unsigned short port, char* ca, char* sa, char* pk);
void webui_stop();
void webui_add_cgi(char*(*function)(char*, char*, unsigned), const char* mark);
void webui_request_handle(int cs, char* data, unsigned len);
void webui_uri_response(int cs, char* uri, char* args);
void webui_lua_response(int cs, char* data, unsigned len);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _WEBUI_H_ */
