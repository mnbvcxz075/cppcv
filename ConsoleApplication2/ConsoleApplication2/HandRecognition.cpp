#include"HandRecognition.h"
#include<cmath>
HandRecognition::HandRecognition(){

	upper[0] = 30;
	lower[0] = 170;
	upper[1] = 250;
	lower[1] = 1;
	upper[2] = 250;
	lower[2] = 1;

	exist_contour = false;
	mouseMode = notMouse;

	fingers = new cv::Point[5];
	fingers2 = new cv::Point[5];

	cv::namedWindow(WINDOW_NAME, 1);
	cv::namedWindow(WINDOW_NAME + '2', 1);
	cv::namedWindow(WINDOW_NAME + '3', 1);

//	button = new ButtonWindow();

}
HandRecognition::HandRecognition(cv::Mat img)
:HandRecognition()
{
	this->img = img;
	src_img = img.clone();

}
HandRecognition::HandRecognition(cv::VideoCapture cap)
:HandRecognition()
{
	this->capture = cap;
	capture >> src_img;
}
HandRecognition::~HandRecognition(){

}

void HandRecognition::update(){
	if (capture.isOpened())
		capture >> src_img;
	else
		src_img = img.clone();
	handContour.clear();

	for (int i = 0; i < button->low; i++){
		for (int j = 0; j < button->col; j++){
			if (button->getButton(j+i*button->col)){
				if (i == 0){
					if (j % 2 == 0)
						button->changeNum(j,++upper[j / 2]);
					else
						button->changeNum(j, ++lower[j / 2]);
				}
				else if (i == 2){

					if (j % 2 == 0)
						button->changeNum(j, --upper[j / 2]);
					else
						button->changeNum(j, --lower[j / 2]);
				}
			}
			button->setButton(false, j + i*button->col);
		}
	}

	binarization();
	findHand();
	distTransform();
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
	cv::imshow(WINDOW_NAME, hand_img);
	cv::imshow(WINDOW_NAME + '2', bin_img);
	cv::imshow(WINDOW_NAME + '3', src_img);
}

void HandRecognition::binarization(){

	cv::cvtColor(src_img, temp_img, bin_type);
	if (lower[0]<upper[0]){
		cv::inRange(temp_img, cv::Scalar(lower[0], lower[1], lower[2], 0), cv::Scalar(upper[0], upper[1], upper[2], 0), bin_img);
	}
	else{
		cv::inRange(temp_img, cv::Scalar(0, lower[1], lower[2], 0), cv::Scalar(upper[0], upper[1], upper[2], 0), bin_img);
		cv::inRange(temp_img, cv::Scalar(lower[0], lower[1], lower[2], 0), cv::Scalar(0, upper[1], upper[2], 0), hand_img);
		cv::add(bin_img,hand_img,bin_img);

	}

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
	soatRegion(contours);
	for (std::vector<std::vector<cv::Point>>::iterator it = may_be_hand_contours.begin(); it != may_be_hand_contours.end(); it++){
		if (getFingers(*it)){
			handContour = *it;
			break;
		}
	}
	cv::rectangle(hand_img, cv::Point(0, 0), cv::Point(bin_img.cols, bin_img.rows), cv::Scalar(0, 0, 0, 0), -1);


//		cv::fillPoly(hand_img, handContour, cv::Scalar(0, 0, 255, 0));
		cv::Moments moments = cv::moments(handContour);
		centroid = cv::Point(moments.m10 / moments.m00, moments.m01 / moments.m00);
		cv::Scalar color;
		switch (mouseMode){
		case notMouse:color = cv::Scalar(255,0,0,0); break;
		case isMouse:color = cv::Scalar(0,0,255,0); break;
		case isTouched:color = cv::Scalar(0,255,0,0); break;
		default:color = cv::Scalar(0,0,0,0); break;
		}
		cv::circle(src_img, centroid, 5, color, -1);
}

void HandRecognition::distTransform(){
	cv::distanceTransform(bin_img, dist_img, CV_DIST_L2, 3);

	double max_distance=0;
	cv::Point max_point=0;
	cv::minMaxLoc(dist_img, NULL, &max_distance, NULL, &max_point);

	//float*ptr = (float*)dist_img.data;
	//for (int i = 0; i < dist_img.rows; i++){
	//	for (int j = 0; j < dist_img.cols; j++){
	//		if (max_distance<*ptr){
	//			max_distance = *ptr;
	//			max_point.x = j;
	//			max_point.y = i;
	//			std::cout << *ptr << i << j << std::endl;
	//		}
	//		ptr++;
	//	}
	//}
		cv::circle(src_img, max_point, 5, cv::Scalar(0, 255, 0, 0), -1);

}

bool HandRecognition::getFingers(std::vector<cv::Point> contour){
	hand_poly.clear();
	for (int i = 0; i < 5; i++){
		fingers[i] = cv::Point(0, 0);
		fingers2[i] = cv::Point(0, 0);
	}

	//‹ßŽ—ƒ|ƒŠƒSƒ“‚ÌŽæ“¾
	cv::approxPolyDP(cv::Mat(contour), hand_poly, 10, true);

	if (hand_poly.size() > 3){
		//‹ßŽ—ƒ|ƒŠƒSƒ“‚Ì•`‰æ
		int pnum = hand_poly.size();
		for (int i = 0; i < pnum; ++i)
			cv::line(hand_img, hand_poly[i], hand_poly[i + 1 < pnum ? i + 1 : 0], cv::Scalar(100, 100, 200), 2, CV_AA);

		//“Ê•ï‚ÌŽæ“¾
		cv::convexHull(hand_poly, hand_hull, true);


		//“Ê•ï‚Ì•`‰æ
		int hnum = hand_hull.size();
		for (int i = 0; i < hnum; ++i)
			cv::line(hand_img, hand_poly[hand_hull[i]], hand_poly[hand_hull[i + 1 < hnum ? i + 1 : 0]], cv::Scalar(100, 100, 200), 2, CV_AA);

		//‰šóŒ‡‘¹‚ÌŽæ“¾
		cv::convexityDefects(hand_poly, hand_hull, convexityDefects);

		mouseMode = notMouse;
		int n = 0;

		//
		for (int i = 0; i < convexityDefects.size(); i++){
			double cos = getCos(convexityDefects[i]);
			if (cos>std::cos(1.7) && convexityDefects[i][3]>10000){
				//fingers[n] = fingers[n].x == hand_poly[convexityDefects[i][0]].x
				//				&& fingers[n].y == hand_poly[convexityDefects[i][0]].y
				//			? hand_poly[convexityDefects[i][0]]
				//			: cv::Point((fingers[n].x + hand_poly[convexityDefects[i][0]].x)/2
				//				, (fingers[n].y + hand_poly[convexityDefects[i][0]].y) / 2);

				//fingers2[n] = hand_poly[convexityDefects[i][2]];
				//fingers[n+1] = hand_poly[convexityDefects[i][1]];
				n++;
			}
		}


		if (n <= 2){
			return false;
		}

		if (n == 4){
			mouseMode = isMouse;
		}
		else
		if (n == 3){
			mouseMode = isTouched;
		}
		return true;
		//for (int i = 0; i < 5; i++){
		//	cv::circle(src_img, fingers[i], 5, cv::Scalar(50 * i,0, 0, 0), -1);
		//	if(i!=5)
		//		cv::circle(src_img, fingers2[i], 5, cv::Scalar(0, 0, 50 * i, 0), -1);
		//}

	}
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

void HandRecognition::setMouseMode(int mode){
	mouseMode = mode;
}
int HandRecognition::getMouseMode(){
	return mouseMode;
}


void HandRecognition::soatRegion(std::vector<std::vector<cv::Point>> contour){
	double area = 0;
	may_be_hand_contours.clear();
	double sizes[] = {0,0,0};
	for (std::vector<cv::Point> con:contour){
		area = cv::contourArea(con);
		if (area > 640 * 480 / 100){
			std::cout << area << " ";
				std::vector<std::vector<cv::Point>>::iterator it = may_be_hand_contours.begin();
				for (int i = 0; i < 3;i++){
					if (sizes[i]<area){
						for (int j = 2; j > i; j--){
							sizes[j] = sizes[j - 1];
						}
						sizes[i] = area;
						may_be_hand_contours.insert(it, con);
						if (may_be_hand_contours.size() > 3)
							may_be_hand_contours.pop_back();
						break;
					}
					it++;
				}
//			}
		}
	}
	std::cout << std::endl;
}