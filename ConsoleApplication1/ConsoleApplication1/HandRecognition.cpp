#include"HandRecognition.h"

HandRecognition::HandRecognition(CvCapture* cap){
	this->capture = cap;
	this->window_name = window_name;

	while (src_img == NULL){
		src_img = cvQueryFrame(capture);
		printf("a");
	}
	printf("%p", src_img);
	bin_img = cvCreateImage(cvGetSize(src_img), IPL_DEPTH_8U, 1);
	hand_img = cvCreateImage(cvGetSize(src_img), IPL_DEPTH_8U, 1);
	dist_img = cvCreateImage(cvGetSize(src_img), IPL_DEPTH_8U, 1);
	mem = cvCreateMemStorage(0);

}
HandRecognition::HandRecognition(IplImage* img){

	if (img == NULL)
		return;
	capture = NULL;
	src_img = img;
	bin_img = cvCreateImage(cvGetSize(src_img), IPL_DEPTH_8U, 1);
	hand_img = cvCreateImage(cvGetSize(src_img), IPL_DEPTH_8U, 1);
	dist_img = cvCreateImage(cvGetSize(src_img), IPL_DEPTH_32F, 1);
	mem = cvCreateMemStorage(0);

}
HandRecognition::~HandRecognition(){
	cvReleaseImage(&src_img);
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
	cvCvtColor(src_img,temp_img,CV_BGR2Lab);
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

//	cvDrawContours(src_img,hand_contour,cvScalar(255,0,255,255),cvScalar(255,255,255,255),0);
	cvReleaseImage(&temp_img);

	cvRectangle(hand_img, cvPoint(0, 0), cvPoint(src_img->width, src_img->height), cvScalar(0, 0, 0, 0), -1, 4, 0);
	cvDrawContours(hand_img, hand_contour, CV_RGB(255, 255, 255), CV_RGB(255, 255, 255), -1, CV_FILLED, 8);

}

void HandRecognition::makeDistTransform(){

}

void HandRecognition::update(){
	if (capture != NULL){
		src_img = NULL;
		while (src_img == NULL){
			src_img = cvQueryFrame(capture);
			printf("c");
		}
	}

		binaryzation();
		findHand();
		setCentroid();
////////////////////////////////////////////////////////////////////

		temp_img = cvCreateImage(cvGetSize(src_img), IPL_DEPTH_32F, 1);
		cvDistTransform(hand_img, temp_img, CV_DIST_L2);
		cvNormalize(temp_img, dist_img, 0.0, 255.0, CV_MINMAX, NULL);
		int max_dist=0;
		POINT max_point;
		unsigned char* ptr = (unsigned char*)dist_img->imageData;
		for (int i = 0; dist_img->height > i; i++){
			for (int j = 0; dist_img->width > j; j++){
				//printf("%d\n",*ptr);
				if (max_dist<=*ptr){
					max_dist = *ptr;
					max_point.x = j;
					max_point.y = i;
				}
				ptr++;
			}
		}
		cvReleaseImage(&temp_img);

		cvCircle(hand_img, cvPoint(max_point.x, max_point.y), 10, cvScalar(0, 255, 0, 0), -1, 4, 0);
////////////////////////////////////////////////////////////////////

		cvShowImage(MYWINDOW_NAME, hand_img);
		cvShowImage(MYWINDOW_NAME + '2', dist_img);

}

void HandRecognition::setCentroid(){
	cvMoments(hand_contour, &moments);
	centroid.y = (int)(moments.m01 / moments.m00);
	centroid.x = (int)(moments.m10 / moments.m00);
	cvCircle(hand_img, cvPoint(centroid.x, centroid.y), 10, cvScalar(0, 0, 255, 0), -1, 4, 0);

}

POINT HandRecognition::getCentroid(){
	return centroid;
}