#include"HandRecognition.h"
#include<cmath>
#include<time.h>
int HandRecognition::req_x, HandRecognition::req_y;

HandRecognition::HandRecognition(){
	req_x = req_y = -1;
	upper[0] = 30;
	lower[0] = 148;
	upper[1] = 150;
	lower[1] = 0;
	upper[2] = 255;
	lower[2] = 200;

	mouseMode = notMouse;
	log = new HandLog();

	fingers = new cv::Point[NUM_OF_FINGER];

	cv::namedWindow(WINDOW_NAME, 1);
	cv::setMouseCallback(WINDOW_NAME, HandRecognition::callback);
	cv::namedWindow(WINDOW_NAME + '3', 1);
	cv::setMouseCallback(WINDOW_NAME+'3', HandRecognition::callback2);
	cv::namedWindow(WINDOW_NAME + '2', 1);

	UVSkinTable = (byte*)malloc(256 * 256 * 256 / 8);
	//	initUVSkinTable();
	button = new ButtonWindow();

}
HandRecognition::HandRecognition(cv::Mat img)
:HandRecognition()
{
	this->img = img;
	src_img = img.clone();

}
HandRecognition::HandRecognition(cv::Mat img, TimeCounter* tc)
: HandRecognition(img)
{
	this->tc = tc;
}
HandRecognition::HandRecognition(cv::VideoCapture cap)
: HandRecognition()
{
	this->capture = cap;
	capture >> src_img;
}
HandRecognition::HandRecognition(cv::VideoCapture cap, TimeCounter* tc)
: HandRecognition(cap)
{
	this->tc = tc;
}
HandRecognition::~HandRecognition(){
	free(UVSkinTable);
}

void HandRecognition::update(){
	//処理画像の取得
	if (capture.isOpened())
		capture >> src_img;
	else
		src_img = img.clone();

	cv::flip(src_img, src_img, 1);
	handContour.clear();

	updateButtons();

	tc->start("binarization");
	binarization();
	tc->stop("binarization");
	tc->start("findHand");
	findHand();
	tc->stop("findHand");

	cv::imshow(WINDOW_NAME, src_img);
	cv::imshow(WINDOW_NAME+'3', bin_img);

}

void HandRecognition::binarization(){
	cv::cvtColor(src_img, temp_img, bin_type);
	if (lower[0] < upper[0]){
		cv::inRange(temp_img, cv::Scalar(lower[0], lower[1], lower[2], 0), cv::Scalar(upper[0], upper[1], upper[2], 0), bin_img);
	}
	else{

		tc->start("inRange");
		cv::inRange(temp_img, cv::Scalar(0, lower[1], lower[2], 0), cv::Scalar(upper[0], upper[1], upper[2], 0), bin_img);
		cv::inRange(temp_img, cv::Scalar(lower[0], lower[1], lower[2], 0), cv::Scalar(180, upper[1], upper[2], 0), hand_img);
		tc->stop("inRange");
		cv::add(bin_img, hand_img, bin_img);
	}

	tc->start("noize");
	cv::erode(bin_img, bin_img, cv::Mat(), cv::Point(-1, -1), 2);
	cv::dilate(bin_img, bin_img, cv::Mat(), cv::Point(-1, -1), 4);
	cv::erode(bin_img, bin_img, cv::Mat(), cv::Point(-1, -1), 2);
	tc->stop("noize");
}
void HandRecognition::binarization2(){

	bin_img = cv::Mat(src_img.rows, src_img.cols, CV_8U);
	byte* src_it = src_img.data;
	byte* src_end = src_img.data + src_img.rows * src_img.cols * 3;
	byte* bin_it = bin_img.data;
	byte* table_it = UVSkinTable;

	while (src_it != src_end){
		if (1){
			table_it = UVSkinTable;
			table_it += *src_it * 256 * 256 / 8;
			src_it++;
			table_it += *src_it * 256 / 8;
			src_it++;
			table_it += *src_it / 8;
			src_it++;
			int a = *table_it;
			int b = std::pow(2, *src_it % 8);
			*bin_it = *table_it&(byte)std::pow(2, *src_it % 8) != 0 ? 255 : 0;
			bin_it++;
		}
		else{
			src_it += 3;
			bin_it++;

		}
	}

	cv::erode(bin_img, bin_img, cv::Mat(), cv::Point(-1, -1), 2);
	cv::dilate(bin_img, bin_img, cv::Mat(), cv::Point(-1, -1), 4);
	cv::erode(bin_img, bin_img, cv::Mat(), cv::Point(-1, -1), 2);
}

void HandRecognition::initUVSkinTable(){
	byte* it = UVSkinTable;
	int i = 0;
	*it = 0;
	for (int i = 0; i < 256; i++){
		for (int j = 0; j < 256; j++){
			for (int k = 0; k < 256; k++){
				double u = k*0.2468 - j*0.5318 + i*0.2850 - 7.3738;
				double v = k*0.4665 - j*0.0478 - i*0.4178;
				if (u>-14 && 10>u&&v>-1 && 33 > v){
					//std::cout << i << ","<<j<<","<<k<<std::endl;
					switch (k % 8){
					case 0:*it += 0x00000001; break;
					case 1:*it += 0x00000010; break;
					case 2:*it += 0x00000100; break;
					case 3:*it += 0x00001000; break;
					case 4:*it += 0x00010000; break;
					case 5:*it += 0x00100000; break;
					case 6:*it += 0x01000000; break;
					case 7:*it += 0x10000000; break;
					}
				}
				if (k % 8 == 7){
					it++;
					*it = 0;
				}
			}
		}
	}


}

void HandRecognition::findHand(){
	temp_img = bin_img.clone();
	hand_img = cv::Mat(bin_img.rows, bin_img.cols, CV_8U, cv::Scalar(0, 0, 0, 0));

	cv::findContours(temp_img, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
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
			//cv::circle(src_img, centroid, 5, color, -1);
			break;
		}
		else{
			cv::rectangle(hand_img, cv::Point(0, 0), cv::Point(hand_img.cols, hand_img.rows), cv::Scalar(0, 0, 0, 0), -1);
		}
	}
}

bool HandRecognition::getFingers(std::vector<cv::Point> contour){
	//輪郭がはみ出ていたら手でないと判定
	cv::Rect rect = cv::boundingRect(contour);
	if (rect.x == 1 || rect.y == 1 || rect.x + rect.width > src_img.cols - 1 || rect.y + rect.height > src_img.rows - 1)
		return false;

	//近似ポリゴンの取得
	cv::approxPolyDP(cv::Mat(contour), hand_poly, rect.height / 15, true);
	if (hand_poly.size() < 4)
		return false;

	//近似ポリゴンの描画
	for (int i = 0; i < hand_poly.size(); ++i)
		cv::line(hand_img, hand_poly[i], hand_poly[i + 1 < hand_poly.size() ? i + 1 : 0], cv::Scalar(255, 0, 0), 2, CV_AA);

	//凸包の取得
	cv::convexHull(hand_poly, hand_hull, true);

	//凸包の描画
	int hnum = hand_hull.size();
	for (int i = 0; i < hnum; ++i)
		cv::line(hand_img, hand_poly[hand_hull[i]], hand_poly[hand_hull[i + 1 < hnum ? i + 1 : 0]], cv::Scalar(255, 0, 0, 0), 2, CV_AA);

	//凹状欠損の取得
	cv::convexityDefects(hand_poly, hand_hull, convexityDefects);
	if (convexityDefects.size() < 2)//
		return false;

	std::vector<cv::Point> fingers;
	std::vector<cv::Point> temp;

	//指の候補でないものを削除&親指の確認
	for (std::vector<cv::Vec4i>::iterator it = convexityDefects.begin(); it != convexityDefects.end();){
		double cos = UsePoints::getCos(hand_poly[(*it)[2]], hand_poly[(*it)[0]], hand_poly[(*it)[1]]);
		std::cout << (*it)[3] / 256 << std::endl;
		if (cos>std::cos(1.7) && (*it)[3] > 18000){
			temp.push_back(hand_poly[(*it)[0]]);
			temp.push_back(hand_poly[(*it)[1]]);
			it++;
		}
		else{
			it = convexityDefects.erase(it);
		}
	}
	cv::Moments moment = cv::moments(contour);
	int  finger_width = std::sqrt(moment.m00) / 3;

	//同じ指のダブりを消す
	for (int i = 0; i < temp.size() / 2; i++){
		int a = (i * 2 + 1) % temp.size();
		int b = (i * 2 + 2) % temp.size();
		double d = UsePoints::distance(temp[a], temp[b]);
		if (d < finger_width){
			fingers.push_back(cv::Point((temp[(i * 2 + 1) % temp.size()].x + temp[(i * 2 + 2) % temp.size()].x) / 2
				, (temp[(i * 2 + 1) % temp.size()].y + temp[(i * 2 + 2) % temp.size()].y) / 2));
		}
		else{
			fingers.push_back(temp[(i * 2 + 1) % temp.size()]);
			fingers.push_back(temp[(i * 2 + 2) % temp.size()]);
		}
	}
	//std::cout << fingers.size() << std::endl;
	if (fingers.size() < 1){//指先候補が２本以下なら手でない
		return false;
	}

	if (fingers.size() >5){//6本以上でも……ねえ？
		return false;
	}




	//輪郭画像の作成
	contour = UsePoints::movePoints(contour, rect.x, rect.y);
	fingers = UsePoints::movePoints(fingers, rect.x, rect.y);
	cv::Mat img = cv::Mat(rect.height, rect.width, CV_8U, cv::Scalar(0, 0, 0, 0));
	std::vector<std::vector<cv::Point>> contours;
	contours.push_back(contour);
	cv::fillPoly(img, contours, cv::Scalar(255, 0, 0));

	//輪郭画像から距離変換画像の作成
	tc->start("Dist");
	cv::Mat dist_img;
	cv::distanceTransform(img.clone(), dist_img, CV_DIST_L2, 3);
	tc->stop("Dist");

	//距離最大の点と最大距離の取得
	cv::Point maxDistPoint;
	double maxDistance;
	cv::minMaxLoc(dist_img, NULL, &maxDistance, NULL, &maxDistPoint);
	this->maxDistPoint = cv::Point(maxDistPoint.x + rect.x, maxDistPoint.y + rect.y);

	//重心を求める
	tc->start("Moment");
	cv::Point centroid = getCentroid(contour);
	this->centroid = cv::Point(centroid.x + rect.x, centroid.y + rect.y);
	tc->stop("Moment");

	//重心と距離変換を用いて輪郭等を回転
	double cos = UsePoints::getCos(centroid, cv::Point(centroid.x, centroid.y + 10), maxDistPoint);
	double rad = maxDistPoint.x -centroid.x >0? acos(cos) : -acos(cos);
	contour = UsePoints::turnPoints(contour, rad, centroid);
	fingers = UsePoints::turnPoints(fingers, rad, centroid);
	maxDistPoint = UsePoints::turnPoint(maxDistPoint, rad, centroid);

	//左上に寄せる
	cv::Rect rect2 = cv::boundingRect(contour);
	contour = UsePoints::movePoints(contour, rect2.x, rect2.y);
	fingers = UsePoints::movePoints(fingers, rect2.x, rect2.y);
	maxDistPoint.x -= rect2.x;
	maxDistPoint.y -= rect2.y;
	centroid.x -= rect2.x;
	centroid.y -= rect2.y;

	//回転後の画像の作成
	img = cv::Mat(rect2.height, rect2.width, CV_8U, cv::Scalar(0, 0, 0));
	contours.clear();
	contours.push_back(contour);
	cv::fillPoly(img, contours, cv::Scalar(255, 0));


	hand_img = cv::Mat(img.rows, img.cols, CV_8U, cv::Scalar(0, 0, 0, 0));
	cv::circle(img, maxDistPoint, 5, cv::Scalar(100, 0, 0, 0), -1);
	cv::circle(img, centroid, 5, cv::Scalar(200, 0, 0, 0), -1);
	cv::circle(bin_img, this->maxDistPoint, 5, cv::Scalar(100, 0, 0, 0), -1);
	cv::circle(bin_img, this->centroid, 5, cv::Scalar(200, 0, 0, 0), -1);


	for (cv::Point p : fingers){
		cv::Point point = cv::Point(p.x, p.y);
		//cv::circle(img, point, 5, cv::Scalar(200, 0, 0, 0), -1);
	}

	//指のlogを保存
	log->setFingers(fingers, centroid);
	fingers = log->getFingers();
	for (cv::Point p : fingers){
		cv::Point point = cv::Point(p.x + centroid.x, p.y + centroid.y);
		//cv::circle(img, point, 5, cv::Scalar(100, 0, 0, 0), -1);
	}
	cv::putText(bin_img, std::to_string(finger_width) + "," + std::to_string(maxDistance), cv::Point(0, 30), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(120, 0, 0, 0), 3);
	if (getScreen){
		getScreenImage(bin_img);
	}
	cv::imshow(WINDOW_NAME + '2', img);
	//for (int i = 0; i<5; i++){
	//	std::cout << log->existFingers[i];
	//}
	//std::cout << std::endl;


	if (fingers.size() > 2){
		mouseMode = log->existFingers[thumb] ? isTouched : isMouse;
	}
	else {
		mouseMode = notMouse;
		return false;
	}


	return true;


}

POINT HandRecognition::getCentroid(){
	POINT p = POINT();
	p.x = centroid.x;
	p.y = centroid.y;

	return p;
}

cv::Point HandRecognition::getCentroid(std::vector<cv::Point> contour){
	cv::Point p;
	cv::Moments moments = cv::moments(contour);
	p.x = moments.m10 / moments.m00;
	p.y = moments.m01 / moments.m00;
	return p;
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
	double sizes[] = { 0, 0, 0 };
	for (std::vector<cv::Point> con : contour){
		area = cv::contourArea(con);
		if (area > 640 * 480 / 100){
			std::vector<std::vector<cv::Point>>::iterator it = may_be_hand_contours.begin();
			for (int i = 0; i < 3; i++){
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

void HandRecognition::callback(int event, int x, int y, int flags, void* param){
	if (event == CV_EVENT_LBUTTONDOWN){
		req_x = x;
		req_y = y;
		mousemouse = mousemouse ? false : true;
	}
}
void HandRecognition::callback2(int event, int x, int y, int flags, void* param){
	if (event == CV_EVENT_LBUTTONUP){
		getScreen = true;
	}
}

void HandRecognition::updateButtons(){
	for (int i = 0; i < button->low; i++){
		for (int j = 0; j < button->col; j++){
			if (button->getButton(j + i*button->col)){
				if (i == 0){
					if (j % 2 == 0)
						button->changeNum(j, ++upper[j / 2]);
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
}

bool HandRecognition::getScreenImage(cv::Mat img){
	time_t now = time(NULL);
	struct tm *pnow = localtime(&now);
	std::cout << "D:/desktop/" + std::to_string(pnow->tm_year + 1900) + std::to_string(pnow->tm_mon + 1) + std::to_string(pnow->tm_mday)
		+ std::to_string(pnow->tm_hour) + std::to_string(pnow->tm_min) + std::to_string(pnow->tm_sec) << std::endl;

	cv::imwrite("D:/desktop/" + std::to_string(pnow->tm_year + 1900) + std::to_string(pnow->tm_mon + 1) + std::to_string(pnow->tm_mday)
		+ std::to_string(pnow->tm_hour) + std::to_string(pnow->tm_min) + std::to_string(pnow->tm_sec)+".png", img);
	getScreen = false;
	return true;
}
bool HandRecognition::mousemouse = false;
bool HandRecognition::getScreen = false;