#include"HandRecognition.h"
#include<cmath>
HandRecognition::HandRecognition(cv::Mat img)
: upper( cv::Scalar(255, 255, 255, 255)),
lower( cv::Scalar(0, 0, 0, 0))
{
	this->img = img;
	src_img = img.clone();

}
HandRecognition::HandRecognition(cv::VideoCapture cap)
: upper(cv::Scalar(100, 255, 255, 255)),
lower(cv::Scalar(1, 125, 110, 0))
{
	this->capture = cap;
	capture >> src_img;

	exist_contour=false;
	isMouse = false;
}
HandRecognition::~HandRecognition(){

}

void HandRecognition::update(){
	if (capture.isOpened())
		capture >> src_img;
	else
		src_img = img.clone();
	handContour.clear();

	binarization();
	findHand();
	//distTransform();
////////////////////////////////////////////////////////////
	if (exist_contour){
		cv::approxPolyDP(cv::Mat(handContour), hand_poly, 40, true);
		if (hand_poly.size()>3){
			int pnum = hand_poly.size();
			for (int i = 0; i < pnum; ++i)
				cv::line(hand_img, hand_poly[i], hand_poly[i + 1 < pnum ? i + 1 : 0], cv::Scalar(100, 100, 200), 2, CV_AA);

			cv::convexHull(hand_poly, hand_hull, true);


			int hnum = hand_hull.size();
			for (int i = 0; i < hnum; ++i)
				cv::line(hand_img, hand_poly[hand_hull[i]], hand_poly[hand_hull[i + 1 < hnum ? i + 1 : 0]], cv::Scalar(100, 100, 200), 2, CV_AA);


			cv::convexityDefects(hand_poly, hand_hull, convexityDefects);


			for (int i = 0; i < convexityDefects.size(); i++){
				double cos = getCos(convexityDefects[i]);
				if (cos>std::cos(1.7) && convexityDefects[i][3]>10000){
					cv::circle(src_img, hand_poly[convexityDefects[i][0]], 5, cv::Scalar(0, 0, 0, 0), -1);
					cv::circle(src_img, hand_poly[convexityDefects[i][1]], 8, cv::Scalar(255, 0, 0, 0), -1);
					cv::circle(src_img, hand_poly[convexityDefects[i][2]], 8, cv::Scalar(0, 255, 0, 0), -1);
				}
			}
		}
	}
	///////////////////////////////////////////////////////////
	cv::imshow(WINDOW_NAME, hand_img);
	cv::imshow(WINDOW_NAME + '2', bin_img);
	cv::imshow(WINDOW_NAME + '3', src_img);
}

void HandRecognition::binarization(){

	cv::cvtColor(src_img, temp_img, bin_type);
	cv::inRange(temp_img, lower, upper, bin_img);

	cv::erode(bin_img, bin_img, cv::Mat(), cv::Point(-1, -1), 5);
	cv::dilate(bin_img, bin_img, cv::Mat(), cv::Point(-1, -1), 8);
	cv::erode(bin_img, bin_img, cv::Mat(), cv::Point(-1, -1), 3);
}

void HandRecognition::findHand(){
	temp_img = bin_img.clone();
	exist_contour = false;
	hand_img.create(bin_img.size(), bin_img.type());

	cv::findContours(temp_img, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	double max_size = 0;
	for (std::vector<std::vector<cv::Point>>::iterator it = contours.begin(); it != contours.end(); it++){
		double area_size = cv::contourArea(cv::Mat(*it));
		if (area_size > max_size){
			max_size = area_size;
			handContour.clear();
			handContour = *it;
		}
	}
	cv::rectangle(hand_img, cv::Point(0, 0), cv::Point(bin_img.cols, bin_img.rows), cv::Scalar(0, 0, 0, 0), -1);


	if (handContour.size() > 2)
		exist_contour = true;

	if (exist_contour){
		cv::Moments moments = cv::moments(handContour);
		centroid = cv::Point(moments.m10 / moments.m00, moments.m01 / moments.m00);
		cv::Scalar color = isMouse ? cv::Scalar(0, 0, 255, 0) : cv::Scalar(0, 255, 0, 0);
		cv::circle(src_img, centroid, 5, color, -1);
	}
}

void HandRecognition::distTransform(){
	cv::distanceTransform(hand_img, dist_img, CV_DIST_L2, 3);
	float*ptr = (float*)dist_img.data;
	double max_distance = 0;
	cv::Point max_point;
	for (int i = 0; i < dist_img.rows; i++){
		for (int j = 0; j < dist_img.cols; j++){
			if (max_distance<*ptr){
				max_distance = *ptr;
				max_point.x = j;
				max_point.y = i;
				std::cout << *ptr << i << j << std::endl;
			}
			ptr++;
		}
	}
	cv::circle(src_img, max_point, 5, cv::Scalar(0, 255, 0, 0), -1);

}

POINT HandRecognition::getCentroid(){
	POINT p = POINT();
	if (exist_contour){
		cv::Moments moments = cv::moments(handContour);
		p.x = moments.m10 / moments.m00;
		p.y = moments.m01 / moments.m00;
	}
	else{
		p.x = 0; p.y = 0;
	}

	return p;
}

double HandRecognition::getCos(cv::Vec4i vec){
	return
		((hand_poly[vec[0]].x - hand_poly[vec[2]].x)
		*(hand_poly[vec[1]].x - hand_poly[vec[2]].x)
		+ (hand_poly[vec[0]].y - hand_poly[vec[2]].y)
		*(hand_poly[vec[1]].y - hand_poly[vec[2]].y))
		/ std::sqrt(
		((hand_poly[vec[1]].x - hand_poly[vec[2]].x)
		*(hand_poly[vec[1]].x - hand_poly[vec[2]].x)
		+ (hand_poly[vec[1]].y - hand_poly[vec[2]].y)
		*(hand_poly[vec[1]].y - hand_poly[vec[2]].y))
		*
		((hand_poly[vec[0]].x - hand_poly[vec[2]].x)
		*(hand_poly[vec[0]].x - hand_poly[vec[2]].x)
		+ (hand_poly[vec[0]].y - hand_poly[vec[2]].y)
		*(hand_poly[vec[0]].y - hand_poly[vec[2]].y)));

}

void HandRecognition::setIsMouse(bool is){
	isMouse = is;
}