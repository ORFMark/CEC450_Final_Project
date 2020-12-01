#ifndef FRAME_HANDLER
#define FRAME_HANDLER
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "util.hpp"
using namespace cv;



FrameQueue initQueue(int size);
bool enqueue(FrameQueue* queue, Frame frame);
Frame dequeue(FrameQueue* queue);
bool isEmpty(FrameQueue* queue);

Frame captureFrame(CvCapture* camToCaptureFrom);
void writebackFrame(int frameNum, Frame frame);

void* writeBackFrameService(void* params);
void* captureFrameService(void* params);
#endif
