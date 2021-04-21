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
	int baudrate;	 //������
	int timeval; 	 //֡���(΢��)
	int port;        //�˿ں�
	std::string path;//�ļ�����·��

}Comcfg;
