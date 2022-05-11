#include "camera.h"
#include <vector>

//监控线程启动
int ccamera::play(bool store){
	m_status  = 0;
	myvideo = NULL;
	myimage = NULL;
	int camid = 0;
	int quality = 50;
	sscanf(m_source,"%d;%d",&camid,&quality);
	myparam.clear();
	myparam.push_back(CV_IMWRITE_JPEG_QUALITY);
	myparam.push_back(quality);
	m_playevt = event_create( false, false );
	m_frmevt  = event_create( false, true );
	mycapture = cvCreateCameraCapture( camid );	
	cvSetCaptureProperty(mycapture, CV_CAP_PROP_FRAME_WIDTH, 1280);
	cvSetCaptureProperty(mycapture, CV_CAP_PROP_FRAME_HEIGHT, 720);
	//myimage   = cvCreateImage();
#ifdef WIN32
	threadid  = (unsigned int)_beginthreadex(NULL,0,playthread,this,0,NULL);
#else
	if( pthread_create(&threadid, NULL, playthread, this) != 0) 
		printf("pthread_create failed! \n");
#endif
	return 0;
}

//停止
int ccamera::stop(){
	if(myvideo)cvReleaseVideoWriter(&myvideo);
	event_set(m_playevt);
	event_timedwait(m_playevt,10000);
	if(myimage) cvReleaseImage(&myimage);
	if(mycapture) cvReleaseCapture(&mycapture);
	mycapture = NULL;
	myvideo = NULL;
	return 0;
}

//接收处理线
winapi  ccamera::playthread(void* lpParam){
	ccamera * fpr = (ccamera*)lpParam;
	while(0!=event_timedwait(fpr->m_playevt,30)){
  		if( 0 == event_wait(fpr->m_frmevt)){
			event_reset(fpr->m_frmevt);
			fpr->myimage = cvQueryFrame(fpr->mycapture);
			cvFlip(fpr->myimage,NULL, 1);
			fpr->storage();
			event_set(fpr->m_frmevt);
		}
	}
	event_set( fpr->m_playevt );
	printf("pthread exist\r\n");
	return   0; 
}

//获取帧画面数据
size_t ccamera::getframe(ftype type, std::string & frm ){
	if(!myimage || 0!=event_wait( m_frmevt ) )
		return 0;
	event_reset(m_frmevt);
	switch( type ){
	case JPG: {
		//cv::Mat tempMat(myimage); //linux 无法编译通过
		cv::Mat tempMat = cv::cvarrToMat(myimage);
		static std::vector<unsigned char> buff;
		buff.clear();
		cv::imencode(".jpg", tempMat, buff, myparam);
		tempMat.release();
		frm.insert(frm.end(), buff.begin(), buff.end());
		break;}
	case BMP: break;
	default: break;
	}
	event_set(m_frmevt);
	return frm.size();
}

int ccamera::query(){
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
void ccamera::storage(){
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
