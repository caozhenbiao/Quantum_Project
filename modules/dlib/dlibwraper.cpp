#include "dlibwraper.h"
#include <dlib/dnn.h>
#include <dlib/gui_widgets.h>
#include <dlib/clustering.h>
#include <dlib/string.h>
#include <dlib/image_io.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/opencv.h>
#include <opencv2/opencv.hpp>

#include <dlib/svm_threaded.h>
#include <dlib/string.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_processing.h>
#include <dlib/data_io.h>
#include <dlib/cmd_line_parser.h>


#include <iostream>
#include <fstream>
 


using namespace dlib;
using namespace std;

template <template <int, template<typename>class, int, typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual = add_prev1<block<N, BN, 1, tag1<SUBNET>>>;

template <template <int, template<typename>class, int, typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual_down = add_prev2<avg_pool<2, 2, 2, 2, skip1<tag2<block<N, BN, 2, tag1<SUBNET>>>>>>;

template <int N, template <typename> class BN, int stride, typename SUBNET>
using block = BN<con<N, 3, 3, 1, 1, relu<BN<con<N, 3, 3, stride, stride, SUBNET>>>>>;

template <int N, typename SUBNET> using ares = relu<residual<block, N, affine, SUBNET>>;
template <int N, typename SUBNET> using ares_down = relu<residual_down<block, N, affine, SUBNET>>;

template <typename SUBNET> using alevel0 = ares_down<256, SUBNET>;
template <typename SUBNET> using alevel1 = ares<256, ares<256, ares_down<256, SUBNET>>>;
template <typename SUBNET> using alevel2 = ares<128, ares<128, ares_down<128, SUBNET>>>;
template <typename SUBNET> using alevel3 = ares<64, ares<64, ares<64, ares_down<64, SUBNET>>>>;
template <typename SUBNET> using alevel4 = ares<32, ares<32, ares<32, SUBNET>>>;

using anet_type = loss_metric<fc_no_bias<128, avg_pool_everything<
	alevel0<
	alevel1<
	alevel2<
	alevel3<
	alevel4<
	max_pool<3, 3, 2, 2, relu<affine<con<32, 7, 7, 2, 2,
	input_rgb_image_sized<150>
	>>>>>>>>>>>>;

image_window * livewin = NULL;
correlation_tracker tracker;
std::vector<object_detector<scan_fhog_pyramid<pyramid_down<6> > > > detectors;
frontal_face_detector facedetector = get_frontal_face_detector();
shape_predictor sp;
anet_type net;

bool dlibwraper::start(bool monitor) {
	if (monitor) {
		livewin = new image_window();
		livewin->set_title("start AI");
		livewin->set_size(1280, 760);
		livewin->clear_overlay();
		livewin->show();
	}
	return true;
}

bool dlibwraper::stop() {
	if (livewin) {
		livewin->hide();
		delete livewin;
		livewin = NULL;
	}
	return true;
}

//设置跟踪数据
bool dlibwraper::add_tracker_target(char* buf, int len, int left, int top, int width, int height) {
	std::vector<unsigned char> tmpbuf(len);
	memcpy(&tmpbuf[0], buf, len);
	matrix<rgb_pixel> img;
	cv::Mat tmpmat = cv::imdecode(tmpbuf, CV_LOAD_IMAGE_COLOR);
	cv::cvtColor(tmpmat, tmpmat, cv::COLOR_BGR2RGB);
	dlib::assign_image(img, cv_image<rgb_pixel>(tmpmat));
	tracker.start_track(img, centered_rect(point(left, top), width, height));
	return true;
}

//获取跟踪数据
int dlibwraper::get_tracking_targets(char* buf, int len, std::vector<rect>& boxs) {
	std::vector<unsigned char> tmpbuf(len);
	memcpy(&tmpbuf[0], buf, len);
	matrix<rgb_pixel> img;
	cv::Mat tmpmat = cv::imdecode(tmpbuf, CV_LOAD_IMAGE_COLOR);
	cv::cvtColor(tmpmat, tmpmat, cv::COLOR_BGR2RGB);
	dlib::assign_image(img, cv_image<rgb_pixel>(tmpmat));
	tracker.update(img);
	if (livewin) {
		livewin->set_image(img);
		livewin->clear_overlay();
		livewin->add_overlay(tracker.get_position());
	}
	rectangle rc = tracker.get_position();
	rect box = { 0.0 };
	box.left = rc.left();
	box.top = rc.top();
	box.right = rc.right();
	box.bottom = rc.bottom();
	boxs.push_back( box );
	return boxs.size();
}


//对象检测
bool dlibwraper::add_detect_model(const char* svm) {
	printf("add_detect_model:%s\n", svm);
	ifstream fin(svm, ios::binary);
	if (!fin) {
		cout << "Can't find a trained object detector file object_detector.svm. " << endl;
		return false;
	}
	object_detector<scan_fhog_pyramid<pyramid_down<6> > > detector;
	deserialize(detector, fin);
	detectors.push_back(detector);
	return true;
}

//获取检测数据
int dlibwraper::get_detecting_targets(char* buf, int len, std::vector<int>& cls, std::vector<rect>& boxs) {
	std::vector<unsigned char> tmpbuf(len);
	memcpy(&tmpbuf[0], buf, len);
	matrix<rgb_pixel> img;
	cv::Mat tmpmat = cv::imdecode(tmpbuf, CV_LOAD_IMAGE_COLOR);
	cv::cvtColor(tmpmat, tmpmat, cv::COLOR_BGR2RGB);
	dlib::assign_image(img, cv_image<rgb_pixel>(tmpmat));
	std::vector<dlib::rect_detection> detections;
	evaluate_detectors(detectors, img, detections);
	if (livewin) {
		livewin->clear_overlay();
		livewin->set_image(img);
	}
	for (auto& det : detections) {
		if (livewin) { livewin->add_overlay(det.rect, rgb_pixel(255, 0, 0)); }
		rect rc = { 0.0 };
		rc.left = det.rect.left();
		rc.top = det.rect.top();
		rc.right = det.rect.right();
		rc.bottom = det.rect.bottom();
		cls.push_back(det.weight_index);
		boxs.push_back(rc);
	}
	return cls.size();
}

//人脸特征模型
bool dlibwraper::set_face_model(const char* marks, const char* network) {
	deserialize(marks) >> sp;
	deserialize(network) >> net;
	return true;
}

//人脸特征获取
int dlibwraper::get_face_features(char* buf, int len, std::vector<rect>& boxs, std::vector<feature>& feats) {
	std::vector<unsigned char> tmpbuf(len);
	memcpy(&tmpbuf[0], buf, len);
	matrix<rgb_pixel> img;
	cv::Mat tmpmat = cv::imdecode(tmpbuf, CV_LOAD_IMAGE_COLOR);
	cv::cvtColor(tmpmat, tmpmat, cv::COLOR_BGR2RGB);
	dlib::assign_image(img, cv_image<rgb_pixel>(tmpmat));
	if (livewin) {
		livewin->clear_overlay();
		livewin->set_image(img);
	}
	std::vector<matrix<rgb_pixel>> faces;
	for (auto face : facedetector(img)) {
		auto shape = sp(img, face);
		matrix<rgb_pixel> face_chip;
		extract_image_chip(img, get_face_chip_details(shape, 150, 0.25), face_chip);
		faces.push_back(move(face_chip));
		if (livewin) {
			livewin->add_overlay(face);
		}
		rect box;
		box.left = face.left();
		box.top = face.top();
		box.right = face.right();
		box.bottom = face.bottom();
		boxs.push_back(box);
	}
	if (faces.size() < 1)
		return -1;
	std::vector<matrix<float, 0, 1>> face_descriptors = net(faces);
	printf("faceregion size:%d\n", face_descriptors.size());
	int featsize = face_descriptors.size();
	for (int i = 0; i < featsize; i++) {
		matrix<float> desc = trans(face_descriptors[i]);
		feature feat;
		for (long r = 0; r < desc.nr(); ++r) {
			for (long c = 0; c < desc.nc(); ++c) {
				feat.point[r*desc.nc() + c] = desc(r, c);
			}
		}
		feats.push_back(feat);
	}
	return featsize;
}