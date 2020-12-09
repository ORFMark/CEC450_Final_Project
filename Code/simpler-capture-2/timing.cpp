#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "timing.h"
#include "util.h"
void addStartTime(timeStruct *structToAddTo) {
	if (structToAddTo != NULL) {
		getTimeMsec(&structToAddTo->start_time);
	}
}
void addEndTime(timeStruct *structToAddTo) {
	if (structToAddTo != NULL) {
		getTimeMsec(&structToAddTo->end_time );
	}
}

void setIterNumber(timeStruct *structToModify, int iterNumber) {
	if (structToModify != NULL) {
		structToModify->iterNumber = iterNumber;
	}
}
void writeArrayOfTimeStructs(timeStruct array[], char *label,
		double startTimeMsec, int arrayLength) {
	int i = 0;

	FILE *f = fopen(label, "w");

	for (i = 0; i < arrayLength; i++) {
		fprintf(f, "%d:%lf:%lf\n", i,
				array[i].start_time - startTimeMsec,
				array[i].end_time - startTimeMsec);
	}
	fclose(f);

}
