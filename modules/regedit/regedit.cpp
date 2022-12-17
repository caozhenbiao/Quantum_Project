#include "regedit.h"
#include <Windows.h>
#include <iostream>
#include <winreg.h>

int cregedit::openKey(HKEY hMasterKey, const char* subkey, HKEY & hKey) {
	int ret = RegOpenKeyEx(hMasterKey, subkey, 0, KEY_WOW64_64KEY | KEY_ALL_ACCESS, &hKey);
	if (ERROR_SUCCESS != ret) {
		printf("openKey failed, return:%d lasterror:%d\n", ret, GetLastError());
	}
	return ret;
}

int cregedit::createKey(HKEY hKey, const char* name, HKEY & subKey) {
	DWORD dwDisposition;
	int ret = RegCreateKeyEx(hKey, name, 0, NULL, 0, KEY_WOW64_64KEY | KEY_WRITE, NULL, &subKey, &dwDisposition);
	if (ERROR_SUCCESS != ret) {
		printf("createKey failed, return:%d lasterror:%d\n", ret, GetLastError());
	}
	return ret;
}

int cregedit::writeValue(HKEY hKey, const char * name, const char* value) {
	int ret = RegSetValueEx(hKey, name, 0, REG_SZ, (LPBYTE)value, strlen(value));
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

int cregedit::writeMulti(HKEY hKey, const char * name, const char* value) {
	int ret = RegSetValueEx(hKey, name, 0, REG_MULTI_SZ, (LPBYTE)value, strlen(value));
	if (ERROR_SUCCESS != ret) {
		printf("writeValue failed, return:%d lasterror:%d\n", ret, GetLastError());
	}
	return ret;
}

int cregedit::readMulti(HKEY hKey, const char* name, char* value, int len) {
	DWORD dwType = REG_MULTI_SZ;
	DWORD dwSize = len;
	int ret = RegQueryValueEx(hKey, name, 0, &dwType, (LPBYTE)value, &dwSize);
	if (ERROR_SUCCESS != ret) {
		printf("readValue failed, return:%d lasterror:%d\n", ret, GetLastError());
	}
	return ret;
}

int cregedit::writeValue(HKEY hKey, const char * name, int value) {
	int ret = RegSetValueEx(hKey, name, 0, REG_DWORD, (LPBYTE)&value, sizeof(DWORD));
	if (ERROR_SUCCESS != ret) {
		printf("writeValue failed, return:%d name:%s value:%d lasterror:%d\n", ret, name, value, GetLastError());
	}
	return ret;
}

int cregedit::readValue(HKEY hKey, const char* name, int& value) {
	DWORD dwType = REG_DWORD;
	DWORD dwSize = sizeof(DWORD);
	int ret = RegQueryValueEx(hKey, name, NULL, &dwType, (LPBYTE)&value, &dwSize);
	if (ERROR_SUCCESS != ret) {
		printf("readValue failed, return:%d lasterror:%d\n", ret, GetLastError());
	}
	return ret;
}

int cregedit::deleteKey(HKEY hKey, const char* name) {
	return RegDeleteKey(hKey, name);
}

int cregedit::deleteValue(HKEY hKey, const char* name) {
	int ret = RegDeleteValue(hKey, name);
	return ret;
}

int cregedit::closeKey(HKEY hKey) {
	int ret = RegCloseKey(hKey);
	if (ERROR_SUCCESS != ret) {
		printf("closeKey failed, return:%d lasterror:%d\n", ret, GetLastError());
	}
	return ret;
}
