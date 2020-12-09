#ifndef RT_TIMING
#define RT_TIMING


#include <sys/time.h>
#include <stdlib.h>


void addStartTime(timeStruct *structToAddTo);
void addEndTime(timeStruct *structToAddTo);
void initTimeStruct(timeStruct *structToInit);
void setIterNumber(timeStruct *structToModify, int iterNumber);
void writeArrayOfTimeStructs(timeStruct array[], char *label,
		double startTimeMsec, int arrayLength);

#endif