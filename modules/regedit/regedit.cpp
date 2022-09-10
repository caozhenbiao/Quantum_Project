#include "regedit.h"
#include <Windows.h>
#include <iostream>
#include <winreg.h>

int cregedit::openKey(HKEY hMasterKey, const char* subkey, HKEY & hKey ) {
	int ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, subkey, 0, KEY_WOW64_64KEY | KEY_ALL_ACCESS, &hKey);
	if (ERROR_SUCCESS != ret) {
		printf("openKey failed, return:%d lasterror:%d\n", ret, GetLastError());
	}
	return ret;
}

int cregedit::createKey(HKEY hKey, const char* name, HKEY & subKey) {
	DWORD dwDisposition;
	int ret = RegCreateKeyEx(HKEY_LOCAL_MACHINE, name, 0, NULL, 0, KEY_WOW64_64KEY | KEY_WRITE, NULL, &subKey, &dwDisposition);
	//int ret = RegCreateKeyEx( hKey, name, &subKey);
	if (ERROR_SUCCESS != ret){
		printf("createKey failed, return:%d lasterror:%d\n", ret, GetLastError());
	}
	return ret;
}

int cregedit::writeValue( HKEY hKey ,  const char * name, const char* value ) {
	int ret = RegSetValueA(hKey, name, REG_SZ, value, strlen(value));
	if (ERROR_SUCCESS != ret) {
		printf("writeValue failed, return:%d lasterror:%d\n", ret, GetLastError());
	}
	return ret;
}

int cregedit::readValue(HKEY hKey, const char* name, char* value, int len) {
	DWORD dwType = REG_SZ;
	DWORD dwSize = len;
	int ret = RegQueryValueEx(hKey, name, 0, &dwType, (LPBYTE)value, &dwSize);
	if (ERROR_SUCCESS != ret) {
		printf("readValue failed, return:%d lasterror:%d\n", ret, GetLastError());
	}
	return ret;
}

int cregedit::writeValue(HKEY hKey, const char * name, int value) {
	int ret = RegSetValueA(hKey, name, REG_DWORD, (LPCSTR)&value, sizeof(DWORD));
	if (ERROR_SUCCESS != ret) {
		printf("writeValue failed, return:%d lasterror:%d\n", ret, GetLastError());
	}
	return ret;
}

int cregedit::readValue(HKEY hKey, const char* name, int& value) {
	DWORD dwType = REG_DWORD;
	DWORD dwSize = sizeof(DWORD);
	int ret = RegQueryValueEx(hKey, name, 0, &dwType, (LPBYTE)&value, &dwSize);
	if (ERROR_SUCCESS != ret) {
		printf("readValue failed, return:%d lasterror:%d\n", ret, GetLastError());
	}
	return ret;
}

int cregedit::closeKey(HKEY hKey) {
	int ret = RegCloseKey(hKey);
	if (ERROR_SUCCESS != ret) {
		printf("closeKey failed, return:%d lasterror:%d\n", ret, GetLastError());
	}
	return ret;
}
