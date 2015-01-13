#include"ButtonWindow.h"

void callback(int event, int x, int y, int flags, void* param){

}

ButtonWindow::ButtonWindow(){

	cv::namedWindow(winName, 1);
	cv::resizeWindow(winName, 200, 40);
	cv::setMouseCallback(winName,callback);
	mat.create(200, 40, CV_8UC3);
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 2; j++){
			cv::rectangle(mat, cv::Point(i*20,j*20), cv::Point((i+1)*20,(j+1)*20), cv::Scalar((i+j)%2*255,0,(i+j+1)%2*255));
		}
	}
	cv::imshow(winName, mat);
}