#include "regedit.h"

int cregedit::openKey(HKEY hMasterKey, const char* subkey, HKEY & hKey ) {
	if (ERROR_SUCCESS != RegOpenKeyEx(hMasterKey, subkey, 0, KEY_ALL_ACCESS, &hKey)){
		printf("RegOpenKeyEx failed\n");
		printf("%d\n", GetLastError());
		return -1;
	}
	return SEC_E_OK;
}

int cregedit::createKey(HKEY hKey, const char* name, HKEY & subKey) {
	if (ERROR_SUCCESS != RegCreateKey(hKey, name, &subKey)){
		printf("RegStatusValueA failed\n");
		printf("%d\n", GetLastError());
		return -1;
	}
	return SEC_E_OK;
}
int cregedit::writeValue( HKEY hKey ,  const char * subject, const char* value ) {
	if (ERROR_SUCCESS != RegSetValueA(hKey, subject, REG_SZ, value, strlen(value))){
		printf("RegStatusValueA failed\n");
		printf("%d\n", GetLastError());
		return -1;
	}
	return SEC_E_OK;
}

int cregedit::closeKey(HKEY hKey) {
	if (ERROR_SUCCESS != RegCloseKey(hKey)){
		printf("RegCloseKey failed\n");
		printf("%d\n", GetLastError());
		return -1;
	}
	return SEC_E_OK;
}


int cregedit::queryKey(HKEY hKey, const char* name, char* value ) {
	DWORD dwType = REG_SZ;
	DWORD dwSize = strlen(value);
	if (ERROR_SUCCESS != RegQueryValueEx(hKey, name, 0, &dwType, (LPBYTE)&value, &dwSize)){
		printf("RegCloseKey failed\n");
		printf("%d\n", GetLastError());
		return true;
	}
	return SEC_E_OK;
}
 





