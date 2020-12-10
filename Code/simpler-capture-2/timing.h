#ifndef RT_TIMING
#define RT_TIMING


#include <sys/time.h>
#include <stdlib.h>

#include "util.h"

typedef struct START_END {
	double start_time;
	double end_time;
	int32 iterNumber;
} timeStruct;

void addStartTime(timeStruct * structToAddTo);
void addEndTime(timeStruct * structToAddTo);
void initTimeStruct(timeStruct * structToInit);
void setIterNumber(timeStruct * structToModify, int32 iterNumber);
void writeArrayOfTimeStructs(timeStruct array[], char * label,
		double startTimeMsec, uint32 arrayLength);
double getTimeMsec(void);
#endif
