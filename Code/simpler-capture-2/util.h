#ifndef UTIL_C
#define UTIL_C

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <stdlib.h>
#include <sys/time.h>

typedef struct FRAME {
	IplImage *frame;
	double capture_timestamp;
} Frame;

typedef struct FRAME_QUEUE {
	int nextFrameIndex;
	int starterIndex;
	int numberOfFrames;
	int maxSize;
	Frame **frames;
	Frame frames_array[];
} FrameQueue;

typedef struct {
	int threadIdx;
	unsigned long long sequencePeriods;
	CvCapture *camera;
	FrameQueue *frameQueue;
} threadParams_t;

double getTimeMsec(void);
void print_scheduler(void);
#endif
