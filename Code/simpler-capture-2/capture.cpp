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
#include <sys/time.h>
#include <iostream>
#include <syslog.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>

using namespace cv;
using namespace std;


//Defined resolution in project, can choose something else but decide not to.
#define HRES 640
#define VRES 480
#define PROJECT_TAG "BG_RT_Final"
bool USE_PRINTF = true;




double getTimeMsec(void);
void log(string thingToLog, int logLevel);
void log(string thingToLog);

int main( int argc, char** argv )
{
    if (argc < 2) {
        printf("Usage: capture -v for printf, -s for syslog");
        return 0;
    }
    else if(argc == 2) {
        string argument(argv[1]);
        if (argument.compare("v"))
            USE_PRINTF = true;
        else
            USE_PRINTF = false;
    }
    double event_time, run_time=0.0;
    cvNamedWindow("Capture Example", CV_WINDOW_AUTOSIZE);
    CvCapture* capture = cvCreateCameraCapture(0);
    IplImage* frame;

    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, HRES);
    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, VRES);

    //Starting the capture of individual frames from camera
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
           log("Pthread Policy is SCHED_FIFO\n");
           break;
     case SCHED_OTHER:
           log("Pthread Policy is SCHED_OTHER\n");
       	   break;
     case SCHED_RR:
           log("Pthread Policy is SCHED_RR\n");
           break;
     default:
       log("Pthread Policy is UNKNOWN\n");
   }

}

void log(string thingToLog) {
   log(thingToLog, LOG_INFO);
}

void log(string thingToLog, int logLevel) {
    struct timeval tv;
   gettimeofday(&tv, (struct timezone *) 0);
   if(USE_PRINTF) {
     cout << PROJECT_TAG << ": SEC:USEC; " << tv.tv_sec << ":" << tv.tv_usec 
        << "; " << thingToLog << endl;
   }
     syslog(logLevel, "%s; SEC:USEC; %d:%d; %s", PROJECT_TAG
        , tv.tv_sec, tv.tv_usec, thingToLog);
   
}
