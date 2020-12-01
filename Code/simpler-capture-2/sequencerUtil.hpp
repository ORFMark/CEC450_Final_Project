#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>
#include <sched.h>
#include <time.h>
#include <semaphore.h>

#include <sys/sysinfo.h>

#define USEC_PER_MSEC (1000)
#define NUM_THREADS (3)
