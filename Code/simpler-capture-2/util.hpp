#ifndef UTIL_C
#define UTIL_C
#include <stdlib.h>
#include <sys/time.h>

typedef struct
{
    int threadIdx;
    unsigned long long sequencePeriods;
	CVCapture* camera;
	FrameQueue* frameQueue;
} threadParams_t;

double getTimeMsec(void);
void print_scheduler(void);
#endif
