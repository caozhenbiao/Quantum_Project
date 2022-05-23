#include "snapc.h"
#include <string>
#include <map>

//监控线程启动
bool csnapc::play(const char * args) {
	char ip[32] = { 0 };
	unsigned int port = 0;
	if (6 != sscanf(args, "%[^:]:%d;%d,%d,%d,%d", ip, &port, &myfrmleft, &myfrmtop, &myfrmwidth, &myfrmheight)){
		printf("error argument!\n");
		return  false;
	}
	mybConn = false;

#ifdef WIN32
	WSADATA wsdata;
	WSAStartup(0x0202, &wsdata);
#endif
	//创建SOCKET
	sockaddr_in myaddr;
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = inet_addr(ip);
	myaddr.sin_port = htons(port);
	myserver = socket(AF_INET, SOCK_STREAM, 0);

	//梆定
	int ibind = bind(myserver, (sockaddr*)&myaddr, sizeof(sockaddr_in));
	if (ibind != 0) {
		printf("websock bind socket error! addr=%s:%d\n", ip, port);
		return false;
	}
	//设置监听
	int ilisten = listen(myserver, 5);
	if (ilisten != 0) {
		printf("websock listen error!\n");
		return false;
	}

	myScreenDC = CreateDC("DISPLAY", NULL, NULL, NULL);
	myCompDC   = CreateCompatibleDC(myScreenDC);
	myimage    = cvCreateImage(cvSize(myfrmwidth, myfrmheight), IPL_DEPTH_8U, 4);
	mysnapevt  = event_create( false, true );
	myplayevt  = event_create(false, false);

#ifdef WIN32
	mythreadid  = _beginthreadex(NULL,0,playthread,this,0,NULL);
#else
	if( pthread_create(&mythreadid, NULL, playthread, this) != 0) 
		printf("pthread_create failed! \n");
#endif
	printf("snapc start work:%s\n", args);
	return true;
}

//停止
bool csnapc::stop(){
	DeleteDC(myScreenDC);
	DeleteDC(myCompDC);
	event_set(mysnapevt);
	Sleep(100);
	event_timedwait(mysnapevt,100000);
#ifdef WIN32
	closesocket(myserver);
	WSACleanup();
#else
	close(myserver);
#endif
	return true;
}

void csnapc::dowork(){
	if (mybConn) {
#ifdef _WIN32
		HBITMAP hBmp = CreateCompatibleBitmap(myScreenDC, myfrmwidth, myfrmheight);
		HBITMAP hOld = (HBITMAP)SelectObject(myCompDC, hBmp);
		//BitBlt(myCompDC, 0, 0, myfrmwidth, myfrmheight, myScreenDC, myfrmtop, myfrmleft, SRCCOPY);
		StretchBlt(myCompDC, 0, 0, myfrmwidth, myfrmheight, myScreenDC, myfrmleft, myfrmtop, myfrmwidth, myfrmheight, SRCCOPY);
		SelectObject(myCompDC, hOld);
		cv::Mat tempMat;
		//cv::Mat sizeMat;
		tempMat.create(cvSize(myfrmwidth, myfrmheight), CV_MAKETYPE(CV_8U, 4));
		GetBitmapBits(hBmp, myfrmwidth * myfrmheight * 4, tempMat.data);
		DeleteObject(hBmp);
		DeleteObject(hOld);

		//resize(tempMat, sizeMat, cvSize(640, 480), 0, 0);
		std::vector<int> param;
		param.push_back(CV_IMWRITE_JPEG_QUALITY);
		param.push_back(100);
		myimgbuf.clear();
		cv::imencode(".jpg", tempMat, myimgbuf, param);
		tempMat.release();
		//sizeMat.release();
		//FILE * fid = fopen("./txt_out.jpg", "wb");
		//fwrite(&myimgbuf[0], myimgbuf.size(), 1, fid);
		//fclose(fid);
#else
		int fd = open("/dev/fb0", O_RDONLY);
		if (fb < 0)
		{
		}
		unsigned char buf[1280 * 480 * 4];
		read(fb, buf, 1280 * 480 * 4);
		close(fb);
		std::vector<init> param;
		param.push_back(CV_IMWRITE_PNG_COMPRESSION);
		param.push_back(0);
		myimgbuf.clear();
		cv::imencode(".jpg", buf, myimgbuf, param);
		tempMat.release();
#endif // 0

		//开始发送
		unsigned int nsize = myimgbuf.size();
		if ( nsize< 1000 )
			return;
		if ( send(myclient, (char*)&nsize, 4, 0) < 4) {
			printf("close socket.\n");
			closesocket(myclient);
			myclient = -1;
			mybConn = false;
			return;
		}
		int ns = send(myclient, (char*)&myimgbuf[0], myimgbuf.size(), 0);
		if (ns == -1) {
			printf("close socket.\n");
			closesocket(myclient);
			myclient = -1;
			mybConn = false;
		}
	}else {
		sockaddr_in addrclt;
		bool bAccept = false;
		int  addlen = sizeof(addrclt);
		printf("wait accept.\n");
		myclient = accept(myserver, (struct sockaddr*)&addrclt, (socklen_t*)&addlen);
		printf("cliend accepted.\n");
		mybConn = true;
	}
}

//接收处理线
winapi  csnapc::playthread(void* lpParam){
	csnapc * fpr = (csnapc*)lpParam;
	while(0!=event_timedwait(fpr->myplayevt,30)){
		fpr->dowork();
	}
	event_set( fpr->myplayevt );
	printf("pthread exist\r\n");
	return   0; 
}