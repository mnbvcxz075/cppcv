#pragma once

#include"Setting.h"


class HandRecognition{
public:
	HandRecognition();
	HandRecognition(cv::Mat);
	HandRecognition(cv::VideoCapture);
	~HandRecognition();

private:
	cv::Scalar upper = cv::Scalar(200, 255, 255, 255);
	cv::Scalar lower = cv::Scalar(10, 100,	100, 0);
	const int bin_type = CV_BGR2Lab;

	cv::Point centroid;

	cv::Mat src_img;
	cv::Mat bin_img;
	cv::Mat hand_img;
	cv::Mat dist_img;
	std::vector<std::vector<cv::Point>> handContour;
	std::vector<std::vector<cv::Point>> contours;


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