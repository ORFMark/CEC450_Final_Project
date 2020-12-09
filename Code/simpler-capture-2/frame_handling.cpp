
// Global include section
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>


// OpenCV include section
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>


// Local include section
#include "frame_handling.h"
#include "util.h"


// 
//using namespace std;
using namespace cv;


// 
Frame localFramePointer;
uint64 LocalFrameCounter = 0;
char LocalFileName[24] = "frames/frame_******.ppm";
char LocalScreenText[44] = "Frame: ******; msecTime: ************";
Mat LocalImage;
uint64 LocalFileTime;


// 
bool initQueue(FrameQueue * WantedFrameQueue, uint32 WantedSize) {
    WantedFrameQueue->ArrayOfFrames = (Frame *)malloc(WantedSize * sizeof(Frame));
    WantedFrameQueue->nextFrameIndex = 0;
    WantedFrameQueue->numberOfFrames = 0;
    WantedFrameQueue->starterIndex = 0;
    if(WantedFrameQueue->ArrayOfFrames != NULL) {
      WantedFrameQueue->maxSize = WantedSize;
      return true;
    } else {
       WantedFrameQueue->maxSize = 0;
       return false;
    }
}


// 
bool destructQueue(FrameQueue * WantedFrameQueue) {
    free(WantedFrameQueue->ArrayOfFrames);
    return true;
}


// 
bool isEmpty(FrameQueue * WantedFrameQueue) {
   return (WantedFrameQueue->numberOfFrames <= 0);
}


// 
bool IsFull(FrameQueue * WantedFrameQueue) {
    return (WantedFrameQueue->numberOfFrames >= WantedFrameQueue->maxSize);
}


// 
bool enqueue(FrameQueue * WantedFrameQueue, Frame * WantedFrame) {
    if(WantedFrameQueue->numberOfFrames < WantedFrameQueue->maxSize) {
        WantedFrameQueue->ArrayOfFrames[WantedFrameQueue->nextFrameIndex].frame = WantedFrame->frame;
        WantedFrameQueue->ArrayOfFrames[WantedFrameQueue->nextFrameIndex].CaptureTimestamp = WantedFrame->CaptureTimestamp;
        ++(WantedFrameQueue->numberOfFrames);
        if (WantedFrameQueue->nextFrameIndex >= (WantedFrameQueue->maxSize - 1)) {
            WantedFrameQueue->nextFrameIndex = 0;
        } else {
            ++(WantedFrameQueue->nextFrameIndex);
        }
        return true;
     } else {
        return false;
     }
}


// 
bool dequeue(FrameQueue * WantedQueue, Frame * DequeuedFrame) {
    if (WantedQueue->numberOfFrames > 0) {
        DequeuedFrame = &WantedQueue->ArrayOfFrames[WantedQueue->starterIndex];
        //DequeuedFrame->MatFrame = WantedQueue->ArrayOfFrames[WantedQueue->starterIndex]->MatFrame;
        //DequeuedFrame->CaptureTimestamp = WantedQueue->ArrayOfFrames[WantedQueue->starterIndex]->CaptureTimestamp;
        ++(WantedQueue->starterIndex);
        --(WantedQueue->numberOfFrames);
        return true;
    }
    return false;
}


//
void captureFrame(CvCapture * camToCaptureFrom, Frame * FramePlacement) {
    *FramePlacement->frame = cvQueryFrame(camToCaptureFrom);
	getTimeMsec(&FramePlacement->CaptureTimestamp);
}


// 
void writebackFrame(Frame * WantedFrame) {
    LocalImage = cvarrToMat(&WantedFrame->frame);
    getTimeMsec(&LocalFileTime);
    snprintf(LocalScreenText, 44, "Frame: %06lu; msecTime: %014lu", LocalFrameCounter, LocalFileTime);
    snprintf(LocalFileName, 24, "frames/frame_%06lu.ppm", LocalFrameCounter);
    putText(LocalImage, LocalScreenText, cv::Point2f(20, 20), cv::FONT_HERSHEY_PLAIN, 1,
			cv::Scalar(0, 0, 255, 255));
    cv::imwrite(LocalFileName, LocalImage);
    ++LocalFrameCounter;
}


// 
void writeBackFrameService(FrameQueue * WantedFrameQueue) {
    while(!isEmpty(WantedFrameQueue)) {
        if (dequeue(WantedFrameQueue, &localFramePointer) == true) {
            writebackFrame(&localFramePointer);
        }
    }
}


// 
void captureFrameService(CvCapture * camToCaptureFrom, FrameQueue * WantedFrameQueue) {
    captureFrame(camToCaptureFrom, &localFramePointer);
#ifdef USE_PRINTF
    if (enqueue(WantedFrameQueue, &localFramePointer) == false) {
        std::cout << std::endl << "ERROR: Failed to queue frame captured at " <<
            localFramePointer.CaptureTimestamp << std::endl <<
            "nsecs\tReason: queue is full" << std::endl;
    }
#else
    enqueue(WantedFrameQueue, &localFramePointer);
#endif
}
