#include "taskdef.h"
#include "../define.h"
#define CONFIGPATH "./device.ini"

//LINUX 下ini文件编写函数
#ifndef WIN32
int GetPrivateProfileString(const char *AppName, const char *key, const char *defaultvalue, char *lpReturnedString, const int nSize, const char *lpFileName)
{
	bool bFindAppName = false;    
	char tem[1000];
	char *ptr;
	FILE *fp;
	char name[150];
	snprintf(name, sizeof(name), "[%s]", AppName);
	strncpy(lpReturnedString,defaultvalue, nSize);
	if( (lpFileName == NULL) || ((fp=fopen(lpFileName,"rt"))==NULL) )// || fgets(tem,len,fp) == NULL)
		return strlen(lpReturnedString);
	while(fgets(tem,sizeof(tem),fp)){   
		if(tem[0] == '['){
			bFindAppName = false;
			if(strstr(tem,name)==tem)
				bFindAppName = true;
		}else{
			if(bFindAppName == true){
				unsigned int n =strcspn(tem,"=");
				if((strlen(key) == n) && (strncasecmp(tem,key,n) == 0)){
					strncpy(lpReturnedString, tem+n+1,nSize);
					if( (ptr = strchr(lpReturnedString, '\n')) != NULL)
						*ptr = '\0';
					if( (ptr = strchr(lpReturnedString, '\r')) != NULL)
						*ptr = '\0';
					break;
				}
			}
		}
	}
	fclose(fp);
	return strlen(lpReturnedString);
}

int GetPrivateProfileInt(const char *AppName, const char *key, int defaultvalue, const char *lpFileName)
{
	char sztext[256] = {0};
	GetPrivateProfileString(AppName,key, "30",sztext, 256, lpFileName);
	return atoi( sztext );
}

#endif


int getdevicename(char * name, int len){
	return GetPrivateProfileString("device","name", "device",name, len, CONFIGPATH);
}

int gettracepath( char* path, int len ){
	return GetPrivateProfileString("device","tracepath", "./logs/",path,len, CONFIGPATH);
} 

int gettracelevel(){
	return GetPrivateProfileInt("device","tracelevel",255, CONFIGPATH);
}

int getkeeptimes(){
	return GetPrivateProfileInt("device","keeptime",30, CONFIGPATH);
} 

char hex2asc(char* shex){
	int mid = 0;
	int idec = 0;
	size_t nlen = strlen(shex);
	if( nlen > 2 )
		return -1;
	for( size_t i = 0; i< nlen; i++ ){
		if( shex[i] >='0' && shex[i]<='9' ) mid = shex[i]-'0';
		else if( shex[i] >='a' && shex[i]<='f' ) mid = shex[i]-'a'+10;
		else if( shex[i] >='A' && shex[i]<='F' ) mid = shex[i]-'A'+10;
		else
			return -1;
		mid <<= ((nlen-i-1)<<2);
		idec |= mid;
	}
	return (char)idec;
}

int bin2hex(unsigned char *src, int slen, char * dst, int dlen ){
	if (slen > (dlen / 3))
		return 0;
	int retlen = 0;
	for (int i = 0; i < slen; i++) {
		retlen += sprintf(&dst[i * 3], "%02X ", (unsigned char)src[i]);
	}
	return retlen;
}

long long get_time_scale() {
	static long long time;
#ifdef WIN32
	SYSTEMTIME t;
	struct tm tm1;
	GetLocalTime(&t);
	tm1.tm_year = t.wYear - 1900;
	tm1.tm_mon = t.wMonth - 1;
	tm1.tm_mday = t.wDay;
	tm1.tm_hour = t.wHour;
	tm1.tm_min = t.wMinute;
	tm1.tm_sec = t.wSecond;
	tm1.tm_isdst = -1;
	time = mktime(&tm1) * 10000000 + t.wMilliseconds * 10000 + BASETIME;
#else
	struct	timeval    m_tv;
	gettimeofday(&m_tv, NULL);
	time = m_tv.tv_sec;
	time *= 10000000;
	time += (m_tv.tv_usec * 10);
	time += BASETIME;
#endif
	return time;
}
