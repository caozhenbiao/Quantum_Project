#ifndef _MANAGER_H_
#define _MANAGER_H_
#include "kernel/xbase.h"
#include <map>
#include <string>
using namespace std; 
 
//MACHINE
class machine{
public:
	int open(const char* name, int ht, const char* params);
	int close(const char* name);
	int send(const char* name, const char* data, int len);
	int recv(const char* name, char* data, int len);
public:
	map<string, xbase*>  mymodules;    //Ä£¿éÁÐ±í
};
#endif
