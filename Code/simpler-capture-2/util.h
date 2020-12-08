<<<<<<< HEAD

#ifndef UTIL_C
#define UTIL_C


// OpenCV include section
#include <opencv2/opencv.hpp>
=======
#ifndef UTIL_C
#define UTIL_C

>>>>>>> 33a0cad75190ecd462740fece63a11cbae4682bf
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

<<<<<<< HEAD

// Global include section
#include <stdlib.h>
#include <sys/time.h>


// Simple type definitions for optimizing memory usage - This is
// 	important due to the small L1 cache of the target hardware
typedef unsigned char uint8;	   // 1 byte  [0 .. 255]
typedef unsigned short uint16;	   // 2 bytes [0 .. 255]
typedef unsigned long uint32;	   // 4 bytes [0 .. 255]
typedef unsigned long long uint64; // 8 bytes [0 .. 255]
typedef signed char int8;	   // 1 byte  [-127 .. 128]
typedef signed short int16;	   // 2 bytes [0 .. 255]
typedef signed long int32;	   // 4 bytes [0 .. 255]
typedef signed long long int64;	   // 8 bytes [0 .. 255]


// Struct for all information regarding a single frame
typedef struct FRAME {
	cv::Mat MatFrame;
	uint64 CaptureTimestamp;
} Frame; // 8+? bytes


// Struct for all information regarding a circular queue of frames
typedef struct FRAME_QUEUE {
	uint32 nextFrameIndex;
	uint32 numberOfFrames;
	uint32 maxSize;
	uint32 starterIndex;   // Also known as the "head" of the queue
	Frame * ArrayOfFrames;
} FrameQueue; // 17 bytes


// Struct for all information regarding thread accessibles
typedef struct THREAD_PARAMS {
	int32 threadIdx;
	uint64 sequencePeriods;
	uint32 CurrentQueueIndex;
} threadParams_t; // 16 bytes


// Function definitions
void getTimeMsec(uint64 * WantedNanosecondsElapsed);
void print_scheduler(void);

#endif // UTIL_C
=======
#include <stdlib.h>
#include <sys/time.h>

typedef struct FRAME {
	IplImage *frame;
	int capture_timestamp_int;
	double capture_timestamp;
} Frame;

typedef struct FRAME_QUEUE {
	int nextFrameIndex;
	int starterIndex;
	int numberOfFrames;
	int maxSize;
	Frame **frames;
	Frame *frames_array;
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
>>>>>>> 33a0cad75190ecd462740fece63a11cbae4682bf
