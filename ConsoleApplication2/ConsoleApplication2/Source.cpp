#include <iostream>
#include"HandRecognition.h"
#include"HandMouse.h"
#include"TimeCounter.h"


int main(int argc, char** argv)
{

	cv::VideoCapture cap(0); // デフォルトカメラをオープン
	cv::Mat img;
	if (!cap.isOpened()){  // 成功したかどうかをチェック
		if (argv[1] != NULL){
			img = cv::imread(argv[1], 1);
		}
		else{
			return -1;
		}
	}

	TimeCounter* tc = new TimeCounter();
	tc->add("binarization");
	tc->add("findHand");
	tc->add("Moment");
	tc->add("Dist");
	HandRecognition hand(img,tc);

	hand.update();
//	HandMouse mouse(&hand);

	while (cvWaitKey(1) == -1){
		hand.update();
//		mouse.update();
	// VideoCapture デストラクタにより，カメラは自動的に終了処理されます
	//	Sleep(10000);
	}

	std::cout << "binarization " << tc->get("binarization", true) << std::endl;
	std::cout << "findHand " << tc->get("findHand", true) << std::endl;
	std::cout << "Dist " << tc->get("Dist", true) << std::endl;
	std::cout << "Moment " << tc->get("Moment", true) << std::endl;
	Sleep(10000);
	free(tc);
	return 0;
}


