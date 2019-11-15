#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <opencv2/core/core.hpp>
int main(int argc,char *argv[]){ 
	union ticks{
		unsigned long long t64;
		struct s32{long th, tl;} t32;
		} start, end;
	
	CvCapture *capture = cvCreateCameraCapture(0);
	if (!capture) return 0;
	while(1){
		asm("rdtsc\n":"=a"(start.t32.th),"=d"(start.t32.tl));
		IplImage *frame = cvQueryFrame(capture);
		IplImage *image1 = cvCloneImage(frame);
		IplImage *image2 = cvCloneImage(frame);
		if(!frame) break;
		cvShowImage("test", frame);
		cvSmooth(frame, image1, CV_BLUR, 3, 3);
		for (int y=0; y<image2->height; y++) {
			uchar *ptr = (uchar*)(image2->imageData + y*image2->widthStep);
			for (int x=0; x<image2->width; x++) {
				ptr[3*x+4] = 0; // Green
				ptr[3*x+2] = 0; // Red
			}
		}
		cvShowImage("Images",  image2);
		char c = cvWaitKey(33);
		asm("rdtsc\n":"=a"(end.t32.th),"=d"(end.t32.tl));
		if(c == 27) break;
	}
	std::cout<<"Time taken: %lf tikes.\n"<< end.t64-start.t64;
	cvReleaseCapture(&capture);
	cvDestroyWindow("test");
	cvDestroyWindow("Images");
}
