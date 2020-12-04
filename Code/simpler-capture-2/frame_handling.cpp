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

// Local include section
#include "frame_handling.h"
#include "util.h"
// 
using namespace std;
using namespace cv;

// 
Frame localFramePointer;

// 
bool initQueue(FrameQueue *frameQueueToInit, int size) {
#ifdef EMPTY_POTENTIAL_QUEUE
    if (frameQueueToInit != NULL) {
        delete frameQueueToInit;
        frameQueueToInit = NULL;
    }
    if (frameQueueToInit == NULL) frameQueueToInit = (FrameQueue*)malloc(sizeof(FrameQueue));
    #endif
	frameQueueToInit->frames = (Frame**) malloc(size * sizeof(Frame*));
	frameQueueToInit->frames_array = (Frame*) malloc(size * sizeof(Frame));
	frameQueueToInit->nextFrameIndex = 0;
	frameQueueToInit->numberOfFrames = 0;
	frameQueueToInit->starterIndex = 0;
	if (frameQueueToInit->frames != NULL
			&& frameQueueToInit->frames_array != NULL) {
		frameQueueToInit->maxSize = size;
	} else {
		frameQueueToInit->maxSize = 0;
		return false;
	}
	return true;
}

//
bool destructQueue(FrameQueue *WantedFrameQueue) {
	free(WantedFrameQueue->frames);
	free(WantedFrameQueue->frames_array);
}

// 
bool isEmpty(FrameQueue *queue) {
	return (queue->numberOfFrames <= 0);
}

// 
bool IsFull(FrameQueue *InputtedQueue) {
	return (InputtedQueue->numberOfFrames >= InputtedQueue->maxSize);
}

// 
bool enqueue(FrameQueue *WantedQueue, Frame *WantedFrame) {
	if (WantedQueue->numberOfFrames < WantedQueue->maxSize) {
		WantedQueue->frames[WantedQueue->nextFrameIndex] = WantedFrame;
		WantedQueue->frames_array[WantedQueue->nextFrameIndex].frame =
				WantedFrame->frame;
		WantedQueue->frames_array[WantedQueue->nextFrameIndex].capture_timestamp =
				WantedFrame->capture_timestamp;
		WantedQueue->frames_array[WantedQueue->nextFrameIndex].capture_timestamp_int =
				WantedFrame->capture_timestamp_int;
		++(WantedQueue->numberOfFrames);
		if (WantedQueue->nextFrameIndex >= (WantedQueue->maxSize - 1)) {
			WantedQueue->nextFrameIndex = 0;
		} else {
			++(WantedQueue->nextFrameIndex);
		}
		return true;
	} else {
		return false;
	}
	/*
	 if(queue->numberOfFrames < queue->maxSize) {
	 queue->frames[queue->nextFrameIndex] = frame;
	 queue->nextFrameIndex = queue->nextFrameIndex + 1 % queue->maxSize;
	 queue->numberOfFrames++;
	 return true;
	 }
	 return false;*/
}

// 
bool dequeue(FrameQueue *WantedQueue, Frame *DequeuedFrame) {

	if (WantedQueue->numberOfFrames > 0) {
		DequeuedFrame = WantedQueue->frames[WantedQueue->starterIndex];
		//(queue->frames[WantedQueue->starterIndex]) = NULL;
		++(WantedQueue->starterIndex);
		--(WantedQueue->numberOfFrames);
		return true;
	}
	return false;

	/*
	 Frame* frame = NULL;
	 if(queue->numberOfFrames < queue->maxSize) {
	 if(queue->nextFrameIndex != 0) {
	 frame = queue->frames[--queue->nextFrameIndex];
	 } else {
	 frame = queue->frames[queue->maxSize - 1];
	 queue->nextFrameIndex = queue->maxSize - 1;
	 }
	 queue->nextFrameIndex--;
	 }
	 return *frame;*/
}

//
void captureFrame(CvCapture *camToCaptureFrom, Frame *FramePlacement) {
	FramePlacement->frame = cvQueryFrame(camToCaptureFrom);
	FramePlacement->capture_timestamp = getTimeMsec(
			FramePlacement->capture_timestamp_int);

	/*
	 IplImage* img = cvQueryFrame(camToCaptureFrom);
	 double time = getTimeMsec();
	 Frame frame;
	 frame.frame = img;
	 frame.capture_timestamp = time;
	 return frame;*/
}

// SHOULD NOT BE UTILIZING STRINGS, AS THEY RESULT IN DYNAMIC MEMORY ALLOCATIONS
void writebackFrame(int frameNum, Frame *frame) {
	static Mat img; // Need to get rid of this
	//double msecTime = frame.capture_timestamp;
#ifdef NULLPTR_CHECKS
    if (frame != NULL) {
    #else
#warning "Should provide -DNULLPTR_CHECKS while compiling this file to minimize potential errors."
#endif
	img = cvarrToMat(frame->frame);
#ifdef NULLPTR_CHECKS
    }
    #endif
	/* Why are we adding a string onto the image?
	 String headerString = "Frame: " + to_string(frameNum) + "   msecTime: " + to_string(msecTime);
	 putText(img, headerString, cv::Point2f(100,100), cv::FONT_HERSHEY_PLAIN
	 , 2, cv::Scalar(0,0,255,255));*/
	static char LocalFileName[24] = "frames/frame_******.ppm";
	snprintf(LocalFileName, 24, "frames/frame_%06d.ppm", frameNum);
	//String fileName = "frames/frame_" + to_string(frameNum) + ".ppm";
	imwrite(LocalFileName/*fileName.c_str()*/, img);
}

// 
void writeBackFrameService(FrameQueue *frameQueue) {
	static int frameNumber = 0;
	while (!isEmpty(frameQueue)) {
		if (dequeue(frameQueue, &localFramePointer) == true) {
			writebackFrame(frameNumber, &localFramePointer);
		}
		++frameNumber;
	}
	frameNumber = 0;
}

// 
void captureFrameService(CvCapture *camToCaptureFrom, FrameQueue *frameQueue) {
	captureFrame(camToCaptureFrom, &localFramePointer);
	enqueue(frameQueue, &localFramePointer);
}
