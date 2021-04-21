#ifndef _OPENCV_H_
#define _OPENCV_H_
#include <vector>
#include <opencv2/opencv.hpp>

//opencv
class copencv{
public:
	int recognition(const char* haar,const char* file,std::vector<CvRect>& v);
	int eyereg(IplImage* pSrc, std::vector<CvRect> v );
public:
	int open(const char* haar, int min, int max);
	int detect(const char* file, std::vector<CvRect>& v);
	int detect(char* buf, std::vector<CvRect>& v);
	int detect(char* buf, char* photo );
	int detect(char* buf, char* photo, std::vector<CvRect>& v);
	int close();
private:

};
#endif