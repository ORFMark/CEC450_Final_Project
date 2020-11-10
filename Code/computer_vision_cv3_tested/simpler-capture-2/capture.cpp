/*
 *
 *  Example by Sam Siewert 
 *  Edited by Mark Burrell and Drew Grobmeier
 *  Updated 11/10/2020 for Final Project
 *  Updated 10/29/16 for OpenCV 3.1
 *
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <thread>

#define NUM_THREADS (3) // sequencer, logger, and initializer

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;


//Defined resolution in project, can choose something else but decide not to.
#define HRES 640
#define VRES 480
#define MY_SCHEDULER SCHED_FIFO
#define MY_SCHEDULER SCHED_OTHER

void captureSequence();
double getTimeMsec(void);

typedef struct
{
    int threadIdx;
} threadParams_t;


int main( int argc, char** argv )
{
    double event_time, run_time=0.0;
    pthread_t threads[NUM_THREADS];
    pthread_attr_t rt_sched_attr[NUM_THREADS];
    int rt_max_prio, rt_min_prio;
    struct sched_param rt_param[NUM_THREADS];
    struct sched_param main_param;
    pthread_attr_t main_attr;
    pid_t mainpid;

    mainpid=getpid();


    cvNamedWindow("Capture Example", CV_WINDOW_AUTOSIZE);
    CvCapture* capture = cvCreateCameraCapture(0);
    IplImage* frame;

    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, HRES);
    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, VRES);

    //Starting the capture of individual frames from camera

    //going to get rid of this to incorporate threading. 
 
    while(1)
    {
        frame=cvQueryFrame(capture);
     
        if(!frame) break;

        cvShowImage("Capture Example", frame);

        char c = cvWaitKey(33);
        if( c == 'q' ) break;
	event_time = getTimeMsec();
	run_time = getTimeMsec() - event_time;

    }
	
    cvReleaseCapture(&capture);
    cvDestroyWindow("Capture Example");
    
};

void captureSequence() {
	

}

double getTimeMsec(void)
{
  struct timespec event_ts = {0, 0};

  clock_gettime(CLOCK_MONOTONIC, &event_ts);
  return ((event_ts.tv_sec)*1000.0) + ((event_ts.tv_nsec)/1000000.0);
}

void print_scheduler(void)
{
   int schedType;

   schedType = sched_getscheduler(getpid());

   switch(schedType)
   {
     case SCHED_FIFO:
           printf("Pthread Policy is SCHED_FIFO\n");
           break;
     case SCHED_OTHER:
           printf("Pthread Policy is SCHED_OTHER\n");
       	   break;
     case SCHED_RR:
           printf("Pthread Policy is SCHED_RR\n");
           break;
     default:
       printf("Pthread Policy is UNKNOWN\n");
   }

}
