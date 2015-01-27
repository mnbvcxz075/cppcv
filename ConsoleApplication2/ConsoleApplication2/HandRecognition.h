#pragma once

#include"Setting.h"
#include"ButtonWindow.h"


class HandRecognition{
private:
	const int NUM_OF_FINGER = 5;

public:
	static bool mousemouse;
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
	double handRad;

	cv::Mat img;
	cv::Mat src_img;
	cv::Mat gray_img;
	cv::Mat canny_img;
	cv::Mat bin_img;
	cv::Mat hand_img;
	cv::Mat dist_img;
	byte *UVSkinTable;
	std::vector<cv::Point> handContour;
	boolean exist_contour;
	std::vector<std::vector<cv::Point>> contours;
	std::vector<std::vector<cv::Point>> may_be_hand_contours;
	std::vector<int> hand_hull;
	std::vector<cv::Point> hand_poly;
	std::vector<cv::Vec4i> convexityDefects;
	cv::Point maxDistPoint;
	cv::Point* fingers;


	cv::Mat temp_img;
	cv::VideoCapture capture;

	int mouseMode;

	void static callback(int, int, int, int, void*);
	void binarization();
	void binarization2();
	void initUVSkinTable();
	void findHand();
	void recognizeHandGesture();
	void soatRegion(std::vector<std::vector<cv::Point>>);
	void kmeanFiltering(cv::Mat, cv::Mat);
	void meanShiftFiltering(cv::Mat, cv::Mat);
	bool getFingers(std::vector<cv::Point>);
	double getCos(cv::Vec4i);
	double getCos(cv::Point, cv::Point, cv::Point);
	double distance(cv::Point p1, cv::Point p2);
	std::vector<cv::Point> movePoints(std::vector<cv::Point>, int, int);
	cv::Point turnPoints(cv::Point, double[4], cv::Point);
	cv::Point turnPoints(cv::Point, double, cv::Point);
	std::vector<cv::Point> turnPoints(std::vector<cv::Point>, double, cv::Point);
	cv::Point getCentroid(std::vector<cv::Point>);

	static int req_x, req_y;
public:
	POINT getCentroid();
	void update();
	void setMouseMode(int);
	int getMouseMode();
};
