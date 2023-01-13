#include "webui.h"
#include "include/tcps.h"
#include <winsock.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>

#define MAX_BUF_SIZE 10240
static tcps_t * tcps = NULL;
char*(*webui_cgi)(char*, char*,unsigned);
char wwwroot[256] = "/webui";

//http GMTIME
 char* get_gmtime(char* szBuf, int delay) {
	time_t tnow = time(NULL) + delay;
	strftime(szBuf, 127, "%a,%d %b %Y %H:%M:%S", gmtime(&tnow));
	return szBuf;
}

static const struct {
	const char  *extension;
	int          ext_len;
	const char  *mime_type;
} builtin_mime_types[] = {
	{"html",    4,  "text/html"                    },
	{"htm",     3,  "text/html"                    },
	{"txt",     3,  "text/plain"                   },
	{"css",     3,  "text/css"                     },
	{"ico",     3,  "image/x-icon"                 },
	{"gif",     3,  "image/gif"                    },
	{"jpg",     3,  "image/jpeg"                   },
	{"jpeg",    4,  "image/jpeg"                   },
	{"png",     3,  "image/png"                    },
	{"svg",     3,  "image/svg+xml"                },
	{"torrent", 7,  "application/x-bittorrent"     },
	{"wav",     3,  "audio/x-wav"                  },
	{"mp3",     3,  "audio/x-mp3"                  },
	{"mid",     3,  "audio/mid"                    },
	{"m3u",     3,  "audio/x-mpegurl"              },
	{"ram",     3,  "audio/x-pn-realaudio"         },
	{"ra",      2,  "audio/x-pn-realaudio"         },
	{"doc",     3,  "application/msword",          },
	{"exe",     3,  "application/octet-stream"     },
	{"zip",     3,  "application/x-zip-compressed" },
	{"xls",     3,  "application/excel"            },
	{"tgz",     3,  "application/x-tar-gz"         },
	{"tar.gz",  6,  "application/x-tar-gz"         },
	{"tar",     3,  "application/x-tar"            },
	{"gz",      2,  "application/x-gunzip"         },
	{"arj",     3,  "application/x-arj-compressed" },
	{"rar",     3,  "application/x-arj-compressed" },
	{"rtf",     3,  "application/rtf"              },
	{"pdf",     3,  "application/pdf"              },
	{"swf",     3,  "application/x-shockwave-flash"},
	{"mpg",     3,  "video/mpeg"                   },
	{"mpeg",    4,  "video/mpeg"                   },
	{"asf",     3,  "video/x-ms-asf"               },
	{"avi",     3,  "video/x-msvideo"              },
	{"bmp",     3,  "image/bmp"                    },
	{NULL,      0,  NULL                           }
};

static char * response_header_style = "HTTP/1.1 200 OK \r\n \
Date: %s GMT\r\n \
Expires: %s GMT\r\n \
Cache-Control: private, max-age=31536000\r\n \
X-Content-Type-Options: nosniff\r\n \
Server: smartx\r\n \
Content-Type: %s\r\n \
Content - Length: %ld\r\n\r\n";

struct vec{
	const char  *ptr;
	int     len;
} _http_methods[] = {
	{"GET",     3},
	{"POST",    4},
	{"LUAPOST",    7},
	{"PUT",     3},
	{"DELETE",  6},
	{"HEAD",    4},
	{NULL,      0}
};

static int get_request_method(const char *c){
	const struct vec *v = NULL;
	for (v = _http_methods; v->ptr != NULL; v++) {
		if (!memcmp(c, v->ptr, v->len)) {
			return (int)(v - _http_methods);
			break;
		}
	}
	return -1;
}

void webui_lua_response(int cs, char* data, unsigned len ) {
	char response_header[1024];
	memset(response_header, 0x00, sizeof(response_header));
	char inspire[128] = { 0 };
	char expires[128] = { 0 };
	get_gmtime(inspire, 0);
	get_gmtime(expires, 3600);
	int hdr_len = sprintf(response_header, response_header_style, inspire, expires,"LUA", len);
	tcps_sends(cs, response_header, hdr_len);
	tcps_sends(cs, data, len);
}

void webui_uri_response(int cs, char* uri, char* args) {
	char resource[256] = { 0 };
	sprintf(resource, "%s%s", wwwroot, uri);
	char response_header[1024];
	memset(response_header, 0x00, sizeof(response_header));
	char inspire[128] = { 0 };
	char expires[128] = { 0 };
	get_gmtime(inspire, 0);
	get_gmtime(expires, 3600);
	FILE* f = fopen( resource, "rb");
	if (!f) {
		static char * context = "<html><b><center>404 not find!</center></b></html>";
		int hdr_len = sprintf(response_header, response_header_style, inspire, expires, "mime_type", 0);
		tcps_sends(cs, response_header, hdr_len);
		tcps_sends(cs, context, strlen(context));
		return;
	}
	//返回文件数据
	fseek(f, 0, SEEK_END);
	int pos = ftell(f);
	int header_length = sprintf(response_header, response_header_style, inspire, expires, "mime_type", pos);
	tcps_sends(cs, response_header, header_length);
	fseek(f, 0, SEEK_SET);
	int nbs = MAX_BUF_SIZE;
	int npieces = (pos + nbs - 1) / nbs;
	for (int i = 0; i < npieces; i++) {
		char piece_buf[MAX_BUF_SIZE] = { 0 };
		int piece_length = fread(piece_buf, 1, nbs, f);
		tcps_sends(cs, piece_buf, piece_length);
	}
	fclose(f);
}

void webui_request_handle(int cs, char* data, unsigned len) {
	char uri[256] = { 0 };
	char args[256] = { 0 };
	char methord[32] = { 0 };
	sscanf(data, "%[^ ] %[^( |?)]?%[^ ]*", methord, uri, args);
	char * data_ptr = NULL;
	size_t data_len = 0;
	for (unsigned hdr_len = 4; hdr_len < len; hdr_len++) {
		if (data[hdr_len - 4] == 0x0D && data[hdr_len - 3] == 0x0A &&
			data[hdr_len - 2] == 0x0D && data[hdr_len - 1] == 0x0A) {
			data_ptr = data + hdr_len;
			data_len = len - hdr_len;
			break;
		}
	}
	if ( get_request_method(methord) == 2 ) {
		char * rdata = webui_cgi(&uri[1], data_ptr, data_len);
		webui_lua_response(cs, rdata, strlen(rdata));
	}else {
		webui_uri_response(cs, uri, args);
	}
	tcps_close(cs);
}

void webui_start(int(*function)(int, char*, unsigned),char*dir, char* ip,unsigned short port) {
	if (!tcps) {
		strcpy(wwwroot, dir);
		tcps = tcps_start(ip, port, function);
	}
}

void webui_stop() {
	if (tcps) {
		tcps_stop(tcps);
		tcps = NULL;
	}
}

void webui_add_cgi(char*(*function)(char*, char*, unsigned), const char* mark) {
	webui_cgi = function;
}