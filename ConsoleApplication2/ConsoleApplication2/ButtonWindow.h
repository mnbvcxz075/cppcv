#include"Setting.h"


class ButtonWindow{
public:
	ButtonWindow();
	~ButtonWindow();
	const std::string winName = "buttons";
private:
	cv::Mat mat;
	 void static callback(int, int, int, int, void*);
	 const int low=2, col=8, buttonSize=40;
};