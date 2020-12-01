#ifndef FRAME_HANDLER
#define FRAME_HANDLER
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "util.hpp"
using namespace cv;

typedef struct FRAME {
	IplImage* image;
	double capture_timestamp;
} Frame;

typedef struct FRAME_QUEUE {
    int nextFrameIndex;
    int numberOfFrames;
    int maxSize;
    Frame* frames;
} FrameQueue;

FrameQueue initQueue(int size);
bool enqueue(FrameQueue* queue, Frame frame);
Frame dequeue(FrameQueue* queue);
bool isEmpty(FrameQueue* queue);

Frame captureFrame(CvCapture* camToCaptureFrom);
void writebackFrame(int frameNum, Frame frame);

void writeBackFrameService(void* params);
void captureFrameServuce(void* params);
#endif
