#include "winos.h"
#include <winsvc.h>
#include <stdio.h>
#include <assert.h>
#include <lm.h>
#pragma comment(lib, "netapi32.lib")
#include <iostream>
#include <map>
#include <string>
#include <vector>
 
bool DisableService(const char* service) {
	SC_HANDLE hSC = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSC == NULL) {
		printf("OpenSCManager fail(%ld)", GetLastError());
		return false;
	}
	// open service
	SC_HANDLE hSvc = ::OpenService(hSC, service, SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);
	if (hSvc == NULL) {
		::CloseServiceHandle(hSC);
		return false;
	}
	// get service status
	SERVICE_STATUS status;
	if (::QueryServiceStatus(hSvc, &status) == FALSE) {
		::CloseServiceHandle(hSvc);
		::CloseServiceHandle(hSC);
		return false;
	}
	//if status == running, stop the service
	if (status.dwCurrentState == SERVICE_RUNNING) {
		if (::ControlService(hSvc, SERVICE_CONTROL_SHUTDOWN, &status) == FALSE) {
			::CloseServiceHandle(hSvc);
			::CloseServiceHandle(hSC);
			return false;
		}
		//wait the start operation
		while (::QueryServiceStatus(hSvc, &status) == TRUE) {
			::Sleep(status.dwWaitHint);
			if (status.dwCurrentState == SERVICE_STOPPED) {
				::CloseServiceHandle(hSvc);
				::CloseServiceHandle(hSC);
				return true;
			}
		}
	}
	::CloseServiceHandle(hSvc);
	::CloseServiceHandle(hSC);
	return true;
}
 
bool CloseService(const char* service) {
	SC_HANDLE hSC = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSC == NULL){
		printf("OpenSCManager fail(%ld)", GetLastError());
		return false;
	}
	// open service
	SC_HANDLE hSvc = ::OpenService(hSC, service,SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);
	if (hSvc == NULL){
		::CloseServiceHandle(hSC);
		return false;
	}
	// get service status
	SERVICE_STATUS status;
	if (::QueryServiceStatus(hSvc, &status) == FALSE){
		::CloseServiceHandle(hSvc);
		::CloseServiceHandle(hSC);
		return false;
	}
	//if status == running, stop the service
	if (status.dwCurrentState == SERVICE_RUNNING){
		if (::ControlService(hSvc,SERVICE_CONTROL_STOP, &status) == FALSE)
		{
			::CloseServiceHandle(hSvc);
			::CloseServiceHandle(hSC);
			return false;
		}
		//wait the stop operation
		while (::QueryServiceStatus(hSvc, &status) == TRUE)
		{
			::Sleep(status.dwWaitHint);
			if (status.dwCurrentState == SERVICE_STOPPED){
				::CloseServiceHandle(hSvc);
				::CloseServiceHandle(hSC);
				return true;
			}
		}
	}
	::CloseServiceHandle(hSvc);
	::CloseServiceHandle(hSC);
	return true;
}


bool StartService(const char* service) {
	SC_HANDLE hSC = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSC == NULL) {
		printf("OpenSCManager fail(%ld)", GetLastError());
		return false;
	}
	// open service
	SC_HANDLE hSvc = ::OpenService(hSC, service, SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);
	if (hSvc == NULL) {
		::CloseServiceHandle(hSC);
		return false;
	}
	// get service status
	SERVICE_STATUS status;
	if (::QueryServiceStatus(hSvc, &status) == FALSE) {
		::CloseServiceHandle(hSvc);
		::CloseServiceHandle(hSC);
		return false;
	}
	//if status == running, stop the service
	if (status.dwCurrentState != SERVICE_RUNNING) {
		if (::ControlService(hSvc, SERVICE_CONTROL_CONTINUE, &status) == FALSE){
			::CloseServiceHandle(hSvc);
			::CloseServiceHandle(hSC);
			return false;
		}
		//wait the start operation
		while (::QueryServiceStatus(hSvc, &status) == TRUE){
			::Sleep(status.dwWaitHint);
			if (status.dwCurrentState == SERVICE_RUNNING) {
				::CloseServiceHandle(hSvc);
				::CloseServiceHandle(hSC);
				return true;
			}
		}
	}
	::CloseServiceHandle(hSvc);
	::CloseServiceHandle(hSC);
	return true;
}

/*************************************
* AddUser
* 功能    增加用户
* 参数    szServerName，主机名，如果为本机增加用户，设置为NULL
*            szUserName，用户名
*            szPassword，密码
**************************************/
void AddUser( LPWSTR szServerName, LPWSTR szUserName, LPWSTR szPassword){
	USER_INFO_1 ui;
	DWORD dwLevel = 1;    // 使用 USER_INFO_1 作为参数
	DWORD dwError = 0;
	NET_API_STATUS nStatus;
	// 填充 USER_INFO_1
	ui.usri1_name = szUserName;    // 用户名
	ui.usri1_password = szPassword;    // 密码
	ui.usri1_priv = USER_PRIV_USER;    // privilege  
	ui.usri1_home_dir = NULL;
	ui.usri1_comment = NULL;//描述
	//UF_PASSWD_CANT_CHANGE 用户不能更改密码
	//UF_SCRIPT 登陆脚本执行，UF_DONT_EXPIRE_PASSWD 表示密码永不过期，  
	ui.usri1_flags = UF_SCRIPT | UF_DONT_EXPIRE_PASSWD;
	ui.usri1_script_path = NULL;
	// 调用 NetUserAdd 增加用户
	nStatus = NetUserAdd(szServerName,dwLevel,(LPBYTE)&ui,&dwError);
	// 判断结果
	if (nStatus == NERR_Success || nStatus == NERR_UserExists){
		wprintf((wchar_t*)stderr, L"User %s has been successfully added on %s\n",
			szUserName, szServerName);
	}else{
		fprintf(stderr, "A system error has occurred: %d\n", nStatus);
	}
}

/************************************
*MondUserName
*功能   修改用户名 ,，直接可以修改
*参数     szServerName，主机名，如果为本机，设置为NULL
oldUserName 老用户名
newUserName 新用户名
*************************************/
void MondUserName(LPWSTR szServerName, LPWSTR oldUserName, LPWSTR newUserName) {
	//TCHAR strName[60] = { 0 };
	//DWORD he;
	//::GetUserNameW(oldUserName, &he);
	USER_INFO_0 ui;
	ui.usri0_name = newUserName;
	DWORD dwResult = ::NetUserSetInfo(NULL, oldUserName, 0, (LPBYTE)&ui, NULL);
	int a = GetLastError();
	if (NERR_Success == dwResult){
		MessageBox(NULL, "修改成功", NULL, 0);
	}else{
		char buf[100] = { 0 };
		sprintf_s(buf, "错误代码是:%d", GetLastError());
		MessageBoxA(NULL, buf, NULL, 0);
	}
}


/*************************************
* GroupAdd
* 功能    添加用户组
* 参数    szServerName，主机名，如果为本机，设置为NULL
*        szGroup，用户组名
		szDescride  描述
**************************************/
int GroupAdd(LPWSTR szServerName,LPWSTR szGroup,LPWSTR szDescride){
	NET_API_STATUS nStatus;
	DWORD   level = 1;
	DWORD dwError = 0;
	LOCALGROUP_INFO_1 ui;
	ui.lgrpi1_name = szGroup;//组名
	ui.lgrpi1_comment = szDescride;//描述
	// 调用 NetGroupAdd
	//nStatus = NetGroupAdd(szServerName,level,(LPBYTE)&ui,&dwError);
	nStatus = NetLocalGroupAdd(szServerName, level, (LPBYTE)&ui, &dwError);
	// 判断结果
	if (nStatus == NERR_Success)
		fwprintf(stderr, L"User %s has been successfully added on %s\n",szGroup, szDescride);
	else
		fprintf(stderr, "NetGroupAdd A system error has occurred: %d\n", nStatus);
	return 0;
}
/*************************************
* AddUserToGroup
* 功能    为用户组增加用户
* 参数    szServerName，主机名，如果为本机，设置为NULL
*        szUserName，用户名
*        szGroup，用户组名
**************************************/
int AddUserToGroup(LPWSTR szServerName,LPWSTR szUserName,LPWSTR szGroup){
	NET_API_STATUS nStatus;
	LOCALGROUP_MEMBERS_INFO_3 account;
	// 调用 NetGroupAddUser
	//nStatus = NetGroupAddUser(szServerName,szGroup,szUserName);
	wchar_t szAccountName[20] = { 0 };
	const wchar_t *name;
	name = (const wchar_t *)szUserName;
	wcscpy_s(szAccountName, name);
	account.lgrmi3_domainandname = szAccountName;
	nStatus = NetLocalGroupAddMembers(szServerName, szGroup, 3, (LPBYTE)&account, 1);

	// 判断结果
	if (nStatus == NERR_Success || nStatus == ERROR_MEMBER_IN_ALIAS)
		//if (nStatus == NERR_Success)
		fwprintf(stderr, L"User %s has been successfully added on %s\n",
			szUserName, szGroup);

	else
		fprintf(stderr, "NetGroupAddUser A system error has occurred: %d\n", nStatus);
	return 0;
}
/*************************************
* DelUserGroup
* 功能    删除用户组
* 参数    szServerName，主机名，如果为本机，设置为NULL
*        szUserName，用户组名
**************************************/
int DelUserGroup(LPWSTR szServerName, LPWSTR szGroupName)
{
	DWORD dwError = 0;
	NET_API_STATUS nStatus;

	// 调用 NetLocalGroupDel 删除用户组
	nStatus = NetLocalGroupDel(szServerName, szGroupName);
	// 判断并显示结果
	if (nStatus == NERR_Success)
		fwprintf(stderr, L"User %s has been successfully deleted on %s\n",
			szGroupName, szServerName);
	else
		fprintf(stderr, "A system error has occurred: %d\n", nStatus);

	return 0;

}
/*************************************
* DelUser
* 功能    删除用户
* 参数    szServerName，主机名，如果为本机，设置为NULL
*            szUserName，用户名
**************************************/
int DelUser(LPWSTR szServerName, LPWSTR szUserName)
{
	DWORD dwError = 0;
	NET_API_STATUS nStatus;

	// 调用 NetUserDel 删除用户
	nStatus = NetUserDel(szServerName, szUserName);
	// 判断并显示结果
	if (nStatus == NERR_Success)
		fwprintf(stderr, L"User %s has been successfully deleted on %s\n",
			szUserName, szServerName);
	else
		fprintf(stderr, "A system error has occurred: %d\n", nStatus);

	return 0;

}

/*************************************
* int ListUsers(LPWSTR pszServerName)
* 功能    列举用户
* 参数    szServerName，主机名，如果为本机，设置为NULL
**************************************/
int ListUsers(LPWSTR pszServerName)
{
	LPUSER_INFO_0 pBuf = NULL;
	LPUSER_INFO_0 pTmpBuf;
	DWORD dwLevel = 0;
	DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
	DWORD dwEntriesRead = 0;
	DWORD dwTotalEntries = 0;
	DWORD dwResumeHandle = 0;
	DWORD i;
	DWORD dwTotalCount = 0;
	NET_API_STATUS nStatus;

	// 循环，直到可以成功调用 NetUserEnum
	do
	{
		// 调用NetUserEnum函数
		nStatus = NetUserEnum(pszServerName,
			dwLevel,// 这里设置为0，使用 LPUSER_INFO_0 返回结果
			FILTER_NORMAL_ACCOUNT, // 只列举“正常”类型的用户
			(LPBYTE*)&pBuf,// LPUSER_INFO_0 保存返回结果
			// MAX_PREFERRED_LENGTH，内存由API分配，需要在之后调用NetApiBufferFree释放
			dwPrefMaxLen,
			&dwEntriesRead,// 读了的 Entries
			&dwTotalEntries,// 一共的 Entries
			&dwResumeHandle);
		// 判断是否成功
		if ((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA)){
			if ((pTmpBuf = pBuf) != NULL){
				// 循环读取用户信息
				for (i = 0; (i < dwEntriesRead); i++){
					assert(pTmpBuf != NULL);
					if (pTmpBuf == NULL){
						fprintf(stderr, "An access violation has occurred\n");
						break;
					}
					wprintf(L"\t-- %s\n", pTmpBuf->usri0_name);
					pTmpBuf++;
					dwTotalCount++;
				}
			}
		}else
			fprintf(stderr, "A system error has occurred: %d\n", nStatus);
		// 释放内存
		if (pBuf != NULL){
			NetApiBufferFree(pBuf);
			pBuf = NULL;
		}
	} while (nStatus == ERROR_MORE_DATA); // end do

	// 释放内存
	if (pBuf != NULL)
		NetApiBufferFree(pBuf);

	fprintf(stderr, "Total of %d users\n\n", dwTotalCount);
	return 0;
}

/*************************************
* int ListGroup(LPWSTR pszServerName)
* 功能    列举用户组
* 参数    szServerName，主机名，如果为本机，设置为NULL
**************************************/
int ListGroup(LPWSTR pszServerName){
	DWORD dwLevel = 0;
	DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
	DWORD dwEntriesRead = 0;
	DWORD dwTotalEntries = 0;
	DWORD dwResumeHandle = 0;
	DWORD i;
	DWORD dwTotalCount = 0;
	NET_API_STATUS nStatus;
	LPLOCALGROUP_INFO_0 pBuf = NULL;
	LPLOCALGROUP_INFO_0 pTmpBuf;

	do{
		// 调用NetLocalGroupEnum 参数设置与NetLocalGroup类似
		nStatus = NetLocalGroupEnum(pszServerName,0,(LPBYTE*)&pBuf,dwPrefMaxLen,&dwEntriesRead,&dwTotalEntries,(PDWORD_PTR)&dwResumeHandle);
		// 判断结果
		if ((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA)){
			if ((pTmpBuf = pBuf) != NULL){
				for (i = 0; (i < dwEntriesRead); i++){
					assert(pTmpBuf != NULL);
					if (pTmpBuf == NULL){
						fprintf(stderr, "An access violation has occurred\n");
						break;
					}
					wprintf(L"\t-- %s\n", pTmpBuf->lgrpi0_name);
					pTmpBuf++;
					dwTotalCount++;
				}
			}
		}
		else
			fprintf(stderr, "A system error has occurred: %d\n", nStatus);
		// 释放内存
		if (pBuf != NULL){
			NetApiBufferFree(pBuf);
			pBuf = NULL;
		}
	}
	while (nStatus == ERROR_MORE_DATA); // end do
	if (pBuf != NULL)
		NetApiBufferFree(pBuf);
	fprintf(stderr, "Total of %d groups\n\n", dwTotalCount);
	return 0;
}

/*************************************
* ShowUsersInfo
* 功能    显示指定用户的信息
* 参数    szServerName，主机名，如果为本机，设置为NULL
*            pszUserName，用户名
**************************************/
int ShowUsersInfo(LPWSTR pszServerName, LPWSTR pszUserName){
	DWORD dwLevel = 4;// 使用 LPUSER_INFO_4 返回结果
	LPUSER_INFO_4 pBuf = NULL;
	NET_API_STATUS nStatus;
	nStatus = NetUserGetInfo(pszServerName,pszUserName,dwLevel,(LPBYTE *)&pBuf);
	// 判断并输出结果
	if (nStatus == NERR_Success){
		if (pBuf != NULL){
			wprintf(L"\n\tAccount:      %s\n", pBuf->usri4_name);
			wprintf(L"\tComment:      %s\n", pBuf->usri4_comment);
			wprintf(L"\tUser comment: %s\n", pBuf->usri4_usr_comment);
			wprintf(L"\tFull name:    %s\n", pBuf->usri4_full_name);
			wprintf(L"\tpriv:    %d\n", pBuf->usri4_priv);
		}
	}else
		fprintf(stderr, "A system error has occurred: %d\n", nStatus);
	// 释放内存
	if (pBuf != NULL)
		NetApiBufferFree(pBuf);
	return 0;

}

/************************************
*mondPass
*功能   修改密码 ,不需要输入旧密码验证，直接可以修改
*参数     szServerName，主机名，如果为本机，设置为NULL
pass 修改的密码
*************************************/
void MondPass(LPWSTR strName, LPWSTR pass) {
	//TCHAR strName[10] = { 0 };
	//DWORD he;
	//::GetUserNameW(strName, &he);
	USER_INFO_1003 ui;
	ui.usri1003_password = pass;
	DWORD dwResult = ::NetUserSetInfo(NULL, strName, 1003, (LPBYTE)&ui, NULL);
	int a = GetLastError();
	if (NERR_Success == dwResult){
		MessageBox(NULL, "修改成功", NULL, 0);
	}else{
		char buf[100] = { 0 };
		sprintf_s(buf, "错误代码是:%d", GetLastError());
		MessageBoxA(NULL, buf, NULL, 0);
	}
}

/************************************
*MondifyPassword
*功能   修改密码 ，需要输入旧密码验证才能修改
*参数
szServerName  主机名，如果为本机，设置为NULL
userName      该NetUserChangePassword函数改变指定用户的密码。如果此参数为NULL，则使用调用方的登录名
oldPassword   用户的旧密码
newpassword   用户的新密码
*************************************/
void MondifyPassword(LPWSTR szServerName, LPWSTR userName, LPWSTR oldPassword, LPWSTR newpassword){
	DWORD dwError = 0;
	DWORD dwLevel = 5;
	NET_API_STATUS nStatus;
	if (dwLevel != 5){
		fwprintf(stderr, L"Usage: %s \\\\ServerName UserName OldPassword NewPassword\n", L"");
		exit(1);
	}
	nStatus = NetUserChangePassword(szServerName, userName, oldPassword, newpassword);
	if (nStatus == NERR_Success) {//调用成功，请通知用户
		fwprintf(stderr, L"User password has been changed successfully\n");
	}else{
		fprintf(stderr, "A system error has occurred: %d\n", nStatus);
	}
}

int GetAllUserInfo(std::vector<std::string>& data ) {
	LPUSER_INFO_0 pBuf = NULL;
	LPUSER_INFO_0 pTmpBuf;
	DWORD dwEntriesRead = 0;
	DWORD dwTotalEntries = 0;
	DWORD dwResumeHandle = 0;
	std::vector<std::wstring> userList;
	NET_API_STATUS nStatus;
	// 循环，直到可以成功调用 NetUserEnum
	do {
		nStatus = NetUserEnum(NULL, 0,// 这里设置为0，使用 LPUSER_INFO_0 返回结果
			FILTER_NORMAL_ACCOUNT, // 只列举“正常”类型的用户
			(LPBYTE*)&pBuf,// LPUSER_INFO_0 保存返回结果
			MAX_PREFERRED_LENGTH, //内存由API分配，需要在之后调用NetApiBufferFree释放
			&dwEntriesRead,// 读了的 Entries
			&dwTotalEntries,// 一共的 Entries
			&dwResumeHandle);
		if ((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA)) {
			if ((pTmpBuf = pBuf) != NULL) {
				// 循环读取用户信息
				for (unsigned int i = 0; ( i < dwEntriesRead); i++) {
					assert(pTmpBuf != NULL);
					if (pTmpBuf == NULL) {
						fprintf(stderr, "An access violation has occurred\n");
						break;
					}
					userList.push_back(pTmpBuf->usri0_name);
					pTmpBuf++;
				}
			}
		}
		if (pBuf != NULL) {
			NetApiBufferFree(pBuf);
			pBuf = NULL;
		}
	} while (nStatus == ERROR_MORE_DATA); // end do
	if (pBuf != NULL) {
		NetApiBufferFree(pBuf); 
	}
	//获取所有用户信息
	for( unsigned int i = 0; i<userList.size(); i++ ){
		LPUSER_INFO_4 pBuf = NULL;
		LPCWSTR userName = userList[i].c_str();
		char szInfo[1204] = { 0 };
		wchar_t uinfo[1204] = {0};
		NET_API_STATUS nStatus = NetUserGetInfo(NULL, userName , 4, (LPBYTE *)&pBuf);
		if ( nStatus==NERR_Success && pBuf != NULL) {
			swprintf_s(uinfo, 1204, L"{'Account':'%s','Fullname':'%s','PwdAge':%d,'Flags':%d,'AccExpires':%d,'PwdExpires':%d}",
				pBuf->usri4_name, 
				pBuf->usri4_full_name,
				pBuf->usri4_password_age,
				pBuf->usri4_flags,
				pBuf->usri4_acct_expires,
				pBuf->usri4_password_expired);
		}else {
			swprintf_s(uinfo, 1024, L"{'Account':'%s','Fullname':'','PwdAge':0,'Flags':0,'AccExpires':0,'PwdExpires':0}", userName);
		}
		int nLen1 = WideCharToMultiByte(CP_ACP, NULL, uinfo, wcslen(uinfo), NULL, 0, NULL, FALSE);
		int nLen2 = WideCharToMultiByte(CP_ACP, NULL, uinfo, wcslen(uinfo), szInfo, nLen1, NULL, FALSE);
		szInfo[nLen1] = '\0';
		data.push_back(szInfo);
		if (pBuf != NULL) {
			NetApiBufferFree(pBuf);
		}
	}
	return 0;
}

int RemoveUser(const char * userName) {
	wchar_t wcName[1024] = { 0 };
	int len = MultiByteToWideChar(CP_ACP, 0, userName, strlen(userName), NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, userName, strlen(userName), wcName, len);
	wcName[len] = '\0';
	return NetUserDel(NULL, wcName);
}