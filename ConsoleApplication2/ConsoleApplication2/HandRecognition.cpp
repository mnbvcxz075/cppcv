#include"HandRecognition.h"

HandRecognition::HandRecognition(cv::Mat img){
	src_img = img;
}
HandRecognition::HandRecognition(cv::VideoCapture cap){
	this->capture = cap;
	capture >> src_img;
}
HandRecognition::~HandRecognition(){

}

void HandRecognition::update(){
	if (capture.isOpened())
	capture >> src_img;
	handContour.clear();

	binarization();
	findHand();
////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
	cv::imshow(WINDOW_NAME, hand_img);
//	cv::imshow(WINDOW_NAME+'2', dist_img);
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
	hand_img.create(bin_img.size(), bin_img.type());
	cv::findContours(temp_img, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	double max_size = 0;
	for (std::vector<std::vector<cv::Point>>::iterator it = contours.begin(); it != contours.end(); it++){
		double area_size = cv::contourArea(cv::Mat(*it));
		if (area_size > max_size){
			max_size = area_size;
			handContour.clear();
			handContour.push_back(*it);
		}
	}
	cv::rectangle(hand_img, cv::Point(0, 0), cv::Point(bin_img.cols, bin_img.rows), cv::Scalar(0, 0, 0, 0), -1);
	cv::drawContours(hand_img, handContour, contours.size()>1 ? -1 : 0, cv::Scalar(255, 255, 0, 0), -1);


	cv::Moments moments = cv::moments(handContour[0]);
	centroid = cv::Point(moments.m10 / moments.m00, moments.m01 / moments.m00);
	cv::circle(hand_img,centroid,30, cv::Scalar(0, 0, 0, 0), -1);

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
	cv::circle(hand_img, max_point, 30, cv::Scalar(0, 0, 0, 0), -1);

}