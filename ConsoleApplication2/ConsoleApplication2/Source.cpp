#include <iostream>
#include"HandRecognition.h"
#include"HandMouse.h"


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

	HandRecognition hand(cap);

	hand.update();
	HandMouse mouse(&hand);

	while (cvWaitKey(1) == -1){
		hand.update();
	//	mouse.update();
	// VideoCapture �f�X�g���N�^�ɂ��C�J�����͎����I�ɏI����������܂�
	//	Sleep(10000);
	}
	return 0;
}
