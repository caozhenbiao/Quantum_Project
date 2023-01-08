#include <stdio.h>
#include "machine.h"
#include "./include/threadpool.h"
#include <pthread.h>
#include <assert.h>
#include <time.h>
#include "luadisp.h"
#include "procmgr.h"
#include "webui.h"
#include <Windows.h>
pthread_mutex_t lock;
#define THREAD 32
#define QUEUE  256
threadpool_t* pool_lua;
threadpool_t* pool_webui;
threadpool_t* pool_mac;

typedef struct web_request_t {
	int  cs;
	unsigned  data_len;
	char* data;
}web_request_t;

void process_webui_request(struct web_request_t * t) {
	webui_request_handle(t->cs, t->data, t->data_len);
	free(t->data);
	free(t);
}

//webui server recv  client request; 
int machine_request_dispath(int cs,  char* data,  unsigned len) {
	struct web_request_t * tsk = (web_request_t *)malloc(sizeof(web_request_t) );
	memset(tsk, 0x00, sizeof(web_request_t));
	tsk->cs = cs;
	tsk->data_len = len;
	tsk->data = (char*)malloc(len+4);
	memset(tsk->data, 0x00, len+4);
	memcpy(tsk->data, data, len+1);
	threadpool_add(pool_webui, &process_webui_request, (void*)tsk, 0);
	return 1;
}

void process_console_request(char *data) {
	char function[256] = {0};
	char args[1024] = { 0 };
	if (2 == sscanf(data, "%[^( ](%[^)]*", function, args)) {
		char * response = luadisp_execute(function, args, strlen(args));
		printf(response);
		printf("\r\n");
	}else {
		printf("error function or args\r\n");
	}
}

//add to lua task deque
void machine_console_dispath(char* data) {
	threadpool_add(pool_lua, &process_console_request, (void*)data, 0);
}

void machine_poll(void * milliseconds) {
	int tms = 1002;
	if ( *(int*)milliseconds == 1000) {
		threadpool_add(pool_mac, &machine_poll, (void*)&tms , 0);
		return;	
	}
	Sleep( 1000 );
	threadpool_add(pool_mac, &luadisp_dotimer, NULL, 0);
	threadpool_add(pool_mac, &machine_poll, (void*)&tms, 0);
}

void machine_start(struct macopt* opt ){
	printf("machine_start\n");
	pthread_mutex_init(&lock, NULL);
	pool_lua = threadpool_create(1, QUEUE, 0);
	pool_mac = threadpool_create(1, QUEUE, 0);
	pool_webui = threadpool_create(1, QUEUE, 0);
	webui_start(machine_request_dispath, opt->webs_path, opt->webs_ip, opt->webs_port);
	webui_add_cgi(luadisp_execute, "LUA");
	luadisp_start(opt->script);
	int tms = 1000;
	machine_poll( (void*)&tms );
}

void machine_stop() {
	threadpool_destroy(pool_lua, 0);
	threadpool_destroy(pool_mac, 0);
	threadpool_destroy(pool_webui, 0);
	luadisp_stop();
	webui_stop();
	printf("machine shutdown\n");
}
