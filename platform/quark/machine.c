#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "machine.h"
#include "./include/threadpool.h"
#include <pthread.h>
#include <assert.h>
#include "luadisp.h"
#include "procmgr.h"
#include "webui.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/time.h>
#endif

pthread_mutex_t lock;
threadpool_t* pool_lua;
threadpool_t* pool_webui;
threadpool_t* pool_mac;

//web任务结构体
typedef struct web_task_t {
	int  cs;
	unsigned  data_len;
	char* data;
}web_task_t;

//处理webui请求
void process_webui_request(struct web_task_t * t) {
	webui_request_handle(t->cs, t->data, t->data_len);
	free(t->data);
	free(t);
}

//webui server recv  client request; 
int webui_request_dispath(int cs,  char* data,  unsigned len) {
	struct web_task_t * tsk = (web_task_t *)malloc(sizeof(web_task_t) );
	memset(tsk, 0x00, sizeof(web_task_t));
	tsk->cs = cs;
	tsk->data_len = len;
	tsk->data = (char*)malloc(len+4);
	memset(tsk->data, 0x00, len+4);
	memcpy(tsk->data, data, len+1);
	threadpool_add(pool_webui, (void*)&process_webui_request, (void*)tsk, 0);
	return 0; // -1：立即断开 0:须在处理函数中断开
}

//从命令行直接请求
void process_console_request(char *data) {
	char function[256] = {0};
	char args[1024] = { 0 };
	if (2 == sscanf(data, "%[^( ](%[^)]*", function, args)) {
		char * response = luadisp_execute(function, args, strlen(args));
		fprintf(stderr, "data:%s\n",response);
	}else {
		fprintf(stderr, "error function or args\n");
	}
}

//从命令行直接请求
void console_request_dispath(char* data) {
	threadpool_add(pool_lua, (void*)&process_console_request, (void*)&data[0], 0);
}

//LUA脚本定时器
void machine_poll(void * delay ) {
	int tms = luadisp_ticktime();
	if ( *(int*)delay == -1 ) {
		threadpool_add(pool_mac, &machine_poll, (void*)&tms , 0);
		return;
	}
#ifdef _WIN32
	Sleep( tms );
#else
	sleep(tms);
#endif
	threadpool_add(pool_mac, &luadisp_dotimer, NULL, 0);
	threadpool_add(pool_mac, &machine_poll, (void*)&tms, 0);
}

//机器启动
void machine_start(struct macopt* opt ){
	fprintf(stderr,"machine_start\n");
	pthread_mutex_init(&lock, NULL);
	pool_lua = threadpool_create(1, 256, 0);
	pool_mac = threadpool_create(1, 256, 0);
	pool_webui = threadpool_create(32, 256, 0);
	webui_start(webui_request_dispath, opt->webs_path, opt->webs_ip, opt->webs_port);
	webui_add_cgi(luadisp_execute, "LUA");
	luadisp_start(opt->script);
	int tms = -1;
	//machine_poll( (void*)&tms);
}

//机器关闭
void machine_stop() {
	threadpool_destroy(pool_lua, 0);
	threadpool_destroy(pool_mac, 0);
	threadpool_destroy(pool_webui, 0);
	luadisp_stop();
	webui_stop();
	fprintf(stderr,"machine shutdown\n");
}
