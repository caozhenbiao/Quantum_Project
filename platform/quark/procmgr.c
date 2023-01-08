#include<string.h>
#include "procmgr.h"
#include "./include/map.h"

typedef struct proc_info {
	int pid;
	char * name;
}proc_info;

typedef map_t(proc_info) proc_info_map_t;
proc_info_map_t  procstates;

void procmgr_start(){
	map_init(&procstates);
}

void procmgr_stop() {
	map_deinit(&procstates);
}

int process_add(const char* argc) {
	proc_info pinfo;
	memset(&pinfo, 0x00, sizeof(pinfo));
	map_set(&procstates, "name", pinfo);
	return 0;
}

int process_remove( char* name ) {
	map_remove(&procstates, name);
	return 0;
}

void procmgr_poll( int milliseconds ){
}




 

