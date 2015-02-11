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
	
	bool* getExist();
	std::vector<cv::Point> getFingers();
	void setFingers(std::vector<cv::Point>, cv::Point);

	POINT getCentroid();

private:
	const int errorDistance = 999;
	const int logSize = 10;

	bool existFingers[5];
	std::vector<cv::Point> fingersLog[5];
	int size;
	int errorWeight;
	int LRweight;
	bool rightHand;
	cv::Point centroid;
	cv::Point privious_centroid;
	std::vector<cv::Point> fingersMean;


	std::vector<cv::Point> sort(std::vector<cv::Point>*);
};