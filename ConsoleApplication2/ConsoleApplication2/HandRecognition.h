#pragma once

#include"Setting.h"
#include"ButtonWindow.h"


class HandRecognition{
public:
	HandRecognition();
	HandRecognition(const HandRecognition&);
	HandRecognition(cv::Mat);
	HandRecognition(cv::VideoCapture);
	~HandRecognition();


	int upper[3];// = cv::Scalar(255, 255, 255, 255);
	int lower[3];// = cv::Scalar(225, 180, 180, 0);


private:
	//cv::Scalar upper = cv::Scalar(195, 235, 235, 255);//Lab
	//cv::Scalar lower = cv::Scalar(1, 120, 100, 0);
	const int bin_type = CV_BGR2HSV;
	ButtonWindow* button;

	cv::Point centroid;

	cv::Mat img;
	cv::Mat src_img;
	cv::Mat gray_img;
	cv::Mat canny_img;
	cv::Mat bin_img;
	cv::Mat hand_img;
	cv::Mat dist_img;
	std::vector<cv::Point> handContour;
	boolean exist_contour;
	std::vector<std::vector<cv::Point>> contours;
	std::vector<std::vector<cv::Point>> may_be_hand_contours;
	std::vector<int> hand_hull;
	std::vector<cv::Point> hand_poly;
	std::vector<cv::Vec4i> convexityDefects;
	cv::Point maxDistPoint;
	cv::Point* fingers;
	cv::Point* fingers2;


	cv::Mat temp_img;
	cv::VideoCapture capture;

	int mouseMode;

	void binarization();
	void findHand();
	void distTransform();
	void recognizeHandGesture();
	void soatRegion(std::vector<std::vector<cv::Point>>);
	bool getFingers(std::vector<cv::Point>);
	double getCos(cv::Vec4i);
	double distance(cv::Point p1, cv::Point p2);


public:
	POINT getCentroid();
	void update();
	void setMouseMode(int);
	int getMouseMode();
};
