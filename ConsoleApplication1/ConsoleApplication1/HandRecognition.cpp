#include"HandRecognition.h"

HandRecognition::HandRecognition(CvCapture* cap){
	this->capture = cap;
	this->window_name = window_name;

	while (src_img == NULL){
		src_img = cvQueryFrame(capture);
		printf("a");
	}
	printf("%p",src_img);
	bin_img = cvCreateImage(cvGetSize(src_img), IPL_DEPTH_8U, 1);
	hand_img = cvCreateImage(cvGetSize(src_img), IPL_DEPTH_8U, 1);
	dist_img = cvCreateImage(cvGetSize(src_img), IPL_DEPTH_32F, 3);	
	mem = cvCreateMemStorage(0);

}
HandRecognition::~HandRecognition(){
	cvReleaseImage(&hand_img);
	cvReleaseImage(&bin_img);
	cvReleaseImage(&dist_img);
	cvReleaseMemStorage(&mem);
}

/*
src_imgからバイナリイメージを生成しbin_imgへ
*/
void HandRecognition::binaryzation(){

	temp_img = cvCreateImage(cvGetSize(src_img), IPL_DEPTH_8U, 3);

	//2値価
	cvCvtColor(src_img,temp_img,CV_BGR2HSV);
	cvInRangeS(temp_img, min_thre, max_thre, bin_img);

	//ノイズ除去
	cvErode(bin_img, bin_img, NULL, 4);
	cvDilate(bin_img, bin_img, NULL, 6);
	cvErode(bin_img, bin_img, NULL, 2);


	cvReleaseImage(&temp_img);
}

void HandRecognition::findHand(){

	temp_img = cvCreateImage(cvGetSize(src_img), IPL_DEPTH_8U, 1);
	cvCopy(bin_img, temp_img);

	int con = cvFindContours(temp_img, mem, &contours, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	//exist_contour = con > 0;

	double max = 0;
	CvSeq *ptr = contours;
	for (; ptr != 0; ptr = ptr->h_next){
		double area = cvContourArea(ptr, CV_WHOLE_SEQ);
		if (area>max){
			max = area;
			hand_contour = ptr;
		}
	}

	cvDrawContours(src_img,hand_contour,cvScalar(255,0,255,255),cvScalar(255,255,255,255),0);
	cvReleaseImage(&temp_img);

	cvRectangle(hand_img, cvPoint(0, 0), cvPoint(src_img->width, src_img->height), cvScalar(0, 0, 0, 0), -1, 4, 0);
	cvDrawContours(hand_img, hand_contour, CV_RGB(255, 255, 255), CV_RGB(255, 255, 255), -1, CV_FILLED, 8);

}

void HandRecognition::update(){
	src_img = NULL;
	while (src_img == NULL){
		src_img = cvQueryFrame(capture);
		printf("c");
	}


		binaryzation();
		findHand();
		setCentroid();
////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////

		cvShowImage(MYWINDOW_NAME, src_img);

}

void HandRecognition::setCentroid(){
	cvMoments(hand_contour, &moments);
	centroid.y = (int)(moments.m01 / moments.m00);
	centroid.x = (int)(moments.m10 / moments.m00);
	cvCircle(src_img, cvPoint(centroid.x, centroid.y), 10, cvScalar(0, 0, 255, 0), -1, 4, 0);

}

POINT HandRecognition::getCentroid(){
	return centroid;
}