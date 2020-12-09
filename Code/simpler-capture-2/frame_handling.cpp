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

pthread_mutex_t QUEUE_LOCK = PTHREAD_MUTEX_INITIALIZER;
// 
Frame localFramePointer;
uint64 LocalFrameCounter = 0;
char LocalFileName[24] = "frames/frame_******.ppm";
char LocalScreenText[44] = "Frame: ******; msecTime: ************";
Mat LocalImage;
uint64 LocalFileTime;

// 
bool initQueue(FrameQueue *WantedFrameQueue, uint32 WantedSize) {
	pthread_mutex_lock(&QUEUE_LOCK);
	WantedFrameQueue->ArrayOfFrames = (Frame*) malloc(
			WantedSize * sizeof(Frame));
	WantedFrameQueue->nextFrameIndex = 0;
	WantedFrameQueue->numberOfFrames = 0;
	WantedFrameQueue->starterIndex = 0;
	if (WantedFrameQueue->ArrayOfFrames != NULL) {
		WantedFrameQueue->maxSize = WantedSize;
		pthread_mutex_unlock(&QUEUE_LOCK);
		return true;
	} else {
		pthread_mutex_unlock(&QUEUE_LOCK);
		WantedFrameQueue->maxSize = 0;
		return false;
	}
}

// 
bool destructQueue(FrameQueue *WantedFrameQueue) {
	pthread_mutex_lock(&QUEUE_LOCK);
	free(WantedFrameQueue->ArrayOfFrames);
	pthread_mutex_unlock(&QUEUE_LOCK);
	return true;
}

// 
bool isEmpty(FrameQueue *WantedFrameQueue) {
	pthread_mutex_lock(&QUEUE_LOCK);
	bool isEmpty = (WantedFrameQueue->numberOfFrames <= 0);
	pthread_mutex_unlock(&QUEUE_LOCK);
	return isEmpty;
}

// 
bool IsFull(FrameQueue *WantedFrameQueue) {
	pthread_mutex_lock(&QUEUE_LOCK);
	bool isFull =
			(WantedFrameQueue->numberOfFrames >= WantedFrameQueue->maxSize);
	pthread_mutex_unlock(&QUEUE_LOCK);
	return isFull;
}

// 
bool enqueue(FrameQueue *WantedFrameQueue, Frame *WantedFrame) {
	pthread_mutex_lock(&QUEUE_LOCK);
	if (WantedFrameQueue->numberOfFrames < WantedFrameQueue->maxSize) {
		WantedFrameQueue->ArrayOfFrames[WantedFrameQueue->nextFrameIndex].frame =
				WantedFrame->frame;
		WantedFrameQueue->ArrayOfFrames[WantedFrameQueue->nextFrameIndex].CaptureTimestamp =
				WantedFrame->CaptureTimestamp;
		++(WantedFrameQueue->numberOfFrames);
		if (WantedFrameQueue->nextFrameIndex
				>= (WantedFrameQueue->maxSize - 1)) {
			WantedFrameQueue->nextFrameIndex = 0;
		} else {
			++(WantedFrameQueue->nextFrameIndex);
		}
		pthread_mutex_unlock(&QUEUE_LOCK);
		return true;
	} else {
		pthread_mutex_unlock(&QUEUE_LOCK);
		return false;
	}
}

// 
bool dequeue(FrameQueue *WantedQueue, Frame *DequeuedFrame) {
	if (WantedQueue->numberOfFrames > 0) {
		pthread_mutex_lock(&QUEUE_LOCK);
		DequeuedFrame = &WantedQueue->ArrayOfFrames[WantedQueue->starterIndex];
		//DequeuedFrame->MatFrame = WantedQueue->ArrayOfFrames[WantedQueue->starterIndex]->MatFrame;
		//DequeuedFrame->CaptureTimestamp = WantedQueue->ArrayOfFrames[WantedQueue->starterIndex]->CaptureTimestamp;
		++(WantedQueue->starterIndex);
		--(WantedQueue->numberOfFrames);
		pthread_mutex_unlock(&QUEUE_LOCK);
		return true;
	}
	return false;
}

//
void captureFrame(CvCapture *camToCaptureFrom, Frame *FramePlacement) {
	FramePlacement->frame = *cvQueryFrame(camToCaptureFrom);
	getTimeMsec(&FramePlacement->CaptureTimestamp);
}

// 
void writebackFrame(Frame *WantedFrame) {
	LocalImage = cvarrToMat(&WantedFrame->frame);
	getTimeMsec(&LocalFileTime);

	if (LocalFrameCounter >= 10) {
		snprintf(LocalScreenText, 44, "Frame: %06lu; msecTime: %014lu",
				LocalFrameCounter - 10, LocalFileTime);
		snprintf(LocalFileName, 24, "frames/frame_%06lu.ppm",
				LocalFrameCounter = 10);
		putText(LocalImage, LocalScreenText, cv::Point2f(20, 20),
				cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 0, 255, 255));
		cv::imwrite(LocalFileName, LocalImage);
	}

	++LocalFrameCounter;
}

// 
void writeBackFrameService(void *params) {
	FrameQueue *frameQueue = (((threadParams_t*) params)->frameQueue);
	while (!isEmpty(frameQueue)) {
		if (dequeue(frameQueue, &localFramePointer) == true) {
			writebackFrame(&localFramePointer);
		}
	}
}

// 
void captureFrameService(void *params) {
	CvCapture *camToCaptureFrom = (((threadParams_t*) params)->camera);
		FrameQueue *frameQueue = (((threadParams_t*) params)->frameQueue);
	captureFrame(camToCaptureFrom, &localFramePointer);

#ifdef USE_PRINTF
    if (enqueue(frameQueue, &localFramePointer) == false) {
        std::cout << std::endl << "ERROR: Failed to queue frame captured at " <<
            localFramePointer.CaptureTimestamp << std::endl <<
            "nsecs\tReason: queue is full" << std::endl;
    }
#else
	enqueue(frameQueue, &localFramePointer);
#endif
}
