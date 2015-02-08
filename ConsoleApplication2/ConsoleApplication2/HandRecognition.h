#pragma once

#include"Setting.h"
#include"ButtonWindow.h"
#include"UsePoints.h"
#include"HandLog.h"
#include"TimeCounter.h"


class HandRecognition{
private:
	const int NUM_OF_FINGER = 5;
	HandLog* log;

public:
	static bool mousemouse;
	static bool getScreen;
	HandRecognition();
	HandRecognition(const HandRecognition&);
	HandRecognition(cv::Mat, HandLog*);
	HandRecognition(cv::Mat,TimeCounter*,HandLog*);
	HandRecognition(cv::VideoCapture, HandLog*);
	HandRecognition(cv::VideoCapture, TimeCounter*, HandLog*);
	~HandRecognition();


	int upper[3];// = cv::Scalar(255, 255, 255, 255);
	int lower[3];// = cv::Scalar(225, 180, 180, 0);


private:
	//cv::Scalar upper = cv::Scalar(195, 235, 235, 255);//Lab
	//cv::Scalar lower = cv::Scalar(1, 120, 100, 0);
	const int bin_type = CV_BGR2HSV;
	ButtonWindow* button;

	cv::Point centroid;
	double handRad;
	TimeCounter *tc;

	cv::Mat img;
	cv::Mat src_img;
	cv::Mat gray_img;
	cv::Mat canny_img;
	cv::Mat bin_img;
	cv::Mat hand_img;
	byte *UVSkinTable;
	std::vector<cv::Point> handContour;
	std::vector<std::vector<cv::Point>> contours;
	std::vector<std::vector<cv::Point>> may_be_hand_contours;
	cv::Point maxDistPoint;
	cv::Point* fingers;


	cv::Mat temp_img;
	cv::VideoCapture capture;

	int mouseMode;

	void static callback(int, int, int, int, void*);
	void static callback2(int, int, int, int, void*);
	void binarize();
	void binarize2();
	void initUVSkinTable();
	void findHand();
	bool trackHand();
	void recognizeHandGesture();
	void sortRegion(std::vector<std::vector<cv::Point>>);
	bool getFingers(std::vector<cv::Point>);
	void updateButtons();
	std::vector<cv::Vec4i> convexityDefects;
	std::vector<cv::Point> hand_poly;
	std::vector<int> hand_hull;
	cv::Point getCentroid(std::vector<cv::Point>);


	void kmeanFiltering(cv::Mat, cv::Mat);
	void meanShiftFiltering(cv::Mat, cv::Mat);
	bool getScreenImage(cv::Mat);

	static int req_x, req_y;
public:
	POINT getCentroid();
	void update();
	void setMouseMode(int);
	int getMouseMode();
};
