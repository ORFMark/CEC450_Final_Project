
// Global include section
#include <stdlib.h>
#include <iostream>
//#include <string>
#include <sys/time.h>


// OpenCV include section
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>


// Local include section
#include "frame_handling.h"


// 
//using namespace std;
//using namespace cv;


// 
Frame localFramePointer;
uint64 LocalFrameCounter = 0;
char LocalFileName[24] = "frames/frame_******.ppm";


// 
bool initQueue(FrameQueue * WantedFrameQueue, int WantedSize) {
    printf("0");
    WantedFrameQueue->ArrayOfFrames = (Frame *)malloc(WantedSize * sizeof(Frame));
    printf("2");
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
        WantedFrameQueue->ArrayOfFrames[WantedFrameQueue->nextFrameIndex].MatFrame = WantedFrame->MatFrame;
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
void captureFrame(cv::VideoCapture * CameraToCaptureFrom, Frame * FramePlacement) {
    (*CameraToCaptureFrom) >> FramePlacement->MatFrame;
    getTimeMsec(&FramePlacement->CaptureTimestamp);
}


// 
void writebackFrame(Frame * WantedFrame) {
    snprintf(LocalFileName, 24, "frames/frame_%06d.ppm", LocalFrameCounter);
    cv::imwrite(LocalFileName, (WantedFrame->MatFrame));
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
void captureFrameService(cv::VideoCapture * CameraToCaptureFrom, FrameQueue * WantedFrameQueue) {
    captureFrame(CameraToCaptureFrom, &localFramePointer);
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
