
#ifndef FRAME_HANDLER
#define FRAME_HANDLER


// OpenCV include section
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


// Local include section
#include "util.h"


// It would be better to not utilize this
using namespace cv;


// Functions for handling a frame queue
bool initQueue(FrameQueue * WantedFrameQueue, int WantedSize);
bool destructQueue(FrameQueue * WantedFrameQueue);
bool enqueue(FrameQueue * WantedFrameQueue, Frame WantedFrame);
bool dequeue(FrameQueue * WantedFrameQueue, Frame * DequeuedFrame);
bool isEmpty(FrameQueue * WantedFrameQueue);
bool IsFull(FrameQueue * WantedFrameQueue);


// Functions for handling a specific frame
void captureFrame(cv::VideoCapture * CameraToCaptureFrom, Frame * FramePlacement);
void writebackFrame(Frame * WantedFrame);


// Functions for handling frame service specific functionality
void writeBackFrameService(FrameQueue * WantedFrameQueue);
void captureFrameService(cv::VideoCapture * CameraToCaptureFrom, FrameQueue * WantedFrameQueue);

#endif // FRAME_HANDLER
