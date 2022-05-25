//±¾»úÉãÏñÍ·
#ifndef _CCAMERA_H_
#define _CCAMERA_H_
#include "cammgr.h"
#include <opencv2/highgui/highgui.hpp>
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
	std::vector<int> myparam;
	cv::VideoCapture myVideoCapture;
	cv::VideoWriter myVideoWriter;
	cv::Mat   myMat;
private:
	void storage();
};
#endif
