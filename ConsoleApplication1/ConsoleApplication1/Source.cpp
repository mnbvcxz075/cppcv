
#include "opencv\cv.h"
#include "opencv\highgui.h"
#include <ctype.h>

int
main(int argc, char **argv)
{
	CvCapture *capture = 0;
	IplImage *frame = 0;
	double w = 320, h = 240;
	int c;

	// (1)�R�}���h�����ɂ���Ďw�肳�ꂽ�ԍ��̃J�����ɑ΂���L���v�`���\���̂��쐬����
	if (argc == 1 || (argc == 2 && strlen(argv[1]) == 1 && isdigit(argv[1][0])))
		capture = cvCreateCameraCapture(argc == 2 ? argv[1][0] - '0' : 0);

	/* ���̐ݒ�́C���p����J�����Ɉˑ����� */
	// (2)�L���v�`���T�C�Y��ݒ肷��D
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, w);
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, h);

	cvNamedWindow("Capture", CV_WINDOW_AUTOSIZE);

	// (3)�J��������摜���L���v�`������
	while (1) {
		frame = cvQueryFrame(capture);
		cvShowImage("Capture", frame);
		c = cvWaitKey(2);
		if (c == '\x1b')
			break;
	}

	cvReleaseCapture(&capture);
	cvDestroyWindow("Capture");

	return 0;
}
