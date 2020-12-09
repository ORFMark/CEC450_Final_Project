/*
 *
 *  Example by Sam Siewert 
 *
 *  Updated for OpenCV 3.1
 *
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include "frame_handling.h"
#include "util.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

#define HRES 640
#define VRES 480

static struct timespec time_now, time_start, time_stop;

void * houghTransform(void * prams)
{
    FrameQueue frameQueue = (((threadParams_t) prams)->frameQueue);
    
    while(1)
    {
        int dev=0;
        Mat gray, canny_frame, cdst;
        vector<Vec4i> lines;
        static double fnow=0.0, fstart=0.0, fstop=0.0;

        clock_gettime(CLOCK_MONOTONIC, &time_now);
        fstart = (double)time_now.tv_sec + (double)time_now.tv_nsec / 1000000000.0;
        
	frame=dequeue(frameQueue,0);

        Mat mat_frame(cvarrToMat(frame));
        Canny(mat_frame, canny_frame, 50, 200, 3);

        cvtColor(canny_frame, cdst, CV_GRAY2BGR);
        cvtColor(mat_frame, gray, CV_BGR2GRAY);
	
        HoughLinesP(canny_frame, lines, 1, CV_PI/180, 50, 50, 10);

        for( size_t i = 0; i < lines.size(); i++ )
        {
          Vec4i l = lines[i];
          line(mat_frame, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
        }

     
        if(!frame) break;
    }

    clock_gettime(CLOCK_MONOTONIC, &time_now);
    fnow = (double)time_now.tv_sec + (double)time_now.tv_nsec / 1000000000.0;
    printf("Total Time: %lf\n", (fnow-fstart));    
};
