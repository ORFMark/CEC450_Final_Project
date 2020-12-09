
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
struct timespec event_ts = {0, 0};
static const char StaticLogInfo[4][32] = {
     "Pthread Policy is SCHED_FIFO\n\0",
     "Pthread Policy is SCHED_OTHER\n\0",
     "Pthread Policy is SCHED_RR\n\0",
     "Pthread Policy is UNKNOWN\n\0"
};


// Function for returning the clock time
//    Direct memory access is faster and more efficient on weaker
//    hardware, along with integers being faster than floats/doubles
void getTimeMsec(uint64 * WantedNanosecondsElapsed) {
     clock_gettime(CLOCK_MONOTONIC, &event_ts);
     (*WantedNanosecondsElapsed) = ((event_ts.tv_sec * 1000) + event_ts.tv_nsec);
}


// Function for logging which pthread policy is currently in use
void print_scheduler(void) {
     switch(sched_getscheduler(getpid())) {
          case SCHED_FIFO:
               log((char *)"Pthread Policy is SCHED_FIFO\n");
               break;
          case SCHED_OTHER:
               log((char *)"Pthread Policy is SCHED_OTHER\n");
               break;
          case SCHED_RR:
               log((char *)"Pthread Policy is SCHED_RR\n");
               break;
          default:
               log((char *)"Pthread Policy is UNKNOWN\n");
     }
}
