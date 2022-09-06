#include "onvifpr.h"
#include <vector>

//监控线程启动
int convifpr::play(bool store) {
	m_bExit = false;
	m_status = 0;
	char rtpurl[256] = {0};
	int quality = 50;
	sscanf(m_source, "%[^;];%d", &rtpurl, &quality);
	myparam.clear();
	myparam.push_back(CV_IMWRITE_JPEG_QUALITY);
	myparam.push_back(quality);
	m_playevt = event_create(false, false);
	m_frmevt = event_create(false, true);

	myVideoCapture.release();


	if (!myVideoCapture.open(rtpurl)) {
		printf("cvCaptureFromFile open Url:%s Fail!\n", rtpurl);
		return -1;
	}
	if (store) {
		myVideoWriter.open("cammer_video.mpg", CV_FOURCC('D', 'I', 'V', 'X'), 30.0, cv::Size(640, 480));
	}
#ifdef WIN32
	threadid = (unsigned int)_beginthreadex(NULL, 0, playthread, this, 0, NULL);
#else
	if (pthread_create(&threadid, NULL, playthread, this) != 0)
		printf("pthread_create failed! \n");
#endif
	return 0;
}

//停止
int convifpr::stop() {
	m_bExit = true;
	event_set(m_playevt);
	event_set(m_frmevt);
	event_timedwait(m_playevt, 100000);
	event_timedwait(m_frmevt, 100000);
	myVideoCapture.release();
	myVideoWriter.release();
	myMat.release();
	return 0;
}

//接收处理线
winapi  convifpr::playthread(void* lpParam) {
	convifpr * fpr = (convifpr*)lpParam;
	while (!fpr->m_bExit && 0 != event_timedwait(fpr->m_playevt, 30)) {
		if (0 == event_wait(fpr->m_frmevt)) {
			event_reset(fpr->m_frmevt);
			if (fpr->myVideoCapture.isOpened()) {
				fpr->myVideoCapture.read(fpr->myMat);
				fpr->storage();
			}
			event_set(fpr->m_frmevt);
		}
	}
	event_set(fpr->m_playevt);
	printf("pthread exist\r\n");
	return   0;
}

//获取帧画面数据
size_t convifpr::getframe(ftype type, std::string & frm) {
	if (!myVideoCapture.isOpened() || 0 != event_wait(m_frmevt))
		return 0;
	event_reset(m_frmevt);
	switch (type) {
	case JPG: {
		static std::vector<unsigned char> buff;
		buff.clear();
		cv::imencode(".jpg", myMat, buff, myparam);
		frm.insert(frm.end(), buff.begin(), buff.end());
		break; }
	case BMP: break;
	default: break;
	}
	event_set(m_frmevt);
	return frm.size();
}

int convifpr::query() {
	cv::VideoCapture temp_camera;
	int maxTested = 10;
	for (int i = 0; i < maxTested; i++) {
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
void convifpr::storage() {
	if (myVideoWriter.isOpened()) {
		myVideoWriter.write(myMat);
	}
}
