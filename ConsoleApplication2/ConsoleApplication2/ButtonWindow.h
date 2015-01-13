#include"Setting.h"


class ButtonWindow{
public:
	ButtonWindow();
	~ButtonWindow();
	const std::string winName = "buttons";
private:
	cv::Mat mat;
	static void callback(int, int, int, int, void*);
};