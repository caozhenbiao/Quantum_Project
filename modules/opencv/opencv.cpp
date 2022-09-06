#include "opencv.h"
#include <cstdio>
#include <cstdlib>
#include <Windows.h>


CvHaarClassifierCascade* myHaarCascade = NULL;
CvMemStorage* myMStorage = NULL;
CvFont m_font;
int   m_minSize = 50;
int   m_maxSize = 300;

//�򿪶�����
int copencv::open(const char* haar, int min, int max){
	myHaarCascade = NULL;
	cvReleaseHaarClassifierCascade(&myHaarCascade);
	myHaarCascade = (CvHaarClassifierCascade*)cvLoad( haar );
	myMStorage = cvCreateMemStorage(0);
	cvInitFont(&m_font, CV_FONT_HERSHEY_PLAIN, 1.5f, 1.5f, 0, 2, CV_AA);//������ʾ������  
	m_minSize = min;
	m_maxSize = max;
	return 0;
}

//������
int copencv::detect(const char* file, std::vector<CvRect>& v){
	IplImage *pSrcImage  = cvLoadImage( file , CV_LOAD_IMAGE_UNCHANGED);
	if( !pSrcImage )
		return -1;
	IplImage *pGrayImage = cvCreateImage(cvGetSize(pSrcImage), IPL_DEPTH_8U, 1);
	cvCvtColor(pSrcImage, pGrayImage, CV_BGR2GRAY);
	cvClearMemStorage(myMStorage);
	//����ʶ��
	CvSeq *pcvSeqFaces = cvHaarDetectObjects(pGrayImage, myHaarCascade, myMStorage);
	for(int i = 0; i <pcvSeqFaces->total; i++){
		CvRect* r = (CvRect*)cvGetSeqElem(pcvSeqFaces, i);
		v.push_back( *r );
	}
	cvReleaseImage(&pSrcImage);	
	cvReleaseImage(&pGrayImage);
	return (int)v.size();
}

//������
int copencv::detect(char* buf, std::vector<CvRect>& v){      
	CvMat mat = cvMat(640,480,CV_8UC1, buf);                 
	IplImage *pSrcImage = cvDecodeImage( &mat, 1 );
	if( !pSrcImage )
		return -1;
	IplImage *pGrayImage = cvCreateImage(cvGetSize(pSrcImage), IPL_DEPTH_8U, 1);
	cvCvtColor(pSrcImage, pGrayImage, CV_BGR2GRAY);
	cvClearMemStorage(myMStorage);
	//����ʶ��
	CvSeq *pcvSeqFaces = cvHaarDetectObjects(pGrayImage, myHaarCascade, myMStorage);
	for(int i = 0; i <pcvSeqFaces->total; i++){
		CvRect* r = (CvRect*)cvGetSeqElem(pcvSeqFaces, i);
		v.push_back( *r );
	}
	cvReleaseImage(&pSrcImage);	
	cvReleaseImage(&pGrayImage);
	return (int)v.size();
}

//֡�������м�������
int copencv::detect( char* buf, char* photo ){     
	CvMat mat = cvMat(640,480,CV_8UC1, buf);                 
	IplImage *pSrcImage = cvDecodeImage( &mat, 1 );
	if( !pSrcImage )
		return -1;
	IplImage *pGrayImage = cvCreateImage(cvGetSize(pSrcImage), IPL_DEPTH_8U, 1);
	cvCvtColor(pSrcImage, pGrayImage, CV_BGR2GRAY);
	cvClearMemStorage(myMStorage);
	//����ʶ��
	CvSeq *pcvSeqFaces = cvHaarDetectObjects(pGrayImage, myHaarCascade, myMStorage,1.1,2,0,cvSize(m_minSize,m_minSize),cvSize(m_maxSize,m_maxSize));
	for(int i = 0; i <pcvSeqFaces->total; i++){
		CvRect * r = (CvRect*)cvGetSeqElem(pcvSeqFaces, i);
		CvPoint P1,P2;  
		P1.x=r->x;  
		P1.y=r->y;  
		P2.x=r->x+r->width;  
		P2.y=r->y+r->height;
		cvRectangle(pSrcImage,P1 ,P2, cvScalar(0, 255, 0), 2);
		cvPutText(pSrcImage,"face", cvPoint(P1.x, P1.y-10), &m_font, cvScalar(255, 0, 0));
	}
	if( photo && pcvSeqFaces->total>0){
		cvSaveImage(photo,pSrcImage);
	}
	cvReleaseImage(&pSrcImage);	
	cvReleaseImage(&pGrayImage);
	return pcvSeqFaces->total;
}

//������
//ͼ������
int copencv::detect(char* buf, char* photo, std::vector<CvRect>& v){    
	CvMat mat = cvMat(640,480,CV_8UC1, buf);                 
	IplImage *pSrcImage = cvDecodeImage( &mat, 1 );
	if( !pSrcImage )
		return -1;
	IplImage *pGrayImage = cvCreateImage(cvGetSize(pSrcImage), IPL_DEPTH_8U, 1);
	cvCvtColor(pSrcImage, pGrayImage, CV_BGR2GRAY);
	cvClearMemStorage(myMStorage);
	//����ʶ��
	CvSeq *pcvSeqFaces = cvHaarDetectObjects(pGrayImage, myHaarCascade, myMStorage,1.1,2,0,cvSize(m_minSize,m_minSize),cvSize(m_maxSize,m_maxSize));
	for(int i = 0; i <pcvSeqFaces->total; i++){
		CvRect * r = (CvRect*)cvGetSeqElem(pcvSeqFaces, i);
		v.push_back( *r );
	}
	if( photo && v.size()>0){
		cvSaveImage(photo,pSrcImage);
	}
	cvReleaseImage(&pSrcImage);	
	cvReleaseImage(&pGrayImage);
	return (int)v.size();
}


//�ر�
int copencv::close(){
	cvReleaseMemStorage(&myMStorage);
	cvReleaseHaarClassifierCascade(&myHaarCascade);
	return 0;
}

int copencv::recognition(const char* haar, const char* file, std::vector<CvRect>& v){
	CvHaarClassifierCascade *pHaarCascade = NULL;
	cvReleaseHaarClassifierCascade(&pHaarCascade);
	//"c:\\opencv-2.4.10\\sources\\data\\haarcascades\\haarcascade_frontalface_alt.xml
	pHaarCascade = (CvHaarClassifierCascade*)cvLoad( haar );
	//����ͼ��
	IplImage *pSrcImage  = cvLoadImage( file , CV_LOAD_IMAGE_UNCHANGED);
	if( !pSrcImage )
		return -1;
	IplImage *pGrayImage = cvCreateImage(cvGetSize(pSrcImage), IPL_DEPTH_8U, 1);
	cvCvtColor(pSrcImage, pGrayImage, CV_BGR2GRAY);
	CvMemStorage *pcvMStorage = cvCreateMemStorage(0);
	cvClearMemStorage(pcvMStorage);
	//����ʶ��
	CvSeq *pcvSeqFaces = cvHaarDetectObjects(pGrayImage, pHaarCascade, pcvMStorage);
	for(int i = 0; i <pcvSeqFaces->total; i++){
		CvRect* r = (CvRect*)cvGetSeqElem(pcvSeqFaces, i);
		v.push_back( *r );
	}
	//eyereg(pSrcImage,v);
	cvReleaseMemStorage(&pcvMStorage);
	cvReleaseImage(&pSrcImage);	
	cvReleaseImage(&pGrayImage);
	cvReleaseHaarClassifierCascade(&pHaarCascade);
	return (int)v.size();
}

int copencv::eyereg(IplImage* pSrc, std::vector<CvRect> v ){
	CvHaarClassifierCascade *pHaarCascade = NULL;
	cvReleaseHaarClassifierCascade(&pHaarCascade);
	pHaarCascade = (CvHaarClassifierCascade*)cvLoad("C://SmartX//soft//bin//haarcascade_frontalface_alt.xml");
	for( size_t i = 0; i<v.size(); i++ ){
		//CvSize size= cvSize(v[i].right-v[i].left,v[i].bottom-v[i].top);
		//cvSetImageROI(pSrc,cvRect(v[i].left,v[i].top,size.width,size.height));//����Դͼ��ROI
		//IplImage* pDest = cvCreateImage(size,pSrc->depth,pSrc->nChannels);//����Ŀ��ͼ��
		//cvCopy(pSrc,pDest); //����ͼ��

		//CvMemStorage *pcvMStorage = cvCreateMemStorage(0);
		//cvClearMemStorage(pcvMStorage);
		//CvSeq *pcvSeqFaces = cvHaarDetectObjects(pDest, pHaarCascade, pcvMStorage);
		//for(size_t i = 0; i <pcvSeqFaces->total; i++){
		//	CvRect* r = (CvRect*)cvGetSeqElem(pcvSeqFaces, i);
		//	//RECT rc;
		//	//SetRect(&rc, r->x,r->y,r->x+r->width,r->y + r->height);
		//	v.push_back( *r );
		//}
		//cvReleaseMemStorage(&pcvMStorage);
		//cvReleaseImage(&pDest);	
		//cvResetImageROI(pDest);//Դͼ����������RO
		//cvSaveImage("c:\\Roi.jpg",pDest);//����Ŀ��ͼ��
	}
	return 0;
}


//֡�������м�������
//int copencv::compare(char* buf, char* buf2) {
//	CvMat mat = cvMat(640, 480, CV_8UC1, buf);
//	IplImage *pSrcImage = cvDecodeImage(&mat, 1);
//	if (!pSrcImage)
//		return -1;
//	IplImage *pGrayImage = cvCreateImage(cvGetSize(pSrcImage), IPL_DEPTH_8U, 1);
//	cvCvtColor(pSrcImage, pGrayImage, CV_BGR2GRAY);
//	cvClearMemStorage(myMStorage);
//	//����ʶ��
//	CvSeq *pcvSeqFaces = cvHaarDetectObjects(pGrayImage, myHaarCascade, myMStorage, 1.1, 2, 0, cvSize(m_minSize, m_minSize), cvSize(m_maxSize, m_maxSize));
//	for (int i = 0; i < pcvSeqFaces->total; i++) {
//		CvRect * r = (CvRect*)cvGetSeqElem(pcvSeqFaces, i);
//		CvPoint P1, P2;
//		P1.x = r->x;
//		P1.y = r->y;
//		P2.x = r->x + r->width;
//		P2.y = r->y + r->height;
//		cvRectangle(pSrcImage, P1, P2, CV_RGB(0, 255, 0), 2);
//		cvPutText(pSrcImage, "face", cvPoint(P1.x, P1.y - 10), &m_font, CV_RGB(255, 0, 0));
//	}
//	if (photo && pcvSeqFaces->total > 0) {
//		cvSaveImage(photo, pSrcImage);
//	}
//	cvReleaseImage(&pSrcImage);
//	cvReleaseImage(&pGrayImage);
//	return pcvSeqFaces->total;
//}

int copencv::screenshot( const char* imgpath ){
	// ��ȡ���ڵ�ǰ��ʾ�ļ�����
	HWND hWnd = GetDesktopWindow();
	HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);

	// ��ȡ�������߼����
	MONITORINFOEX monitorInfo;
	monitorInfo.cbSize = sizeof(monitorInfo);
	GetMonitorInfo(hMonitor, &monitorInfo);
	int cxLogical = (monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left);

	// ��ȡ������������
	DEVMODE dm;
	dm.dmSize = sizeof(dm);
	dm.dmDriverExtra = 0;
	EnumDisplaySettings(monitorInfo.szDevice, ENUM_CURRENT_SETTINGS, &dm);
	int cxPhysical = dm.dmPelsWidth;

	double zoom = dm.dmPelsWidth * 1.0 / cxLogical;
	int m_width   = GetSystemMetrics(SM_CXSCREEN) * zoom;
	int m_height  = GetSystemMetrics(SM_CYSCREEN) * zoom;

	LPVOID screenshotData = new char[m_width * m_height * 4];
	memset(screenshotData, 0, m_width);
	// ��ȡ��Ļ DC
	HDC m_screenDC = GetDC(NULL);
	HDC m_compatibleDC = CreateCompatibleDC(m_screenDC);
	HBITMAP m_hBitmap = CreateCompatibleBitmap(m_screenDC, m_width, m_height);
	SelectObject(m_compatibleDC, m_hBitmap);
	// �õ�λͼ������
	BitBlt(m_compatibleDC, 0, 0, m_width, m_height, m_screenDC, 0, 0, SRCCOPY);
	GetBitmapBits(m_hBitmap, m_width * m_height * 4, screenshotData);
	// ����ͼ��
	cv::Mat screenshot(m_height, m_width, CV_8UC4, screenshotData);
	cv::imwrite(imgpath, screenshot);
	DeleteDC(m_compatibleDC);
	DeleteObject(m_hBitmap);
	free(screenshotData);
	return 0;
}
