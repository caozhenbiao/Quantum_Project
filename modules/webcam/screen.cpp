#include "screen.h"
#include <string>
#include <map>

//监控线程启动
int cscreen::play(bool store){
	printf("screen play\n");
	sscanf(m_source, "%d,%d,%d,%d", &myfrmleft,&myfrmtop,&myfrmwidth,&myfrmheight);
#ifdef _WIN32
	myScreenDC = CreateDC("DISPLAY", NULL, NULL, NULL);
	myCompDC = CreateCompatibleDC(myScreenDC);
#endif
	myimage = cvCreateImage(cvSize(myfrmwidth, myfrmheight), IPL_DEPTH_8U, 4);
	m_playevt = event_create( false, false );
	m_frmevt  = event_create( false, true );
#ifdef WIN32
	threadid  = (unsigned int)_beginthreadex(NULL,0,playthread,this,0,NULL);
#else
	if( pthread_create(&threadid, NULL, playthread, this) != 0) 
		printf("pthread_create failed! \n");
#endif
	return 0;
}

//停止
int cscreen::stop(){
#ifdef _WIN32
	DeleteDC(myScreenDC);
	DeleteDC(myCompDC);
#endif
	if (myimage) cvReleaseImage(&myimage);
	event_set(m_playevt);
	event_set(m_frmevt);
	Sleep(100);
	event_timedwait(m_playevt,10000);
	return 0;
}

//获取帧画面数据
size_t cscreen::getframe(ftype type, std::string & frm ){
	if( 0 != event_wait( m_frmevt ) )
		return 0;
	event_reset(m_frmevt);
	switch( type ){
	case JPG: frm.append((char*)&myimgbuf[0], myimgbuf.size());break;
	case BMP: break;
	default: break;
	}
	event_set(m_frmevt);
	return frm.size();
}

void cscreen::work(){

	return;
}

//接收处理线
winapi  cscreen::playthread(void* lpParam){
	cscreen * sc = (cscreen*)lpParam;
	while(0!=event_timedwait(sc->m_playevt,30)){
		event_reset(sc->m_frmevt);
#ifdef _WIN32
		HBITMAP hBmp = CreateCompatibleBitmap(sc->myScreenDC, sc->myfrmwidth, sc->myfrmheight);
		HBITMAP hOld = (HBITMAP)SelectObject(sc->myCompDC, hBmp);
		//BitBlt(myCompDC, 0, 0, myfrmwidth, myfrmheight, myScreenDC, myfrmtop, myfrmleft, SRCCOPY);
		StretchBlt(sc->myCompDC, 0, 0, sc->myfrmwidth, sc->myfrmheight, sc->myScreenDC, sc->myfrmleft, sc->myfrmtop, sc->myfrmwidth, sc->myfrmheight, SRCCOPY);
		SelectObject(sc->myCompDC, hOld);
		cv::Mat tempMat;
		tempMat.create(cvSize(sc->myfrmwidth, sc->myfrmheight), CV_MAKETYPE(CV_8U, 4));
		GetBitmapBits(hBmp, sc->myfrmwidth * sc->myfrmheight * 4, tempMat.data);
		DeleteObject(hBmp);
		DeleteObject(hOld);
		//resize(tempMat, sizeMat, cvSize(640, 480), 0, 0);
		std::vector<int> param;
		param.push_back(CV_IMWRITE_JPEG_QUALITY);
		param.push_back(100);
		sc->myimgbuf.clear();
		cv::imencode(".jpg", tempMat, sc->myimgbuf, param);
		tempMat.release();
#else
		int fb = open("/dev/fb0", O_RDONLY);
		printf("open screen:%d\n", fb);
		if ( fb > 0 ){
			unsigned int bufsize = sc->myfrmwidth * sc->myfrmheight * 4;
			static unsigned char* buf = (unsigned char*)malloc(bufsize);
			memset(buf, 0x00, bufsize);
			read(fb, buf, bufsize);
			close(fb);
			std::vector<int> param;
			param.push_back(CV_IMWRITE_PNG_COMPRESSION);
			param.push_back(100);
			cv::Mat tempMat;
			tempMat.create(cvSize(sc->myfrmwidth, sc->myfrmheight), CV_MAKETYPE(CV_8U, 4));
			tempMat.data = buf;
			sc->myimgbuf.clear();
			//imwrite("./xxx.png", tempMat, param);
			cv::imencode(".jpg", tempMat, sc->myimgbuf, param);	
			tempMat.release();
		}
#endif
			event_set(sc->m_frmevt);
	}
	event_set( sc->m_playevt );
	printf("pthread exist\n");
	return   0; 
}
