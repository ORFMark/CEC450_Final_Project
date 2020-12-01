#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "util.hpp"
#include "logging.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

double getTimeMsec(void)
{
  struct timespec event_ts = {0, 0};

  clock_gettime(CLOCK_MONOTONIC, &event_ts);
  return ((event_ts.tv_sec)*1000.0) + ((event_ts.tv_nsec)/1000000.0);
}

void print_scheduler(void)
{
   int schedType;

   schedType = sched_getscheduler(getpid());

   switch(schedType)
   {
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

