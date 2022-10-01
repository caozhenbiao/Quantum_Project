
#ifndef _REGEDIT_H_
#define _REGEDIT_H_
#include <map>
#include <Windows.h>
#include <iostream>


class cregedit {
public:
	int openKey(HKEY hMasterKey, const char* subkey, HKEY & hKey);
	int createKey(HKEY hKey, const char* name, HKEY & subKey);
	int writeValue(HKEY hKey, const char * name, const char* value);
	int readValue(HKEY hKey, const char* name, char* value, int len);

	int writeValue(HKEY hKey, const char * name, int value);
	int readValue(HKEY hKey, const char* name, int& value);


	int closeKey(HKEY hKey);





	int deleteKey(HKEY hKey, const char* name);
	int deleteValue(HKEY hKey, const char* name);



 



 

private:
	std::map<int, HKEY> myHinstant;
};
#endif
