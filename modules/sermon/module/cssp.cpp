#include "cssp.h"
#include "../kernel/Log.h"
#include "../kernel/datamgr.h"

//执行任务
void cssp::dotask(task * tsk) {
	switch (tsk->mark) {
	case fly_back:
		flyback( tsk );
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
void cssp::dowork() {
	if ( mystatus == NORMAL) {
		char buf[DATAMAXSIZE] = { 0 };
		int len = readdata(buf, sizeof(buf));
		if( len > 0 ){
			task tsk;
			tsk.datadir = dir_recv;
			tsk.trigger = this;
			tsk.mark = recv_data;
			tsk.datastamp = get_time_scale();
			tsk.retcode = 0;
			memcpy(tsk.buffer, buf, len);
			tsk.buflen = len;
			observer->addtask(&tsk);
		}
		return;
	}

	if (mystatus ==  INITIAL) {
		open(myparams);
		return;
	}

	if (mystatus == ABNORMAL) {
		open(myparams);
		return;
	}

}

//打开串口
bool cssp::open( char* param ){
	char name[100];
	char protocal[100];
	int  baudrate = 0;
	if( 3 != sscanf(param,"%[^,],%d,%s",name,&baudrate,protocal)){
		LOG(eParameterError,"analyze param:%s error!",param);
		return false;
	}
	mystatus = INITIAL;
	if( device.Open(name,baudrate,protocal,ctb::SerialPort::NoFlowControl)<0){
		LOG(eSoftError,"cssp::open %s open error!",name);
		return false;
	}
	mystatus = NORMAL;
	return true;
}

//关闭模块
bool cssp::close(){
	mystatus = REMOVE;
	stop();
	if( rcitem.size()>0){
		CRawDataDB dbfile;
		dbfile.SetDir( mypath );
		dbfile.Save( &rcitem );
		rcitem.clear();
	}
	device.Close();
	return true;
}

//重新设置串口参数
int cssp::setparam(char* args, int len){
	LOG(eTipMessage,"%s setparam:%s",myname,args);
	//原参数表，更新参数表
	char name[100]={0};
	char pros[100]={0};
	int  baudrate = 9600;
	sscanf(args,"%[^,],%d,%[^,]",name,&baudrate,pros);

	//重新打开串口
	if( mystatus == NORMAL ){
		device.Close();
		mystatus = INITIAL;
	}

	if( device.Open(name,baudrate,pros,ctb::SerialPort::NoFlowControl)<0){
		LOG(eSoftError,"cssp::setparam %s open error!",name);
		return -1;
	} 
	mystatus = NORMAL;
	return 0;
}

int cssp::readdata(char* data, int len) {
	int read = 0;
	int cnt = device.Read(data, len);
	if (cnt > 0) inports += cnt;
	read = cnt;
	while (cnt> 0 && read < MAXRECVSIZE) {
		Sleep(50);
		cnt = device.Read(&data[read], MAXRECVSIZE - read);
		if (cnt > 0) inports += cnt;
		read += cnt;
	}
	datamgr::saveframe( e_FrameIn, data, read);
	return read;
}

int cssp::writedata(char* data, int len) {
	int wroted = 0;
	do {
		wroted += device.Write(&data[wroted], len - wroted);
	} while (wroted < len);
	datamgr::saveframe(e_FrameOut, data, len); //wroted
	return wroted;
}

int cssp::flyback( task * tsk ) {
	task reptask(tsk);
	reptask.trigger = this;
	reptask.datadir = dir_send;
	reptask.datastamp = get_time_scale();
	reptask.mark = tsk->mark | send_data;
	writedata(tsk->buffer, tsk->buflen);
	//observer->addtask(&reptask);
	Sleep(tsk->delay);
	reptask.datadir = dir_recv;
	reptask.mark = tsk->mark | recv_data;
	reptask.buflen = readdata(reptask.buffer,sizeof(reptask.buffer));
	observer->addtask(&reptask);
	return 0; 
};
