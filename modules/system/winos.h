#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <windows.h>
#include <vector>
#include <string>
#include <map>


typedef struct _serviceInfo {
	std::string name;
	std::string display;
	unsigned int status;
} serviceInfo;


bool DisableService(const char* service);
bool CloseService(const char* service);
bool StartService(const char* service);
int  EnumService(std::vector<serviceInfo> & services);

void AddUser(LPWSTR szServerName, LPWSTR szUserName, LPWSTR szPassword);
void MondUserName(LPWSTR szServerName, LPWSTR oldUserName, LPWSTR newUserName);
int GroupAdd(LPWSTR szServerName, LPWSTR szGroup, LPWSTR szDescride);
int AddUserToGroup(LPWSTR szServerName, LPWSTR szUserName, LPWSTR szGroup);
int DelUserGroup(LPWSTR szServerName, LPWSTR szGroupName);
int DelUser(LPWSTR szServerName, LPWSTR szUserName);
int ListUsers(LPWSTR pszServerName);
int ListGroup(LPWSTR pszServerName);
int ShowUsersInfo(LPWSTR pszServerName, LPWSTR pszUserName);
bool MondPass(LPWSTR strName, LPWSTR pass);
bool MondifyPassword(LPWSTR szServerName, LPWSTR userName, LPWSTR oldPassword, LPWSTR newpassword);
int GetAllUserInfo(std::vector<std::string>& data);
int RemoveUser(const char * userName);
bool CheckWinUser(const char * name, const char* pwd);
bool MondPass(const char * name, const char * opwd, const char * npwd);
