#pragma once

#include"Setting.h"
#include"UsePoints.h"
#include"opencv2\opencv.hpp"
enum{
	thumb,index,middle,ring,little
};
class HandLog{
public:
	HandLog();
	~HandLog();

	void setFingers(std::vector<cv::Point>, cv::Point);
	cv::Point getCVCentroid();
	POINT getCentroid();
	POINT getPriviousCentroid();

	bool ready();
	std::vector<cv::Point> getFingers();
	bool existFingers[5];
	std::vector<cv::Point> fingersLog[5];
private:
	const int errorDistance = 999;
	const int logSize = 10;

	int size;
	int errorWeight;
	int LRweight;
	bool rightHand;
	cv::Point centroid;
	cv::Point privious_centroid;
	std::vector<cv::Point> fingersMean;


	std::vector<cv::Point> sort(std::vector<cv::Point>*);
};