#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <windows.h>
#include <vector>
#include <string>


bool DisableService(const char* service);
bool CloseService(const char* service);
bool StartService(const char* service);

void AddUser(LPWSTR szServerName, LPWSTR szUserName, LPWSTR szPassword);
void MondUserName(LPWSTR szServerName, LPWSTR oldUserName, LPWSTR newUserName);
int GroupAdd(LPWSTR szServerName, LPWSTR szGroup, LPWSTR szDescride);
int AddUserToGroup(LPWSTR szServerName, LPWSTR szUserName, LPWSTR szGroup);
int DelUserGroup(LPWSTR szServerName, LPWSTR szGroupName);
int DelUser(LPWSTR szServerName, LPWSTR szUserName);
int ListUsers(LPWSTR pszServerName);
int ListGroup(LPWSTR pszServerName);
int ShowUsersInfo(LPWSTR pszServerName, LPWSTR pszUserName);
void MondPass(LPWSTR strName, LPWSTR pass);
void MondifyPassword(LPWSTR szServerName, LPWSTR userName, LPWSTR oldPassword, LPWSTR newpassword);
int GetAllUserInfo(std::vector<std::string>& data);
int RemoveUser(const char * userName);

