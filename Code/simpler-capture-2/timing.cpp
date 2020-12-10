#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "timing.h"
#include "util.h"

double getTimeMsec(void)
{
  struct timespec event_ts = {0, 0};

  clock_gettime(CLOCK_MONOTONIC_RAW, &event_ts);
  return ((event_ts.tv_sec)*1000.0) + ((event_ts.tv_nsec)/1000000.0);
}

void addStartTime(timeStruct * structToAddTo) {
	if (structToAddTo != NULL) {
		structToAddTo->start_time = getTimeMsec();
	}
}
void addEndTime(timeStruct * structToAddTo) {
	if (structToAddTo != NULL) {
		structToAddTo->end_time = getTimeMsec();
	}
}

void setIterNumber(timeStruct * structToModify, int32 iterNumber) {
	if (structToModify != NULL) {
		structToModify->iterNumber = iterNumber;
	}
}
void writeArrayOfTimeStructs(timeStruct array[], char *label,
		double startTimeMsec, uint32 arrayLength) {
	int i = 0;

	FILE *f = fopen(label, "w");

	for (i = 0; i < arrayLength; i++) {
		fprintf(f, "%l:%lf:%lf", array[i].iterNumber,
				array[i].start_time - startTimeMsec,
				array[i].end_time - startTimeMsec);
	}
	fclose(f);

}
