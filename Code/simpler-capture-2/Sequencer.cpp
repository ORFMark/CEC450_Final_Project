#define _GNU_SOURCE

#define HRES 640
#define VRES 480

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "util.hpp"
using namespace std;
using namespace cv;
//

#include <pthread.h>
#include <sched.h>
#include <time.h>
#include <semaphore.h>

#include <syslog.h>
#include <sys/time.h>
#include <sys/sysinfo.h>
#include <errno.h>

#define USEC_PER_MSEC (1000)
#define NANOSEC_PER_SEC (1000000000)
#define NUM_CPU_CORES (1)
#define TRUE (1)
#define FALSE (0)

#define S0_CYCLES 20000000 //runs every 0.02 seconds
#define S1_CYCLES 5        //runs every (0.02 * 5) seconds (demo service, to be used for an image processing transform); for now just dishes out Fibonacci numbers
#define S2_CYCLES 10       //runs every (0.02 * 10) seconds (captures frames; passes frame with global pointer to be annotated by S3) 
#define S3_CYCLES 25       //runs every (0.02 * 25) seconds (annotates frame and saves the image with cv2::imwrite() )

#define SEQUENCER_MAX_CYCLES 1500 // runtime = (0.02 * 1500); 30 seconds of capture, 60 images 

#define NUM_THREADS (3+1)

#define TEXT_SCALE 1
#define INDEX_MIN 0
#define SCALAR_DRAW_VALUE 143
#define SHIFT_DEFAULT_Y 50                  //Default 'y' position of text in image
#define TEXT_SHIFT 20 

int abortTest=FALSE;
int abortS1=FALSE, abortS2=FALSE, abortS3=FALSE;
sem_t semS1, semS2, semS3;
struct timeval start_time_val;

typedef struct
{
    int threadIdx;
    unsigned long long sequencePeriods;
} threadParams_t;


void *Sequencer(void *threadp);

void *Service_1(void *threadp);
void *Service_2(void *threadp);
void *Service_3(void *threadp);

double getTimeMsec(void);
void print_scheduler(void);

int fib_s1_a = 0;
int fib_s1_b = 1;

Mat *pointer_to_shared_image;


void *Sequencer(void *threadp)
{
    struct timeval current_time_val;
    //struct timespec delay_time = {0,33333333}; // delay for 33.33 msec, 30 Hz: original sequencer delay
    struct timespec delay_time = {0,S0_CYCLES}; //delay for 20 msec, 50 Hz
    struct timespec remaining_time;
    double current_time;
    double residual;
    int rc, delay_cnt=0;
    unsigned long long seqCnt=0;
    threadParams_t *threadParams = (threadParams_t *)threadp;

    gettimeofday(&current_time_val, (struct timezone *)0);
    syslog(LOG_CRIT, "Sequencer thread @ sec=%d, msec=%d\n", (int)(current_time_val.tv_sec-start_time_val.tv_sec), (int)current_time_val.tv_usec/USEC_PER_MSEC);
    printf("Sequencer thread @ sec=%d, msec=%d\n", (int)(current_time_val.tv_sec-start_time_val.tv_sec), (int)current_time_val.tv_usec/USEC_PER_MSEC);

    do
    {
        delay_cnt=0; residual=0.0;

        //gettimeofday(&current_time_val, (struct timezone *)0);
        //syslog(LOG_CRIT, "Sequencer thread prior to delay @ sec=%d, msec=%d\n", (int)(current_time_val.tv_sec-start_time_val.tv_sec), (int)current_time_val.tv_usec/USEC_PER_MSEC);
        do
        {
            rc=nanosleep(&delay_time, &remaining_time);

            if(rc == EINTR)
            { 
                residual = remaining_time.tv_sec + ((double)remaining_time.tv_nsec / (double)NANOSEC_PER_SEC);

                if(residual > 0.0) printf("residual=%lf, sec=%d, nsec=%d\n", residual, (int)remaining_time.tv_sec, (int)remaining_time.tv_nsec);
 
                delay_cnt++;
            }
            else if(rc < 0)
            {
                perror("Sequencer nanosleep");
                exit(-1);
            }
           
        } while((residual > 0.0) && (delay_cnt < 100));

        seqCnt++;
        gettimeofday(&current_time_val, (struct timezone *)0);
        syslog(LOG_CRIT, "Sequencer cycle %llu @ sec=%d, msec=%d\n", seqCnt, (int)(current_time_val.tv_sec-start_time_val.tv_sec), (int)current_time_val.tv_usec/USEC_PER_MSEC);


        if(delay_cnt > 1) printf("Sequencer looping delay %d\n", delay_cnt);


        // Release each service at a sub-rate of the generic sequencer rate

        // Servcie_1 = RT_MAX-1	@ 3 Hz: original example sequencer runtime for S1
        if((seqCnt % S1_CYCLES) == 0) sem_post(&semS1); //originally 10

        // Service_2 = RT_MAX-2	@ 1 Hz: original example sequencer runtime for S2
        if((seqCnt % S2_CYCLES) == 0) sem_post(&semS2); //originally 25

        // Service_3 = RT_MAX-3	@ 0.5 Hz: original example sequencer runtime for S3
        if((seqCnt % S3_CYCLES) == 0) sem_post(&semS3); //originally 50

        //gettimeofday(&current_time_val, (struct timezone *)0);
        //syslog(LOG_CRIT, "Sequencer release all sub-services @ sec=%d, msec=%d\n", (int)(current_time_val.tv_sec-start_time_val.tv_sec), (int)current_time_val.tv_usec/USEC_PER_MSEC);

    } while(!abortTest && (seqCnt < threadParams->sequencePeriods));

    sem_post(&semS1); sem_post(&semS2); sem_post(&semS3);

    abortS1=TRUE; abortS2=TRUE; abortS3=TRUE;

    pthread_exit((void *)0);
}

