#pragma once
#define TIMEOUT	  1
#define FRAMELEN  1024
#define ULONGLONG_ unsigned long long 
typedef struct
{
	int dir;
	long long timeScale;
	int size;
	char buf[FRAMELEN];
	
}Rec;

typedef struct
{
	std::string devname; 
	std::string protocol;
	int    baudrate;
	int    timeout;

}ComParam;

typedef struct
{
	std::string devnameA; 
	std::string devnameB; 
	int dirA;
	int dirB;
	int baudrate;	 //波特率
	int timeval; 	 //帧间隔(微秒)
	int port;        //端口号
	std::string path;//文件保存路径

}Comcfg;
