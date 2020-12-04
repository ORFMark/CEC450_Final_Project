// Global include section
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <pthread.h>

// Local include section
#include "util.h"
#include "logging.h"

// "Local" global variables as to remove the constant reallocation of cache memory
struct timespec event_ts = { 0, 0 };
//int schedType = -1;

// Function for returning the clock time 
double getTimeMsec(u_int64_t WantedNanosecondsElapsed) {
	clock_gettime(CLOCK_MONOTONIC, &event_ts);
	WantedNanosecondsElapsed = ((event_ts.tv_sec * 1000) + event_ts.tv_nsec);
	printf("%lu - %f", WantedNanosecondsElapsed,
			((event_ts.tv_sec * 1000.0) + (event_ts.tv_nsec * (1.0 / 1000000.0))));
	return (event_ts.tv_sec * 1000.0) + (event_ts.tv_nsec * (1.0 / 1000000.0));
}

// Function for logging which pthread policy is currently in use
void print_scheduler(void) {
	switch (sched_getscheduler(getpid())) {
	case SCHED_FIFO:
		log("Pthread Policy is SCHED_FIFO\n");
		break;
	case SCHED_OTHER:
		log("Pthread Policy is SCHED_OTHER\n");
		break;
	case SCHED_RR:
		log("Pthread Policy is SCHED_RR\n");
		break;
	default:
		log("Pthread Policy is UNKNOWN\n");
	}
}
