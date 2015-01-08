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
//		// ウィンドウに画像を表示する
//		cvShowImage(MYWINDOW_NAME, image);
//
//	}
////	hand->~HandRecognition();
//	// ビデオキャプチャを解放する
//	cvReleaseCapture(&capture);
//
//	// ウィンドウを破棄する
//	cvDestroyWindow(MYWINDOW_NAME);
//	return 0;
//}
//


//#include <stdio.h>
//#include "opencv\highgui.h"

int main(int argc,char**argv)
{
	// カメラからのビデオキャプチャを初期化する
	CvCapture *videoCapture = cvCreateCameraCapture(0);
	if (videoCapture == NULL&&argv[1]==NULL)
	{
		return -1;
	}


	// ウィンドウを作成する
	cvNamedWindow(MYWINDOW_NAME, CV_WINDOW_AUTOSIZE);
	cvNamedWindow(MYWINDOW_NAME+'2', CV_WINDOW_AUTOSIZE);

	HandRecognition *hand = videoCapture!= NULL
		?new HandRecognition(videoCapture)
		:new HandRecognition(cvLoadImage(argv[1], CV_LOAD_IMAGE_COLOR));
	// 何かキーが押下されるまで、ループをくり返す
	while (cvWaitKey(1) == -1)
	{
		// カメラから1フレーム取得する
		IplImage *image = cvQueryFrame(videoCapture);

		// ウィンドウに画像を表示する
		hand->update();
		//cvShowImage(MYWINDOW_NAME, image);
	}
//	while (cvWaitKey(1) == -1){}

	// ビデオキャプチャを解放する
	cvReleaseCapture(&videoCapture);

	// ウィンドウを破棄する
	cvDestroyWindow(MYWINDOW_NAME);

	return 0;
}