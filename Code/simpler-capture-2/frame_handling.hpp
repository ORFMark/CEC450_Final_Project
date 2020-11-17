#ifndef FRAME_HANDLER
#define FRAME_HANDLER
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;
typedef struct FRAME_QUEUE {
    int nextFrameIndex;
    int numberOfFrames;
    int maxSize;
    IplImage** frames;
} FrameQueue;

FrameQueue initQueue(int size);
bool enqueue(FrameQueue* queue, IplImage* frame);
IplImage* dequeue(FrameQueue* queue);
bool isEmpty(FrameQueue* queue);

IplImage* captureFrame(CvCapture* camToCaptureFrom);
void writebackFrame(int frameNum, IplImage* frame);

void writeBackFrameService(int frameNumber, FrameQueue* frameQueue);
void captureFrameServuce(CvCapture* camToCaptureFrom, FrameQueue* frameQueue);
#endif
