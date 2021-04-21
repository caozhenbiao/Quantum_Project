#include "onvifpr.h"
#include <vector>

//监控线程启动
int convifpr::play(bool store){
	m_status  = 0;
	myvideo = NULL;
	myimage = NULL;
	char rtpurl[256];
	int quality = 50;
	sscanf(m_source,"%[^;];%d",&rtpurl,&quality);
	myparam.clear();
	myparam.push_back(CV_IMWRITE_JPEG_QUALITY);
	myparam.push_back(quality);
	m_playevt = event_create( false, false );
	m_frmevt  = event_create( false, true );
	mycapture = cvCaptureFromFile(rtpurl);
#ifdef WIN32
	threadid  = _beginthreadex(NULL,0,playthread,this,0,NULL);
#else
	if( pthread_create(&threadid, NULL, playthread, this) != 0) 
		printf("pthread_create failed! \n");
#endif
	return 0;
}

//停止
int convifpr::stop(){
	if(myvideo)cvReleaseVideoWriter(&myvideo);
	event_set(m_playevt);
	event_timedwait(m_playevt,10000);
	cvReleaseCapture(&mycapture);
	cvReleaseImage(&myimage);
	mycapture = NULL;
	myvideo = NULL;
	return 0;
}

//接收处理线
winapi convifpr::playthread(void* lpParam){
	convifpr * fpr = (convifpr*)lpParam;
	while(0!=event_timedwait(fpr->m_playevt,30)){
  		if( 0 == event_wait(fpr->m_frmevt)){
			event_reset(fpr->m_frmevt);
			fpr->myimage = cvQueryFrame(fpr->mycapture);
			event_set(fpr->m_frmevt);
		}
	}
	event_set( fpr->m_playevt );
	printf("pthread exist\r\n");
	return   0; 
}

//获取帧画面数据
size_t convifpr::getframe(ftype type, std::string & frm ){
	if( !myimage || 0!=event_wait(m_frmevt) )
		return 0;
	event_reset(m_frmevt);
	switch( type ){
	case JPG:{
		//cv::Mat tempMat(myimage);
		cv::Mat tempMat = cv::cvarrToMat(myimage);
		std::vector<unsigned char>  buff; 
		cv::imencode(".jpg",tempMat,buff,myparam);
		tempMat.release();
		frm.insert(frm.end(),buff.begin(),buff.end());
		break;}
	case BMP: break;
	default: break;
	}
	event_set(m_frmevt);
	return frm.size();
}

int convifpr::query(){
	cv::VideoCapture temp_camera;
	int maxTested = 10;
	for (int i = 0; i < maxTested; i++){
		cv::VideoCapture temp_camera(i);
		bool res = (!temp_camera.isOpened());
		temp_camera.release();
		if (res)
		{
			return i;
		}
	}
	return maxTested;
}

//视频存储
void convifpr::storage(){
	if( m_bsave && myvideo && myimage){
		cvWriteFrame(myvideo,myimage);
		return;
	}
	if( !m_bsave && myvideo ){
		cvReleaseVideoWriter(&myvideo);
		myvideo = NULL;
		return;
	}
	if( m_bsave && myimage ){
		char szavi[256]={0};
		sprintf( szavi, "../%s.avi", identify);
		myvideo = cvCreateVideoWriter(szavi,CV_FOURCC('M','P','4','3'),15,cvSize(myimage->width,myimage->height),1);
		return;
	}
	//如果过了一些时间
}
