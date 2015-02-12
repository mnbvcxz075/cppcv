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
	HandRecognition(const HandRecognition&);
	HandRecognition(cv::Mat);
	HandRecognition(cv::Mat,TimeCounter*);
	HandRecognition(cv::VideoCapture);
	HandRecognition(cv::VideoCapture, TimeCounter*);
	~HandRecognition();

	void update();

	int upper[3];// = cv::Scalar(255, 255, 255, 255);
	int lower[3];// = cv::Scalar(225, 180, 180, 0);

private:
	HandRecognition();
	const int bin_type = CV_BGR2HSV;
	ButtonWindow* button;

	cv::Point centroid;
	double radian;
	double handRad;
	TimeCounter *tc;

	cv::Mat img;
	cv::Mat src_img;
	cv::Mat bin_img;
	cv::Mat hand_img;
	byte *UVSkinTable;
	std::vector<cv::Point> handContour;
	std::vector<std::vector<cv::Point>> contours;
	std::vector<std::vector<cv::Point>> may_be_hand_contours;
	cv::Point maxDistPoint;
	cv::Point* fingers;	
	std::vector<cv::Vec4i> convexityDefects;
	std::vector<cv::Point> hand_poly;
	std::vector<int> hand_hull;


	cv::VideoCapture capture;

	void static callback(int, int, int, int, void*);
	void static callback2(int, int, int, int, void*);
	void binarize();
	void binarize2();
	void initUVSkinTable();
	void findHand();
	void sortRegion(std::vector<std::vector<cv::Point>>);
	bool getFingers(std::vector<cv::Point>);
	void updateButtons();
	cv::Point getCentroid(std::vector<cv::Point>);


	bool getScreenImage(cv::Mat);

	static int req_x, req_y;	
	static bool getScreen;

public:
	POINT getCentroid();
	bool isTurned();
	bool* existFingers();
};
