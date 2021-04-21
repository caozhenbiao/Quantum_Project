#include "cdsp.h"
#include "../kernel/Log.h"

//执行任务
void cdsp::dotask(task * tsk) {
	switch (tsk->mark) {
	case fly_back:
		flyback(tsk);
		return;
	case close_channel:
		close();
		return;
	default:
		break;
	}
	xbase::dotask(tsk);
}

//模块循环工作，如监听，状状检测等
void cdsp::dowork() {
	unsigned char * status = (unsigned char*)&mystatus;
	if (status[0] == NORMAL) {
		char buf0[DATAMAXSIZE] = { 0 };
		int len0 = readdata(buf0, sizeof(buf0));
		if (len0 > 0) {
			task tsk;
			tsk.trigger = this;
			tsk.mark = recv_data;
			tsk.datastamp = get_time_scale();
			tsk.datadir = 1;
			tsk.retcode = 0;
			memcpy(tsk.buffer, buf0, len0);
			tsk.buflen = len0;
			observer->addtask(&tsk);
		}
	}

	if (status[1] == NORMAL) {
		char buf1[DATAMAXSIZE] = { 0 };
		int len1 = writedata(buf1, sizeof(buf1));
		if ( len1 > 0) {
			task tsk;
			tsk.trigger = this;
			tsk.mark = recv_data;
			tsk.datastamp = get_time_scale();
			tsk.datadir = 2;
			tsk.retcode = 0;
			memcpy(tsk.buffer, buf1, len1);
			tsk.buflen = len1;
			observer->addtask(&tsk);
		}
	}

	if (status[0] == INITIAL || status[1] == INITIAL) {
		open(myparams);
		return;
	}

	if (status[0] == ABNORMAL || status[1] == ABNORMAL) {
		open(myparams);
		return;
	}
}

bool cdsp::open( char* param ){
	char args[2][256]={{0},{0}};
	if( 2 != sscanf( param, "%[^;];%s", args[0], args[1]) ){
		LOG(eParameterError,"analyze param:%s error!",param);
		return false;
	}
	unsigned char * status = (unsigned char*)&mystatus;
	for( unsigned int i = 0; i < 2; i++ ){
		if ( status[i] == NORMAL )
			continue;
		char name[100], pros[100];
		int  baudrate = 0;
		if( 3 != sscanf(args[i],"%[^,],%d,%s",name,&baudrate,pros)){
			LOG(eParameterError,"analyze param:%s error!",args[i]);
			continue;
		}
		status[i] = INITIAL;
		if( device[i].Open(name,baudrate,pros,ctb::SerialPort::NoFlowControl)<0){
			LOG(eSoftError,"cdsp::open %s open error!",name);
			continue;
		}
		status[i] = NORMAL;
	}
	return true;
}

int cdsp::setparam(char* args, int len){
	LOG(eTipMessage,"%s setparam:%s",myname,args);
	bool bFix = false;
	char pars[2][256]={{0},{0}};
	char argv[2][256]={{0},{0}};
	if( 2!=sscanf(args,"%[^;];%s",argv[0],argv[1])){
		LOG(eParameterError,"analyze param error!");
		return -1;
	}
	unsigned char * status = (unsigned char*)&mystatus;
	for( int i=0; i<2  && strcmp(argv[i],"-")!=0; i++ ){
		char name[2][100]={"","-"};
		char pros[2][100]={"","-"};
		int  baudrate[2]={0,-1};
		sscanf(pars[i],"%[^,],%d,%[^,]",name[0],&baudrate[0],pros[0]);
		sscanf(argv[i],"%[^,],%d,%[^,]",name[1],&baudrate[1],pros[1]);
		//比较数据
		bool bOpen = false;
		bool bParam = false;
		if(strcmp(name[1],"-")!=0 && strcmp(name[0],name[1])!=0){strcpy(name[0],name[1]);bOpen=true;}
		if(strcmp(pros[1],"-")!=0 && strcmp(pros[0],pros[1])!=0){strcpy(pros[0],pros[1]);bParam=true;}
		if(baudrate[1]!=-1 && baudrate[0]!=baudrate[1] ){baudrate[0]=baudrate[1];bParam = true;	}
		//重新打开串口
		if( bOpen || status[i]!=NORMAL ){
			if(status[i] == NORMAL ){
				device[i].Close();
				status[0] = INITIAL;
			}
			if( device[i].Open(name[0],baudrate[0],pros[0],ctb::SerialPort::NoFlowControl)<0){
				LOG(eSoftError,"cdsp::setparam %s open error!",name[0]);
				continue;
			}
			status[0] = NORMAL;
		}
		//通信参数设置
		else if( bParam ){
			char params[256];
			sprintf_s(params,sizeof(params),"%d,%s",baudrate[0],pros[0]);
			if( -1 == device[i].setserialparam(params,strlen(params)) ){
				LOG(eSoftError,"%s setserialparam:%s error!",myname,args);
				continue;
			}
		}
		//构建参数
		memset(pars[i],0,sizeof(pars[i]));
		sprintf(pars[i],"%s,%s",name[i],device[i].getserialparam());
		status[i] = NORMAL;
		bFix = true;
	}
	//更新参数
	if( bFix ){
		sprintf(myparams,"%s;%s", pars[0],pars[1]);
	}
	return 0;
}

bool cdsp::close(){
	unsigned char * status = (unsigned char*)&mystatus;
	status[0] = REMOVE;
	status[1] = REMOVE;
	stop();
	device[0].Close();
	device[1].Close();
	return true;
}

//读取数据,并记录到DB
int cdsp::readdata( char* data, int len ){
	int read = 0;
	int cnt = device[0].Read(data, len);
	if (cnt > 0) inports += cnt;
	read = cnt;
	while (cnt> 0 && read < MAXRECVSIZE) {
		Sleep(50);
		cnt = device[0].Read(&data[read], MAXRECVSIZE - read);
		if (cnt > 0) inports += cnt;
		read += cnt;
	}
	return read;
}

int cdsp::writedata( char* data, int len ){
	int wroted = 0;
	int cnt = device[1].Read(data, len);
	if (cnt > 0) exports += cnt;
	wroted = cnt;
	while (cnt> 0 && wroted < MAXRECVSIZE) {
		Sleep(50);
		cnt = device[1].Read(&data[wroted], MAXRECVSIZE - wroted);
		if (cnt > 0) exports += cnt;
		wroted += cnt;
	}
	return wroted;
}

int cdsp::flyback(task * tsk) {
	task reptask(tsk);
	reptask.trigger = this;
	reptask.mark = tsk->mark;
	reptask.datastamp = get_time_scale();
	writedata(tsk->buffer, tsk->buflen);
	Sleep(tsk->delay);
	reptask.buflen = readdata(reptask.buffer, sizeof(reptask.buffer));
	observer->addtask(&reptask);
	return 0;
};