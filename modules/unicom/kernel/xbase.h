#ifndef _XBASE_H_
#define _XBASE_H_
#include "../define.h"
#include <vector>

//ģ��������
class xbase {
public:
	virtual int writedata(const char* data, int len)=0;
	virtual int readdata(char* data, int len)=0;
	virtual bool open(const char* param)=0;
	virtual bool close()=0;
public:
	char          myname[100];     //�߼�ģ������
	char          myparams[256];   //ģ���������
	char          myprotocal[100]; //ͨ�Ź�Լ
	unsigned int  mystatus;        //��ģ��״̬�����4����ģ��
	unsigned int  mydelay;         //�ӳ�ʱ��
	char          myclass[100];
	int          inports;
	int          exports;
};
#endif
