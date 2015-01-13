#include"ButtonWindow.h"



ButtonWindow::ButtonWindow(){

	cv::namedWindow(winName);
	cv::resizeWindow(winName, 200, 40);
	cv::setMouseCallback(winName,ButtonWindow::callback);
	mat.create(low*buttonSize,col*buttonSize, CV_8UC3);
	for (int i = 0; i < col; i++){
		for (int j = 0; j < low; j++){
			cv::rectangle(mat, cv::Point(i*buttonSize,j*buttonSize), cv::Point((i+1)*buttonSize,(j+1)*buttonSize), cv::Scalar((i+j)%2*255,0,(i+j+1)%2*255));
		}
	}
	cv::imshow(winName, mat);
}

void ButtonWindow::callback(int event, int x, int y, int flags, void* param){

}