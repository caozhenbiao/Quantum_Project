
#ifndef _REGEDIT_H_
#define _REGEDIT_H_
#include <map>
#include <Windows.h>
#include <iostream>


class cregedit {
public:
	int openKey(HKEY hMasterKey, const char* subkey, HKEY & hKey);
	int createKey(HKEY hKey, const char* name, HKEY & subKey);
	int writeValue(HKEY hKey, const char * subject, const char* value);
	int closeKey(HKEY hKey);


	int queryKey(HKEY hKey, const char* name, char* value);


private:
	std::map<int, HKEY> myHinstant;
};
#endif
