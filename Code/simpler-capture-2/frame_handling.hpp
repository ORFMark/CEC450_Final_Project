#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

typedef struct FRAME_QUEUE {
    IplImage* frame[];
    int nextFrameIndex;
    int lastFrameIndex;
} FrameQueue;

FrameQueue initQueue(int size);
int enqueue(FrameQueue* queue, IplImage* frame);
IplImage* dequeue(FrameQueue* queue);

void captureFrame()
void writebackFrame(int frameNum, )
