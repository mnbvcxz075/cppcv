#include"UsePoints.h"

/*
UsePoints::turnPoint
cv::Point point   
double turnMat[4]
cv::Point center
*/
cv::Point UsePoints::turnPoint(cv::Point point, double turnMat[4], cv::Point center){
	double x = point.x - center.x, y = point.y - center.y;
	point.x = x*turnMat[0] + y*turnMat[1] + center.x;
	point.y = x*turnMat[2] + y*turnMat[3] + center.y;

	return point;

}
cv::Point UsePoints::turnPoint(cv::Point point, double rad, cv::Point center){
	double mat[] = { std::cos(rad), -std::sin(rad), std::sin(rad), std::cos(rad) };
	return turnPoint(point, mat, center);

}
std::vector<cv::Point> UsePoints::turnPoints(std::vector<cv::Point> points, double rad, cv::Point center){
	double mat[] = { std::cos(rad), -std::sin(rad), std::sin(rad), std::cos(rad) };
	for (std::vector<cv::Point>::iterator it = points.begin(); it != points.end(); it++){
		*it = turnPoint(*it, mat, center);
	}
	return points;

}

std::vector<cv::Point> UsePoints::movePoints(std::vector<cv::Point> points, int x, int y){
	for (std::vector<cv::Point>::iterator it = points.begin(); it != points.end(); it++){
		it->x -= x;
		it->y -= y;
	}
	return points;
}





double UsePoints::getCos(cv::Point a, cv::Point b, cv::Point c){
	double d = ((c.x - a.x)*(c.x - a.x) + (c.y - a.y)*(c.y - a.y));
	double e = d*((b.x - a.x)*(b.x - a.x) + (b.y - a.y)*(b.y - a.y));
	return ((b.x - a.x)*(c.x - a.x) + (b.y - a.y)*(c.y - a.y)) / std::sqrt(e);
}

double UsePoints::distance(cv::Point p1, cv::Point p2){
	return sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
}


cv::Point UsePoints::meanPoint(std::vector<cv::Point>points){
	if (points.size() < 1){
		std::cout << "meanPoint : points size < 1" << std::endl;
		return cv::Point(0, 0);
	}
	int x = 0, y = 0;
	for (cv::Point point: points){
		x += point.x;
		y += point.y;
	}
	x /= (int)points.size();
	y /= (int)points.size();
	return cv::Point(x, y);
}