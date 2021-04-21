
#include "Log.h"
#include <time.h>


#ifdef WIN32
#include <time.h>
#include <process.h>
#include <direct.h>
#include <io.h>
#define WIN32_LEAN_AND_MEAN  //这很重要，解决winsock.h winsock2.h冲突
#include <windows.h>
#define ACCESS _access   
#define MKDIR(a) _mkdir((a))   
 
#else
#include <time.h>
#include <unistd.h>
#include <stdarg.h>   
#include <sys/stat.h> 
#define ACCESS access   
#define MKDIR(a) mkdir((a),0755)   
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define sprintf_s snprintf
#define __int64 long long
#define Sleep(x) usleep(x * 1000)
#include <sys/vfs.h>
#include <mntent.h>
#include <dirent.h>
#include <sys/types.h>
#endif


CLogger* CLogger::m_pLogInstance = NULL;
char LOG_FILE_NAME[128];
char LOG_FILE_PATH[256];
int m_traceLevel;

void WriteLog1( const int iMsgType, const char * strMsg)
{
	time_t tt;
	struct tm tm1;
	char buf[512];
	FILE * fpLog = NULL;

	time(&tt);
#ifdef WIN32
	localtime_s(&tm1, &tt);
#else
	localtime_r(&tt, &tm1);
#endif
	sprintf_s(buf, 500, "%04d-%02d-%02d %02d:%02d:%02d "
		,tm1.tm_year+1900,  tm1.tm_mon+1, tm1.tm_mday
		,tm1.tm_hour, tm1.tm_min, tm1.tm_sec);

	char szpath[256];
	sprintf_s(szpath,256,"%s/%04d-%02d-%02d%s",
		LOG_FILE_PATH,
		tm1.tm_year+1900,  tm1.tm_mon+1, tm1.tm_mday,LOG_FILE_NAME);

#ifdef WIN32
	fopen_s(&fpLog, szpath, "a+");
#else
	fpLog = fopen(szpath, "a+");
#endif
	if(NULL != fpLog)
	{
		fseek(fpLog, 0, SEEK_END);
		fwrite(buf, strlen(buf), 1, fpLog);
		fwrite(strMsg, strlen(strMsg), 1, fpLog); 
		fwrite("\n",1, 1, fpLog);
		fclose(fpLog);
	}
}

#ifdef WIN32
#include <windows.h>
#include <atlcomtime.h>
char SVCNAME[128];
void WriteLog( const int iMsgType, const char * strMsg)
{
	if(iMsgType < int(eTipMessage))
	{
		HANDLE hEventSource;
		LPCTSTR lpszStrings[2];
		hEventSource = RegisterEventSource(NULL, SVCNAME);
		if( NULL != hEventSource )
		{
			lpszStrings[0] = SVCNAME;
			lpszStrings[1] = strMsg;

			ReportEvent(hEventSource,        // event log handle
				EVENTLOG_ERROR_TYPE, // event type
				0,                   // event category
				0,           // event identifier
				NULL,                // no security identifier
				2,                   // size of lpszStrings array
				0,                   // no binary data
				lpszStrings,         // array of strings
				NULL);               // no binary data

			DeregisterEventSource(hEventSource);
		}
	}
	WriteLog1(iMsgType, strMsg);
}
#else
void WriteLog( const int iMsgType, const char * strMsg)
{
	WriteLog1(iMsgType, strMsg);
}
#endif	

void CLogger::setParam(const char* name,char* path, int level){
	#ifdef WIN32
	strcpy(LOG_FILE_NAME,name);
	strcpy(SVCNAME,name);
	#else
	strcpy(LOG_FILE_NAME, name);
	//strcpy(SVCNAME, name);
	#endif
	char folder[256]; 
	sprintf_s( folder, sizeof(folder),"%s/", path);
	for( size_t i=1; i<strlen(folder); i++){ 
		if( folder[i]=='/' || folder[i]=='\\'){ 
			folder[i] = 0; 
			if(ACCESS(folder,0) != 0){
				if(MKDIR(folder)==-1){  
					perror("mkdir error");
					return;
				} 
			} 
			folder[i] = '/';  
		} 
	} 
	strncpy(LOG_FILE_PATH,folder,strlen(folder));
	m_traceLevel = level;
}

void CLogger::Log(const eLogType type, const char* lpszFormat, ...){
	if( m_traceLevel & type ){
		va_list args;
		char   szBuffer[2048] = {0};
		va_start(args, lpszFormat);
#ifdef WIN32
		vsnprintf_s(szBuffer,sizeof(szBuffer), lpszFormat, args);
#else
		vsnprintf(szBuffer, sizeof(szBuffer), lpszFormat, args);
#endif
		va_end(args);
		WriteLog((int)type, szBuffer);
	}
}

