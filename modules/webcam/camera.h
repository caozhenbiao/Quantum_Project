//±¾»úÉãÏñÍ·
#ifndef _CCAMERA_H_
#define _CCAMERA_H_
#include "cammgr.h"
#include <opencv2/opencv.hpp>
class mbase;
class ccamera : public mbase{
public:
	virtual int play(bool store);
	virtual int stop();
	virtual size_t getframe(ftype type, std::string& frm);
	static int query();
private:
	static winapi playthread(void* lpParam);
	pthread_t threadid;
	bool m_bExit;
private:
	IplImage*  myimage;   
	CvCapture* mycapture;
	CvVideoWriter *myvideo;
	std::vector<int> myparam; 
private:
	void storage();
};
#endif
