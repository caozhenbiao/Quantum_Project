#ifndef _HTTP_H_
#define _HTTP_H_
#ifdef __cplusplus
extern "C" {
#endif

int http_get_request(const char* uri, const char * data, char ** payload);
int http_post_request(const char* uri, const char * data, int len, char ** payload );

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _HTTP_H_ */
