#include"Setting.h"

class HandRecognition
{
public:
	HandRecognition(CvCapture*);
	~HandRecognition();


private:
	IplImage *src_img,*bin_img,*temp_img,*dist_img,*hand_img;
	CvSeq* contours;
	CvSeq* hand_contour;
	CvMemStorage* mem;
	CvMoments moments;
	CvCapture *capture;
	CvScalar max_thre = cvScalar(30, 128, 255, 255), min_thre = cvScalar(0, 0, 0, 0);
	POINT centroid;
	bool exist_contour;
	char* window_name;

	void setCameraImage();
	POINT getCentroid();
	void setCentroid();
	void binaryzation();
	void findHand();


public:
	void update();
};

