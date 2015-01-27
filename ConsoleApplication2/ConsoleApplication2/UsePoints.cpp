#include"UsePoints.h"

cv::Point UsePoints::turnPoints(cv::Point point, double turnMat[4], cv::Point center){
	double x = point.x - center.x, y = point.y - center.y;
	point.x = x*turnMat[0] + y*turnMat[1];
	point.y = x*turnMat[2] + y*turnMat[3];

	return point;

}
cv::Point UsePoints::turnPoints(cv::Point point, double rad, cv::Point center){
	double mat[] = { std::cos(rad), -std::sin(rad), std::sin(rad), std::cos(rad) };
	return turnPoints(point, mat, center);

}

double UsePoints::getCos(cv::Point a, cv::Point b, cv::Point c){
	return
		((b.x - a.x)*(c.x - a.x) + (b.y - a.y)*(c.y - a.y))
		/ std::sqrt(((c.x - a.x)*(c.x - a.x) + (c.y - a.y)*(c.y - a.y))
		*((b.x - a.x)*(b.x - a.x) + (b.y - a.y)*(b.y - a.y)));
}

double UsePoints::distance(cv::Point p1, cv::Point p2){
	return sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
}