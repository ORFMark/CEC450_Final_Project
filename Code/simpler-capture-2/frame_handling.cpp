#include <stdlib.h>
#include <iostream>
#include <string>
#include <sys/time.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "frame_handling.hpp"
#include "util.hpp"
using namespace std;
using namespace cv;


FrameQueue initQueue(int size) {
   FrameQueue theQueue;
   theQueue.frames = (Frame) malloc(size * sizeof(Frame));
   theQueue.nextFrameIndex = 0;
   theQueue.numberOfFrames = 0;
   if(theQueue.frames != NULL) {
      theQueue.maxSize = size;
    } else {
       theQueue.maxSize = 0;
    }
    return theQueue;
}
bool isEmpty(FrameQueue* queue) {
   return queue->numberOfFrames == 0;
}

bool enqueue(FrameQueue* queue, Frame* frame) {
    if(queue->numberOfFrames < queue->maxSize) {
        queue->frames[queue->nextFrameIndex] = frame;
        queue->nextFrameIndex = queue->nextFrameIndex + 1 % queue->maxSize;
        queue->numberOfFrames++;
        return true;
     }
     return false;
}

Frame dequeue(FrameQueue* queue) {
    Frame frame = NULL;
    if(queue->numberOfFrames < queue->maxSize) {
        if(queue->nextFrameIndex != 0) { 
            frame = queue->frames[--queue->nextFrameIndex];
        } else {
            frame = queue->frames[queue->maxSize - 1];
            queue->nextFrameIndex = queue->maxSize - 1;
        }
        queue->nextFrameIndex--;
    }
    return frame;
}

Frame captureFrame(CvCapture* camToCaptureFrom) {
    IplImage* img = cvQueryFrame(camToCaptureFrom);
	double time = getTimeMsec();
	Frame frame;
	frame.frame = img;
	frame.capture_timestamp = time;
    return frame;
}


void writebackFrame(int frameNum, Frame frame) {
    static Mat img;
    double msecTime = frame->capture_timestamp;
    img = cvarrToMat(frame->frame);
    String headerString = "Frame: " + to_string(frameNum) + "   msecTime: " + to_string(msecTime);
    putText(img, headerString, cv::Point2f(100,100), cv::FONT_HERSHEY_PLAIN
        , 2, cv::Scalar(0,0,255,255));
    String fileName = "frames/frame_" + to_string(frameNum) + ".ppm";
    imwrite(fileName.c_str(), img);
}

void writeBackFrameService(FrameQueue* frameQueue) {
    static int frameNumber = 0;
    while(!isEmpty(frameQueue)) {
        writebackFrame(frameNumber, dequeue(frameQueue));
        frameNumber++;
    }    
}
void captureFrameServuce(CvCapture* camToCaptureFrom, FrameQueue* frameQueue) {
    enqueue(frameQueue, captureFrame(camToCaptureFrom));
}



