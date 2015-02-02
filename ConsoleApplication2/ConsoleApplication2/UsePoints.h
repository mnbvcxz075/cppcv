#pragma once
#include"Setting.h"

class UsePoints
{
public:
	UsePoints();
	~UsePoints();

	static cv::Point turnPoints(cv::Point, double[4], cv::Point);
	static cv::Point turnPoints(cv::Point, double, cv::Point);
	static double getCos(cv::Point, cv::Point, cv::Point);
	static double distance(cv::Point p1, cv::Point p2);
private:

};
