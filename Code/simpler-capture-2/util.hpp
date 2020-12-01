#ifndef UTIL_C
#define UTIL_C
#include <stdlib.h>
#include <sys/time.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#define USEC_PER_MSEC (1000)
#define NANOSEC_PER_SEC (1000000000)
#define NUM_CPU_CORES (1)
#define TRUE (1)
#define FALSE (0)

typedef struct threadParams_t {
	FrameQueue* queue;
	CvCapture* camToCaptureFrom;
	int frameNum;
	pthread_mutex_t lock;
} threadParams_t;


double getTimeMsec(void);
void print_scheduler(void);
#endif
