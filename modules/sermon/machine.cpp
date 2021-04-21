#include "machine.h"
#include "kernel/taskdef.h"
#include "kernel/Log.h"
#include "module/cssp.h"
#include "module/cdsp.h"
#include "module/csnp.h"
#include "module/icmp.h"

extern void responsetolua(std::string name, task * tsk);

//����������,��ʼ����Ӧģ��
void machine::start(){
	char strace[256] = { 0 };
	gettracepath(strace, 256);
	int nlevel = gettracelevel();
	CLogger::createInstance()->setParam("sermon",strace,nlevel);
	diskscan.start();
	setdelay(60 * 1000);
	xbase::start();
}

//��¼�ǹر�,���ģ��رչ���������
void machine::stop(){
	task newtask;	
	memset( &newtask,0,sizeof(task));
	newtask.direct = true;
	newtask.mark = close_channel;
	for(map<string,xbase* >::iterator it=mymodules.begin();it!=mymodules.end();it++){
		newtask.trigger = this;
		it->second->addtask(&newtask);
	}
	diskscan.stop();
	xbase::stop();
}

//������ͬ�Ĺ���ģ��
xbase * machine::createmodule(const char* name,const char* path, int ht){
	LOG(eTipBusiness,"createmodule name:%s path:%s type:%d",name,path,ht);
	xbase * newmdl = NULL;
	switch( ht ){
	case SSP:   newmdl = new cssp(); break;
	case DSP:   newmdl = new cdsp(); break;
	case SNP:   newmdl = new csnp(); break;
	//case UDP:   newmdl = new cudp; break;
	//case TCPS:  newmdl = new tcps; break;
	//case TCPC:  newmdl = new tcpc; break;
	//case GPIO:  newmdl = new gpio; break;
	//case XSP:   newmdl = new cxsp; break;
	default:  return NULL;
	}
	newmdl->inject(name,this,path);
	newmdl->start();
	mymodules.insert(make_pair(name, newmdl));
	return newmdl;
}

bool  machine::removemodule(const char* name) {
	map<string, xbase*>::iterator ifnd = mymodules.find(name);
	if (ifnd != mymodules.end()) {
		xbase* mod = ifnd->second;
		task newtask;
		memset(&newtask, 0, sizeof(task));
		newtask.trigger = NULL;
		newtask.mark = close_channel;
		mod->addtask(&newtask);
		mymodules.erase(name);
	}
	return true;
}

//��ȡ�����豸
xbase * machine::getmodulebyname(const char* name){
	map<string, xbase*>::iterator ifnd = mymodules.find( name );
	if( ifnd != mymodules.end() )
		return ifnd->second;
	return NULL;
}

//��ȡģ������
std::string machine::getmodulename(xbase* dev) {
	return "";
}

void machine::transhistorydata(const char * name, const char* timestamp){
	xbase * xptr = getmodulebyname(name);
	if ( !xptr)
		return;
	task newtask;
	memset(&newtask, 0, sizeof(task));
	newtask.mark = read_histdata;
	if( timestamp ) strcpy(newtask.buffer, timestamp);
	newtask.trigger = xptr->observer;
	xptr->addtask(&newtask);
	printf("transhistorydata command");
}

//�������
void machine::addtask(const char* name, task * tsk, bool wait) {
	xbase* mod = getmodulebyname(name);
	if (mod) {
		mod->addtask(tsk, wait);
	}
}

//ִ������
void machine::dotask(task * tsk) {
	for (map<string, xbase*>::iterator it = mymodules.begin(); it != mymodules.end(); it++) {
		if (it->second == tsk->trigger) {
			responsetolua(it->first, tsk);
			break;
		}
	}
}

//ģ��ѭ���������������״״����
void machine::dowork() {
 
}


//PING
int machine::ping(const char* ip, int bfs, int timeout, int& usedtime) {
	CICMP icmp;
	icmp.m_timeout = timeout;
	icmp.m_bufsize = bfs;
	int ret = icmp.doPing(ip);
	printf("icmp doping\n");
	usedtime = icmp.m_usedtime;
	return ret;
}