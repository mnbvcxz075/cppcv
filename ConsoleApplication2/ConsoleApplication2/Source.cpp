#include <iostream>
#include"HandRecognition.h"
#include"HandMouse.h"
#include"TimeCounter.h"


int main(int argc, char** argv)
{

	cv::VideoCapture cap(0); // �f�t�H���g�J�������I�[�v��
	cv::Mat img;
	if (!cap.isOpened()){  // �����������ǂ������`�F�b�N
		if (argv[1] != NULL){
			img = cv::imread(argv[1], 1);
		}
		else{
			return -1;
		}
	}

	TimeCounter* tc = new TimeCounter();
	HandRecognition hand(cap,tc);

	hand.update();
//	HandMouse mouse(&hand);
	int timer = timeGetTime();
	while (cvWaitKey(1) == -1){
		hand.update();
//		mouse.update();
	//	Sleep(1000);
	// VideoCapture �f�X�g���N�^�ɂ��C�J�����͎����I�ɏI����������܂�
	}

	std::cout << "binarize " << tc->get("binarize", true) << std::endl;
	std::cout << "findHand " << tc->get("findHand", true) << std::endl;
	std::cout << "Dist " << tc->get("Dist", true) << std::endl;
	std::cout << "Moment " << tc->get("Moment", true) << std::endl;
	std::cout << "inRange " << tc->get("inRange", true) << std::endl;
	std::cout << "noize " << tc->get("noize", true) << std::endl;
	Sleep(10000);
	free(tc);
	return 0;
}


