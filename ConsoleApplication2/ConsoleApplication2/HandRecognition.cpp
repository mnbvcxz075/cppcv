#include"HandRecognition.h"
#include<cmath>
HandRecognition::HandRecognition(){

	upper[0] = 35;
	lower[0] = 170;
	upper[1] = 255;
	lower[1] = 60;
	upper[2] = 250;
	lower[2] = 10;

	exist_contour = false;
	mouseMode = notMouse;

	fingers = new cv::Point[NUM_OF_FINGER];

	cv::namedWindow(WINDOW_NAME, 1);
	cv::namedWindow(WINDOW_NAME + '2', 1);
	cv::namedWindow(WINDOW_NAME + '3', 1);

	button = new ButtonWindow();

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

	cv::resize(src_img, src_img, cv::Size(), 0.5, 0.5);

	binarization();
	findHand();
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////

	cv::imshow(WINDOW_NAME + '3', bin_img);
	cv::imshow(WINDOW_NAME + '2', hand_img);
//	cv::circle(src_img, maxDistPoint, 5, cv::Scalar(0, 255, 0, 0), -1);

}

void HandRecognition::binarization(){

	cv::cvtColor(src_img, temp_img, bin_type);
	if (lower[0]<upper[0]){
		cv::inRange(temp_img, cv::Scalar(lower[0], lower[1], lower[2], 0), cv::Scalar(upper[0], upper[1], upper[2], 0), bin_img);
	}
	else{
		cv::inRange(temp_img, cv::Scalar(0, lower[1], lower[2], 0), cv::Scalar(upper[0], upper[1], upper[2], 0), bin_img);
		cv::inRange(temp_img, cv::Scalar(lower[0], lower[1], lower[2], 0), cv::Scalar(180, upper[1], upper[2], 0), hand_img);

		cv::add(bin_img, hand_img, bin_img);

	}

	cv::erode(bin_img, bin_img, cv::Mat(), cv::Point(-1, -1), 2);
	cv::dilate(bin_img, bin_img, cv::Mat(), cv::Point(-1, -1), 4);
	cv::erode(bin_img, bin_img, cv::Mat(), cv::Point(-1, -1), 2);
}


void HandRecognition::findHand(){
	temp_img = bin_img.clone();
	exist_contour = false;
	hand_img = cv::Mat(bin_img.rows, bin_img.cols, CV_8U, cv::Scalar(0, 0, 0, 0));

	cv::findContours(temp_img, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	double max_size = 0;
	soatRegion(contours);

	for (std::vector<std::vector<cv::Point>>::iterator it = may_be_hand_contours.begin(); it != may_be_hand_contours.end(); it++){
		if (getFingers(*it)){
			cv::Scalar color;
			switch (mouseMode){
			case notMouse:color = cv::Scalar(255, 0, 0, 0); break;
			case isMouse:color = cv::Scalar(0, 0, 255, 0); break;
			case isTouched:color = cv::Scalar(0, 255, 0, 0); break;
			default:color = cv::Scalar(0, 0, 0, 0); break;
			}
			cv::circle(src_img, centroid, 5, color, -1);
			break;
		}
		else{
			cv::rectangle(hand_img, cv::Point(0,0),cv::Point(hand_img.cols, hand_img.rows), cv::Scalar(0, 0, 0, 0), -1);
		}
	}
	cv::imshow(WINDOW_NAME, src_img);
}


bool HandRecognition::getFingers(std::vector<cv::Point> contour){
	
	
	//輪郭の取得と輪郭画像の作成
	cv::Rect rect = cv::boundingRect(contour);
	contour = movePoints(contour, rect.x, rect.y);
	cv::Mat img = cv::Mat(rect.height,rect.width, CV_8U, cv::Scalar(0, 0, 0, 0));
	std::vector<std::vector<cv::Point>> contours;
	contours.push_back(contour);
	cv::fillPoly(img, contours, cv::Scalar(255,0,0));

	//輪郭画像から距離変換画像の作成
	cv::Mat dist_img;
	cv::distanceTransform(img.clone(), dist_img, CV_DIST_L2, 3);
	cv::Point maxDistPoint;
	this->maxDistPoint = cv::Point(maxDistPoint.x + rect.x, maxDistPoint.y + rect.y);
	cv::minMaxLoc(dist_img, NULL, NULL, NULL, &maxDistPoint);

	//重心を求め、重心と距離変換を用いて輪郭等を回転
	cv::Point centroid = getCentroid(contour);
	this->centroid = cv::Point(centroid.x+rect.x,centroid.y+rect.y);
	double cos = getCos(centroid, cv::Point(centroid.x, centroid.y + 10), maxDistPoint);
	double rad = maxDistPoint.x*(centroid.y + 10)>maxDistPoint.y*centroid.x ? acos(cos) : -acos(cos);
	contour = turnPoints(contour, rad, centroid);
	maxDistPoint = turnPoints(maxDistPoint, rad, centroid);
	cv::Rect rect2 = cv::boundingRect(contour);
	contour = movePoints(contour, rect2.x, rect2.y);
	maxDistPoint.x -= rect2.x;
	maxDistPoint.y -= rect2.y;
	centroid.x = -rect2.x;
	centroid.y = -rect2.y;
	img = cv::Mat(rect2.height, rect2.width, CV_8U);

	//回転後の画像の作成
	contours.clear();
	contours.push_back(contour);
	cv::rectangle(img, cv::Point(0, 0), cv::Point(rect2.width, rect2.height), cv::Scalar(0, 0, 0, 0), -1);
	cv::fillPoly(img, contours, cv::Scalar(255,0));


	hand_img = cv::Mat(img.rows, img.cols, CV_8U, cv::Scalar(0, 0, 0, 0));
	cv::circle(hand_img, maxDistPoint, 5, cv::Scalar(255, 0, 0, 0), -1);
	cv::circle(hand_img, centroid, 5, cv::Scalar(255, 0, 0, 0), -1);

	hand_poly.clear();
	for (int i = 0; i < 5; i++){
		fingers[i] = cv::Point(0, 0);
	}

	//近似ポリゴンの取得
	cv::approxPolyDP(cv::Mat(contour), hand_poly, rect2.height/15, true);

	if (hand_poly.size() > 3){
		//近似ポリゴンの描画
		int pnum = hand_poly.size();
		for (int i = 0; i < pnum; ++i)
			cv::line(hand_img, hand_poly[i], hand_poly[i + 1 < pnum ? i + 1 : 0], cv::Scalar(255, 0, 0), 2, CV_AA);

		//凸包の取得
		cv::convexHull(hand_poly, hand_hull, true);


		//凸包の描画
		int hnum = hand_hull.size();
		for (int i = 0; i < hnum; ++i)
			cv::line(hand_img, hand_poly[hand_hull[i]], hand_poly[hand_hull[i + 1 < hnum ? i + 1 : 0]], cv::Scalar(255,0,0,0), 2, CV_AA);

		//凹状欠損の取得
		cv::convexityDefects(hand_poly, hand_hull, convexityDefects);

		//指の候補でないものを削除&親指の確認
		bool thumb = false;
		for (std::vector<cv::Vec4i>::iterator it = convexityDefects.begin(); it!=convexityDefects.end();){
			if (getCos(*it)>std::cos(1.7) && (*it)[3]>rect2.height*40){
				if (hand_poly[(*it)[2]].y > centroid.y)//親指かどうかの判断
					thumb = true;
				it++;
			}
			else{
				it = convexityDefects.erase(it);
			}
		}

		if (convexityDefects.size() > 2){
			mouseMode = thumb ? isMouse : isTouched;
		}
		else return false;


		return true;

	}
}

POINT HandRecognition::getCentroid(){
	POINT p = POINT();
	if (exist_contour){
		cv::Point point = getCentroid(handContour);
		p.x = point.x;
		p.y = point.y;
	}
	else{
		p.x = 0; p.y = 0;
	}

	return p;
}

cv::Point HandRecognition::getCentroid(std::vector<cv::Point> contour){
	cv::Point p ;
	cv::Moments moments = cv::moments(contour);
	p.x = moments.m10 / moments.m00;
	p.y = moments.m01 / moments.m00;
	return p;
}

double HandRecognition::getCos(cv::Vec4i vec){
	return getCos(hand_poly[vec[2]], hand_poly[vec[0]], hand_poly[vec[1]]);
}

double HandRecognition::getCos(cv::Point a, cv::Point b, cv::Point c){
	return
		((b.x - a.x)*(c.x - a.x)+(b.y - a.y)*(c.y - a.y))
		/ std::sqrt(((c.x - a.x)*(c.x - a.x)+(c.y - a.y)*(c.y - a.y))
					*((b.x - a.x)*(b.x - a.x)+(b.y - a.y)*(b.y - a.y)));
}

void HandRecognition::setMouseMode(int mode){
	mouseMode = mode;
}
int HandRecognition::getMouseMode(){
	return mouseMode;
}

double HandRecognition::distance(cv::Point p1, cv::Point p2){
	return sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
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

void HandRecognition::kmeanFiltering(cv::Mat src_img, cv::Mat dimg){
	cv::Mat_<int> labels(img.size(), CV_32SC1);
	cv::Mat img;
	img = src_img.reshape(1, src_img.rows*src_img.cols);
	img.convertTo(img, CV_32F);
	cv::Mat centers;
	cv::kmeans(img, 5, labels, cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1), 5, cv::KMEANS_RANDOM_CENTERS, centers);

	cv::Mat dst_img(src_img.size(), src_img.type());
	cv::MatIterator_<cv::Vec3b> itd = dst_img.begin<cv::Vec3b>(),
		itd_end = dst_img.end<cv::Vec3b>();
	for (int i = 0; itd != itd_end; ++itd, ++i) {
		cv::Vec3f &color = centers.at<cv::Vec3f>(labels(i), 0);
		(*itd)[0] = cv::saturate_cast<uchar>(color[0]);
		(*itd)[1] = cv::saturate_cast<uchar>(color[1]);
		(*itd)[2] = cv::saturate_cast<uchar>(color[2]);
	}


}

void HandRecognition::meanShiftFiltering(cv::Mat src_img, cv::Mat dimg){
	//cv::cvtColor(src_img, gray_img, CV_BGR2GRAY);
	//cv::Canny(gray_img, canny_img, 10, 200);
	//cv::TermCriteria tc;
	//tc.epsilon = 0.1;
	//tc.maxCount = 30;
	//cv::pyrMeanShiftFiltering(src_img,src_img,8,64,0,tc);
	//std::cout << "nn" << std::endl;

}

std::vector<cv::Point> HandRecognition::movePoints(std::vector<cv::Point> points, int x, int y){
	for (std::vector<cv::Point>::iterator it = points.begin(); it != points.end(); it++){
		it->x -= x;
		it->y -= y;
	}
	return points;
}

cv::Point HandRecognition::turnPoints(cv::Point point, double turnMat[4], cv::Point center){
	double x = point.x - center.x, y = point.y - center.y;
	point.x = x*turnMat[0] + y*turnMat[1];
	point.y = x*turnMat[2] + y*turnMat[3];

	return point;

}
cv::Point HandRecognition::turnPoints(cv::Point point, double rad, cv::Point center){
	double mat[] = { std::cos(rad), -std::sin(rad), std::sin(rad), std::cos(rad) };
	return turnPoints(point,mat,center);

}
std::vector<cv::Point> HandRecognition::turnPoints(std::vector<cv::Point> points, double rad, cv::Point center){
	double mat[] = { std::cos(rad), -std::sin(rad), std::sin(rad), std::cos(rad) };
	for (std::vector<cv::Point>::iterator it = points.begin(); it != points.end(); it++){
		*it = turnPoints(*it, mat, center);
	}
	return points;

}