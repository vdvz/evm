#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <cstdio>
#include <opencv2/core/core.hpp>
int main(){ 
	union ticks{
		unsigned long long t64;
		struct s32{long th, tl;} t32;
	} start_frames, end_frames;
	
	struct timespec start, end, start_proc, end_proc;
	float count_frames = 0;
	double input_time=0;
	double conversion_time=0;
	double show_time=0;

	CvCapture *capture = cvCreateCameraCapture(0);
	if (!capture) return 0;
	while(1){
		asm("rdtsc\n":"=a"(start_frames.t32.th),"=d"(start_frames.t32.tl));
		count_frames++;
		
		clock_gettime(CLOCK_MONOTONIC_RAW, &start_proc);
		IplImage *frame = cvQueryFrame(capture);
		if(!frame) break;
		clock_gettime(CLOCK_MONOTONIC_RAW, &end_proc);
		input_time += (end_proc.tv_sec - start_proc.tv_sec) + 0.000000001 * (end_proc.tv_nsec - start_proc.tv_nsec);
		
		clock_gettime(CLOCK_MONOTONIC_RAW, &start_proc);
		IplImage *image1 = cvCloneImage(frame);
		IplImage *image2 = cvCloneImage(frame);	
		//cvSmooth(frame, image1, CV_BLUR, 3, 3);
		for (int y=0; y<image2->height; y++) {
			uchar *ptr = (uchar*)(image2->imageData + y*image2->widthStep);
			for (int x=0; x<image2->width; x++) {
				ptr[3*x+4] = 0; // Green
				ptr[3*x+2] = 0; // Red
			}
		}
		clock_gettime(CLOCK_MONOTONIC_RAW, &end_proc);
		conversion_time += (end_proc.tv_sec - start_proc.tv_sec) + 0.000000001 * (end_proc.tv_nsec - start_proc.tv_nsec);
		
		clock_gettime(CLOCK_MONOTONIC_RAW, &start_proc);
		cvShowImage("Image1",  image1);
		cvShowImage("Image2",  image2);
		clock_gettime(CLOCK_MONOTONIC_RAW, &end_proc);
		show_time += (end_proc.tv_sec - start_proc.tv_sec) + 0.000000001 * (end_proc.tv_nsec - start_proc.tv_nsec);
		
		char c = cvWaitKey(33);
		asm("rdtsc\n":"=a"(end_frames.t32.th),"=d"(end_frames.t32.tl));
		std::cout<<"Frames per second: "<< end_frames.t64-start_frames.t64 <<"\n";
		if(c == 27) break;
	}
	clock_gettime(CLOCK_MONOTONIC, &end);
	double time = (end.tv_sec - start.tv_sec) + 0.000000001 * (end.tv_nsec - start.tv_nsec);
	printf("time taken : %lf sec\n", time);
	printf("fps  : %lf \n", count_frames / time);
	printf("input_time taken : %lf%%\n", input_time/time*100);
	printf("conversion_time taken : %lf%%\n", conversion_time/time*100);
	printf("show_time taken : %lf%%\n", show_time/time*100);
/*
	std::cout<<"full time:"<< time<<" sec. \n";
	std::cout<<"fps"<<count_frames / time<<"\n";
	std::cout<<"input time: "<<input_time/time*100<<"\n";
	std::cout<<"conversion time: "<<conversion_time/time*100<<"\n";
	std::cout<<"show time: "<< show_time/time*100<<"\n";
*/
	cvReleaseCapture(&capture);
	cvDestroyWindow("Image1");
	cvDestroyWindow("Image2");
}
