#include"Setting.h"


class ButtonWindow{
public:
	ButtonWindow();
	~ButtonWindow();
	const std::string winName = "buttons";
private:
	cv::Mat mat;

	void callback(int event, int x, int y, int flags, void* param){

	}
};