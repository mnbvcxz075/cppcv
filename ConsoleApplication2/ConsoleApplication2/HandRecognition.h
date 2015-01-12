#pragma once

#include"Setting.h"


class HandRecognition{
public:
	HandRecognition();
	HandRecognition(cv::Mat);
	HandRecognition(cv::VideoCapture);
	~HandRecognition();

private:
	//cv::Scalar upper = cv::Scalar(195, 235, 235, 255);//Lab
	//cv::Scalar lower = cv::Scalar(1, 120, 100, 0);
	cv::Scalar const upper;// = cv::Scalar(255, 255, 255, 255);
	cv::Scalar const lower;// = cv::Scalar(225, 180, 180, 0);
	const int bin_type = CV_BGR2YCrCb;

	cv::Point centroid;

	cv::Mat img;
	cv::Mat src_img;
	cv::Mat bin_img;
	cv::Mat hand_img;
	cv::Mat dist_img;
	std::vector<cv::Point> handContour;
	boolean exist_contour;
	std::vector<std::vector<cv::Point>> contours;
	std::vector<int> hand_hull;
	std::vector<cv::Point> hand_poly;
	std::vector<cv::Vec4i> convexityDefects;


	cv::Mat temp_img;
	cv::VideoCapture capture;

	bool isMouse;


	void binarization();
	void findHand();
//	POINT getCentroid();
	void distTransform();
	void recognizeHandGesture();


public:
	void update();
};