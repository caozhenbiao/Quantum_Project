#ifndef _DLIBWRAPER_H_
#define _DLIBWRAPER_H_
#include <vector>
#include <string>

typedef struct _rect {
	float left;
	float top;
	float right;
	float bottom;
}rect;

typedef struct _feature {
	float point[128];
}feature;


//opencv
class dlibwraper{
public:
	bool start( bool monitor );
	bool stop();

public:
	bool add_tracker_target(char* buf, int len, int left, int top, int width, int height);
	int get_tracking_targets(char* buf, int len, std::vector<rect>& boxs);

	bool add_detect_model(const char* svm);
	int get_detecting_targets(char* buf, int len, std::vector<int>& cls, std::vector<rect>& boxs);

	bool set_face_model(const char* marks, const char* network);
	int get_face_features(char* buf, int len, std::vector<rect>& boxs, std::vector<feature>& feats);

};
#endif