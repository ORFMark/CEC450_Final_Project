#ifndef RT_TIMING
#define RT_TIMING


#include <sys/time.h>
#include <stdlib.h>

#include "util.h"

typedef struct START_END {
	uint64 start_time;
	uint64 end_time;
	int32 iterNumber;
} timeStruct;

void addStartTime(timeStruct * structToAddTo);
void addEndTime(timeStruct * structToAddTo);
void initTimeStruct(timeStruct * structToInit);
void setIterNumber(timeStruct * structToModify, int32 iterNumber);
void writeArrayOfTimeStructs(timeStruct array[], char * label,
		uint64 startTimeMsec, uint32 arrayLength);

#endif
