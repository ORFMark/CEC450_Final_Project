#define _GNU_SOURCE

#ifndef SEQUENCER_UTIL_HPP
#define  SEQUENCER_UTIL_HPP
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>
#include <sched.h>
#include <time.h>
#include <semaphore.h>

#include <sys/sysinfo.h>


#define SEQUENCER_MAX_CYCLES 1500 // runtime = (0.02 * 1500); 30 seconds of capture, 60 images 

#define NUM_THREADS (4)
#define NUM_CPU_CORES (1)
#define TEXT_SCALE 1
#define INDEX_MIN 0
#define SCALAR_DRAW_VALUE 143
#define SHIFT_DEFAULT_Y 50                  //Default 'y' position of text in image
#define TEXT_SHIFT 20 

	
void *Sequencer(void *threadp)

#endif