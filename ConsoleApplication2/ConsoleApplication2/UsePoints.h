#pragma once
#include"Setting.h"
#include"opencv2\opencv.hpp"

class UsePoints
{
public:
	UsePoints();
	~UsePoints();

	static cv::Point turnPoint(cv::Point, double[4], cv::Point);
	static cv::Point turnPoint(cv::Point, double, cv::Point);
	static std::vector<cv::Point> movePoints(std::vector<cv::Point>, int, int);
	static std::vector<cv::Point> turnPoints(std::vector<cv::Point>, double, cv::Point);
	static double getCos(cv::Point, cv::Point, cv::Point);
	static double distance(cv::Point p1, cv::Point p2);
	static cv::Point meanPoint(std::vector<cv::Point>);
private:

};
