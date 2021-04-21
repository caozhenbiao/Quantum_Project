#include "cssp.h"

//打开串口
bool cssp::open(const char* param ){
	char name[100];
	char protocal[100];
	int  baudrate = 0;
	if( 3 != sscanf(param,"%[^,],%d,%s",name,&baudrate,protocal)){                             
		return false;
	}
	if( device.Open(name,baudrate,protocal,ctb::SerialPort::NoFlowControl)<0){
		return false;
	}
	return true;
}

//关闭模块
bool cssp::close(){
	device.Close();
	return true;
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
	return read;
}

int cssp::writedata(const char* data, int len) {
	int wroted = 0;
	do {
		wroted += device.Write((char*)&data[wroted], len - wroted);
	} while (wroted < len);
	return wroted;
}