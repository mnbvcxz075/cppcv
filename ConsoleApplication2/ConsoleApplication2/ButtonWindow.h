#include"Setting.h"


class ButtonWindow{
public:
	ButtonWindow();
	~ButtonWindow();
	const std::string winName = "buttons";
	static const int low = 3, col = 6, buttonSize = 40;

	static void setButton(bool, int);
	static bool getButton(int);
	void changeNum(int,int);
private:
	cv::Mat mat;
	 void static callback(int, int, int, int, void*);
	 static bool buttons[low*col];
	 std::string nums[col];
};