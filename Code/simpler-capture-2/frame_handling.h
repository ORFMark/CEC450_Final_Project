
#ifndef FRAME_HANDLER
#define FRAME_HANDLER


// OpenCV include section
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


// Local include section
#include "util.h"


// 
using namespace cv;


// Functions for handling a frame queue
bool initQueue(FrameQueue * WantedFrameQueue, uint32 WantedSize);
bool destructQueue(FrameQueue * WantedFrameQueue);
bool enqueue(FrameQueue * WantedFrameQueue, Frame * WantedFrame);
bool dequeue(FrameQueue * WantedFrameQueue, Frame * DequeuedFrame);
bool isEmpty(FrameQueue * WantedFrameQueue);
bool IsFull(FrameQueue * WantedFrameQueue);


// Functions for handling a specific frame
void captureFrame(CvCapture * camToCaptureFrom, Frame * FramePlacement);
void writebackFrame(Frame * WantedFrame);


// Functions for handling frame service specific functionality
void writeBackFrameService(void* params);
void captureFrameService(void* params);

#endif // FRAME_HANDLER
