#ifndef _CROSS_DEFINE_H_
#define _CROSS_DEFINE_H_

#ifdef WIN32
#include <time.h>
#include <process.h>
#include <direct.h>
#include <io.h>
#define WIN32_LEAN_AND_MEAN  //这很重要，解决winsock.h winsock2.h冲突
#include <windows.h>
#define ACCESS _access   
#define MKDIR(a) _mkdir((a))   
#define winapi unsigned int __stdcall
#define pthread unsigned int
#define mssleep(x) Sleep(x)
#define localtime_x(x,y) localtime_s(x,y)
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
#include <sys/time.h>
#include <mntent.h>
#include <dirent.h>
#include <sys/types.h>
#define winapi void*
//#define SOCKET int
#define pthread pthread_t
#define localtime_x(x,y) localtime_r(y,x)
#endif

//URLDECODE
#include <string>
#include <vector>
std::string UrlDecode(std::string &src);
int split(const std::string& str, std::vector<std::string>& ret_, std::string sep = ",");


char StrToBin(char *str);
char CharToInt(char ch);


//字符处理
#ifdef WIN32
#include <lm.h>
void UTF_8ToUnicode(wchar_t* pOut,const char *pText);
void UnicodeToUTF_8(char* pOut,const wchar_t* pText); 
void UnicodeToGB2312(char* pOut,wchar_t uData);
void Gb2312ToUnicode(wchar_t* pOut,char *gbBuffer);
void GB2312ToUTF_8(std::string& pOut,char *pText, int pLen); 
void UTF_8ToGB2312(char*pOut, const char *pText, int pLen);


std::string UrlGB2312Decode(std::string str);
std::string UrlUTF8Decode(std::string str);

std::string utf2gb( const char* data );
std::string gb2utf( char* data);
#else
#include <iconv.h>
#include <string.h>
#define closesocket close
int code_convert(char *from_charset,char *to_charset,char *inbuf,size_t inlen,char *outbuf,size_t outlen);
int u2g(char *inbuf,size_t inlen,char *outbuf,size_t outlen);
int g2u(char *inbuf,size_t inlen,char *outbuf,size_t outlen);
#endif
#endif 

