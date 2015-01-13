#include <iostream>
#include"HandRecognition.h"
#include"HandMouse.h"
#include"ButtonWindow.h"


int main(int argc, char** argv)
{
	cv::VideoCapture cap(1); // デフォルトカメラをオープン
	cv::Mat img;
	if (!cap.isOpened()){  // 成功したかどうかをチェック
		if (argv[1] != NULL){
			img = cv::imread(argv[1], 1);
		}
		else{
			return -1;
		}
	}

	HandRecognition hand(cap);
	ButtonWindow buttons();

	HandMouse mouse(&hand);

	while (cvWaitKey(1) == -1){
		hand.update();
		mouse.update();
	// VideoCapture デストラクタにより，カメラは自動的に終了処理されます
	}
	return 0;
}