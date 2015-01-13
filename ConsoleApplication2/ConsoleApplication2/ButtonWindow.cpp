#include"ButtonWindow.h"


bool ButtonWindow::buttons[col*low];

ButtonWindow::ButtonWindow()
{
	for (int i = 0; i < col; i++){
		for (int j = 0; j < low; j++){
			buttons[i*low + j] = false;
			
		}
	}
	cv::namedWindow(winName);
	cv::resizeWindow(winName, 200, 40);
	cv::setMouseCallback(winName,ButtonWindow::callback);
	mat.create(low*buttonSize,col*buttonSize, CV_8UC3);
	for (int i = 0; i < col; i++){
		for (int j = 0; j < low; j++){
			cv::rectangle(mat, cv::Point(i*buttonSize,j*buttonSize), cv::Point((i+1)*buttonSize,(j+1)*buttonSize), cv::Scalar((i+j)%2*255,0,(i+j+1)%2*255),-1);
		}
	}
	cv::imshow(winName, mat);
}

void ButtonWindow::callback(int event, int x, int y, int flags, void* param){
	for (int i = 0; i < col; i++){
		for (int j = 0; j < low; j++){
			if (buttonSize*(i + 1)>x&&x > buttonSize*i&&buttonSize*(j + 1)>y&&y > buttonSize*j){
				setButton(true, i + j*col);
			}
		}
	}
}

void ButtonWindow::setButton(bool b,int i){
	buttons[i] = b;
}
bool ButtonWindow::getButton(int i){
	return buttons[i];
}

void ButtonWindow::changeNum(int i, int num){
	nums[i]=std::to_string(num);

	for (int i = 0; i < col; i++){
		for (int j = 0; j < low; j++){
			cv::rectangle(mat, cv::Point(i*buttonSize, j*buttonSize), cv::Point((i + 1)*buttonSize, (j + 1)*buttonSize), cv::Scalar((i + j) % 2 * 255, 0, (i + j + 1) % 2 * 255),-1);
			if (i == 1)
				cv::putText(mat, nums[j], cv::Point(j*buttonSize, buttonSize * 2), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 0, 0, 0),3);
		}
	}

	cv::imshow(winName, mat);

}