#include "machine.h"
#include "module/cssp.h"
#include "module/udpc.h"
#include "module/tcpc.h"


//创建不同的功能模块
int machine::open(const char* name, int ht, const char* params){
	map<string, xbase*>::iterator ifnd = mymodules.find(name);
	if (ifnd != mymodules.end())
		return 0;
	xbase * newmdl = NULL;
	switch( ht ){
	case 1: newmdl = new cssp(); break;
	case 2: newmdl = new tcpc(); break;
	case 3: newmdl = new udpc(); break;
	default:  return -1;
	}
	if (newmdl->open(params)) {
		mymodules.insert(make_pair(name, newmdl));
	}else {
		delete newmdl;
		return -1;
	}
	return 0;
}

int  machine::close(const char* name) {
	map<string, xbase*>::iterator ifnd = mymodules.find(name);
	if (ifnd != mymodules.end()) {
		xbase* mod = ifnd->second;
		mod->close();
		mymodules.erase(name);
	}
	return true;
}

int machine::send(const char* name, const char* data, int len) {
	map<string, xbase*>::iterator ifnd = mymodules.find(name);
	if (ifnd != mymodules.end()) {
		xbase* mod = ifnd->second;
		int writed = mod->writedata(data, len);
		return writed;
	}
	return -1;
}

int machine::recv(const char* name, char* data, int len) {
	map<string, xbase*>::iterator ifnd = mymodules.find(name);
	if (ifnd != mymodules.end()) {
		xbase* mod = ifnd->second;
		int readed = mod->readdata(data, len);
		return readed;
	}
	return -1;
}
