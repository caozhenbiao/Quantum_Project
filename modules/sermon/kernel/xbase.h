#ifndef _XBASE_H_
#define _XBASE_H_
#include "taskdef.h"
#include "datamgr.h"
#include "../define.h"
#include "../event.h"
#include <vector>

//ģ��������
class xbase : public datamgr {
public:
	xbase();
	virtual ~xbase(){};
protected:
	virtual void dotask(task * tsk);
	virtual void dowork() = 0;
	virtual int setparam(char* args, int len) { return 0; };
	virtual int readdata(char* data, int len) { return 0; };
	virtual int writedata(char* data, int len){ return 0; };

public:
	void addtask(task * tsk ,bool wait=false);   //�������
	void inject(const char * name, xbase * obsvr, const char* path);//ע��ģ��
	void setdelay(unsigned int tms);             //��ʱ����
	bool start();                                //����
	void stop();                                 //ֹͣ  

public:
	std::vector<task> mytasks;     //�����б�
	xbase *           observer;

public:
	char          myname[100];     //�߼�ģ������
	char          myparams[256];   //ģ���������
	char          myprotocal[100]; //ͨ�Ź�Լ
	unsigned int  mystatus;        //��ģ��״̬�����4����ģ��
	unsigned int  mydelay;         //�ӳ�ʱ��
	char          myclass[100];

private:
    static winapi workthread(void * argv);
	pthread		 threadid; 
	bool         exitthread;
	event_handle myevent;
	event_handle myreccs;
};
#endif
