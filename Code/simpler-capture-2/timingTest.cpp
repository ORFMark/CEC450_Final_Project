#include <stdio.h>
#include <stdlib.h>
#include "frame_handling.h"
#include "timing.h";
#include "util.h";

#define INTERATIONS 100



int main(void) {
	timeStruct timeArray[INTERATIONS];
	FrameQueue queue;
	CvCapture* camera = cvCreateCameraCapture(0);
	initQueue(&queue, 256);
	threadParams_t prams;
	prams.camera = camera;
	prams.frameQueue = &frameQueue;
	double startTime = getTimeMsec();
	int i = 0;
	for(i = 0; i < INTERATIONS; i++) {
		addStartTime(timeArray[i]);
		captureFrameService((void*) &prams);
		addEndTime(timeArray[i]);
	}
	writeArrayOfTimeStructs(timeArray, "captureTest.txt",
			startTime, INTERATIONS);
	startTime = getTimeMsec();
	for(i = 0; i < INTERATIONS; i++) {
			addStartTime(timeArray[i]);
			writeBackFrameService((void*) &prams);
			addEndTime(timeArray[i]);
		}
	writeArrayOfTimeStructs(timeArray, "writebackTimingTest.txt",
				startTime, INTERATIONS);
	return 0;
}
