#include"HandRecognition.h"

//int main()
//{
//
//	CvCapture *capture = cvCreateCameraCapture(0);
//	if (capture == NULL)return -1;
//
//	cvNamedWindow(MYWINDOW_NAME, CV_WINDOW_AUTOSIZE);
//
////	HandRecognition *hand = new HandRecognition(capture);
//	while (1){
//		//hand->update();
//		IplImage *image = cvQueryFrame(capture);
//
//		// �E�B���h�E�ɉ摜��\������
//		cvShowImage(MYWINDOW_NAME, image);
//
//	}
////	hand->~HandRecognition();
//	// �r�f�I�L���v�`�����������
//	cvReleaseCapture(&capture);
//
//	// �E�B���h�E��j������
//	cvDestroyWindow(MYWINDOW_NAME);
//	return 0;
//}
//


//#include <stdio.h>
//#include "opencv\highgui.h"

int main(int argc,char**argv)
{
	// �J��������̃r�f�I�L���v�`��������������
	CvCapture *videoCapture = cvCreateCameraCapture(0);
	if (videoCapture == NULL&&argv[1]==NULL)
	{
		return -1;
	}


	// �E�B���h�E���쐬����
	cvNamedWindow(MYWINDOW_NAME, CV_WINDOW_AUTOSIZE);
	cvNamedWindow(MYWINDOW_NAME+'2', CV_WINDOW_AUTOSIZE);

	HandRecognition *hand = videoCapture!= NULL
		?new HandRecognition(videoCapture)
		:new HandRecognition(cvLoadImage(argv[1], CV_LOAD_IMAGE_COLOR));
	// �����L�[�����������܂ŁA���[�v������Ԃ�
	while (cvWaitKey(1) == -1)
	{
		// �J��������1�t���[���擾����
		IplImage *image = cvQueryFrame(videoCapture);

		// �E�B���h�E�ɉ摜��\������
		hand->update();
		//cvShowImage(MYWINDOW_NAME, image);
	}
//	while (cvWaitKey(1) == -1){}

	// �r�f�I�L���v�`�����������
	cvReleaseCapture(&videoCapture);

	// �E�B���h�E��j������
	cvDestroyWindow(MYWINDOW_NAME);

	return 0;
}