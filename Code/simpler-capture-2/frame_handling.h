#ifndef FRAME_HANDLER
#define FRAME_HANDLER
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "util.h"
using namespace cv;

bool initQueue(FrameQueue *wantedFrameQueue, int size);
bool enqueue(FrameQueue *queue, Frame frame);
Frame dequeue(FrameQueue *queue);
bool isEmpty(FrameQueue *queue);
bool isFull(FrameQueue *queue);
void destructQueue(FrameQueue *WantedFrameQueue);
void captureFrame(CvCapture *camToCaptureFrom, Frame *wantedFrame);
void writebackFrame(int frameNum, Frame frame);

void* writeBackFrameService(void *prams);
void* captureFrameService(void *prams);
#endif
