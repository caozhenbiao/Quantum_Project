#include "xbase.h"
#include "../machine.h"
#include "../db/RawDataDB.h"
#include "./Log.h"

//xbase
xbase::xbase(){
	exitthread = true;
	observer = NULL;
	mydelay  = 1000;
	mystatus = 0;
	memset(myname, 0, sizeof(myname));
	memset(myparams, 0, sizeof(myparams));
	memset(myprotocal,0,sizeof(myprotocal));
	inports = 0;
	exports = 0;
}

//模块注入(设置模块相关逻辑参数)
void xbase::inject(const char * name, xbase * obsvr, const char* path){
	strncpy(myname, name, strlen(name));
	observer = obsvr;
	setpath(path);
}

//设置线程事件等待时间
void xbase::setdelay(unsigned int tms ){
	if (tms >= 10 && tms < 10000) {
		mydelay = tms;
	}
}

//启动线程(硬件或接口参数)
bool xbase::start(){
	exitthread = false;
	myevent  = event_create(false, false);
	myreccs  = event_create(false, true);
#ifdef WIN32
	threadid = _beginthreadex(NULL,0,workthread,this,0,NULL);
#else
    if( pthread_create(&threadid, NULL, workthread, this) != 0) 
		printf("pthread_create failed! \n");
#endif
	if( !myevent || !myreccs )
		return false;
	return true;
}

//关闭线程
void xbase::stop(){
	mystatus = -1;
	exitthread = true;
	event_set( myevent );
	event_set( myreccs );
#ifdef WIN32
	WaitForSingleObject((HANDLE)threadid, 1000);
#else
	pthread_join(threadid, NULL);
#endif
	event_destroy( myevent );
	event_destroy( myreccs );
	myevent=NULL;
	myreccs=NULL;
	printf ("%s thread exit.\n", myname); 
}

//添加任务
void xbase::addtask( task * tsk , bool wait ){
 	if( 0 == event_wait(myreccs) ){
		event_reset( myreccs );
		task newtask(tsk);
	    mytasks.push_back( newtask);
		event_set( myreccs );
		if(!wait) event_set( myevent );
	}
}

//工作线程
winapi xbase::workthread(void * argv){
	xbase * xm = (xbase *)argv;
	Sleep(1000);
	vector<task> worktasks;
	while( !xm->exitthread ){
		event_timedwait( xm->myevent, xm->mydelay);
		if( 0==event_wait( xm->myreccs )){
			event_reset( xm->myreccs );	
			std::swap( worktasks, xm->mytasks );
			event_set( xm->myreccs );
		}
		for( size_t i=0; i<worktasks.size(); i++ ){
			xm->dotask( &worktasks[i] );
		}
		worktasks.clear();
		xm->dowork();
	}
	return 0;
}

//这里都是执行任务, type:response;
void xbase::dotask(task * tsk) {
	int retcode = 0;
	switch (tsk->mark) {
	case send_data:
		retcode = (writedata(tsk->buffer, tsk->buflen)>0 ? 0 : -1);
		break;
	case set_parameter:
		retcode = setparam(tsk->buffer, tsk->buflen);
		strncpy(myparams, tsk->buffer, 256);
		break;
	case close_channel:
		retcode = 0;
		break;
	case read_histdata:
		requestHistoryData(tsk);
		return;
	default:
		return;
	}
	task reptask;
	reptask.trigger = this;
	reptask.mark = tsk->mark;
	reptask.retcode = retcode;
	reptask.datastamp = getTimeScale();
	observer->addtask(&reptask);
}
