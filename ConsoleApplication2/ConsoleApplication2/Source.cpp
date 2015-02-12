#include <iostream>
#include"HandRecognition.h"
#include"HandMouse.h"
#include"TimeCounter.h"
#include<time.h>


//int main(int argc, char** argv){
//	cv::Mat img = cv::imread(argv[1], 0);
//	cv::namedWindow("aa", 1);
//	cv::inRange(img, cv::Scalar(0, 0, 0, 0), cv::Scalar(220, 220, 220, 0), img);
//	while (cvWaitKey(1) == -1){
//		cv::imshow("aa", img);
//	}
//
//	cv::Mat dist;
//	cv::distanceTransform(img.clone(), dist, CV_DIST_L2, 3);
//	cv::Mat img2;
//	cv::normalize(dist, img2, 0, 255, cv::NORM_MINMAX,CV_8U);
//	while (cvWaitKey(1) == -1){
//		cv::imshow("aa", img2);
//	}
//	cv::Point p;
//	double distance;
//	cv::minMaxLoc(dist, NULL, &distance, NULL, &p);
//	cv::circle(img, p, 5, cv::Scalar(100, 0, 0, 0), -1);
//	cv::circle(img, p, distance, cv::Scalar(100, 0, 0, 0), 2);
//	cv::imwrite("D:/desktop/2.png", img2);
//	cv::imwrite("D:/desktop/1.png", img);
//}

int main(int argc, char** argv)
{

	cv::VideoCapture cap(0); // デフォルトカメラをオープン
	cv::Mat img;
	if (!cap.isOpened()){  // 成功したかどうかをチェック
		if (argv[1] != NULL){
			img = cv::imread(argv[1], 1);
		}
		else{
			return -1;
		}
	}

	TimeCounter* tc = new TimeCounter();
	HandRecognition hand(cap,tc);

	HandMouse mouse(&hand);
	int timer = timeGetTime();
	double time = 0;
	while (cvWaitKey(1) == -1){
		tc->start("loop");
		tc->start("time");
		hand.update();
		mouse.update();
		tc->stop("loop");
		time = tc->stop("time");
		Sleep(80 - time>0 ? 80 - time : 1);
	}

	std::cout << "binarize " << tc->get("binarize", true) << std::endl;
	std::cout << "findHand " << tc->get("findHand", true) << std::endl;
	std::cout << "Dist " << tc->get("Dist", true) << std::endl;
	std::cout << "Moment " << tc->get("Moment", true) << std::endl;
	std::cout << "inRange " << tc->get("inRange", true) << std::endl;
	std::cout << "noize " << tc->get("noize", true) << std::endl;
	std::cout << "loop " << tc->get("loop", true) << std::endl;
	Sleep(10000);
	return 0;
}


