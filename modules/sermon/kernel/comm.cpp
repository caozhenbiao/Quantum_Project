#include "comm.h"

//�򿪴���
int comm::open( char* param ){
	char name[100];
	char protocal[100];
	int  baudrate = 0;
	if( 3 != sscanf(param,"%[^,],%d,%s",name,&baudrate,protocal))
		return -2;
	if( isopen ) close();
	if( device.Open(name,baudrate,protocal,ctb::SerialPort::NoFlowControl)<0)
		return -1;
	printf("comm:%s is open\n", name);
	isopen = true;
	return 0;
}

//�ر�ģ��
int comm::close(){
	device.Close();
	isopen = false;
	return 0;
}

int comm::read(char* data, int len) {
	if (!isopen)
		return -1;
	int cnt = device.Read(data, len);
	printf("read:%s\n", data);
	return cnt;
}

int comm::write(char* data, int len) {
	if (!isopen)
		return -1;
	int n = device.Write(data, len);
	return n;
}
