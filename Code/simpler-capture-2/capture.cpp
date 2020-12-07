// What is the point of this section?
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

// Global includes section
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <pthread.h>
#include <sched.h>
#include <time.h>
#include <semaphore.h>
#include <syslog.h>
#include <sys/time.h>
#include <sys/sysinfo.h>
#include <errno.h>

// OpenCV includes section
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// Local includes section
#include "util.h"
#include "frame_handling.h"

// Resolution defines section
#define HRES 640
#define VRES 480

// Time defines section
#define USEC_PER_MSEC (1000)
#define NANOSEC_PER_SEC (1000000000)

// Sequencer defines section
#define SEQUENCER_MAX_CYCLES 1500 // runtime = (0.02 * 1500); 30 seconds of capture, 60 images 
#define S0_CYCLES 20000000 //runs every 0.02 seconds
#define S1_CYCLES 5        //runs every (0.02 * 5) seconds (demo service, to be used for an image processing transform); for now just dishes out Fibonacci numbers
#define S2_CYCLES 10       //runs every (0.02 * 10) seconds (captures frames; passes frame with global pointer to be annotated by S3) 
#define S3_CYCLES 25       //runs every (0.02 * 25) seconds (annotates frame and saves the image with cv2::imwrite() )

// Hardware defines section
#define NUM_THREADS (3+1)
#define NUM_CPU_CORES (1)

// Other defines section
#define TEXT_SCALE 1
#define INDEX_MIN 0
#define SCALAR_DRAW_VALUE 143
#define SHIFT_DEFAULT_Y 50                  //Default 'y' position of text in image
#define TEXT_SHIFT 20
#define TRUE (1)
#define FALSE (0)

// Namespace includes section
using namespace std;
using namespace cv;

// Global variables section
int8_t abortTest = FALSE, abortS1 = FALSE, abortS2 = FALSE, abortS3 = FALSE;
sem_t semS1, semS2, semS3;
struct timeval start_time_val;
FrameQueue frameQueue;

// Forward function declarations section
void* Sequencer(void *threadp);
void* Service_1(void *threadp);

// Main function
int main(void) {
	struct timeval current_time_val;
	int i, rc, scope;
	cpu_set_t threadcpu;
	pthread_t threads[NUM_THREADS];
	threadParams_t threadParams[NUM_THREADS];
	pthread_attr_t rt_sched_attr[NUM_THREADS];
	int rt_max_prio, rt_min_prio;
	struct sched_param rt_param[NUM_THREADS];
	struct sched_param main_param;
	pthread_attr_t main_attr;
	pid_t mainpid;
	cpu_set_t allcpuset;

	printf("Starting Sequencer Demo\n");
	gettimeofday(&start_time_val, (struct timezone*) 0);
	gettimeofday(&current_time_val, (struct timezone*) 0);
	syslog(LOG_CRIT, "Sequencer @ sec=%d, msec=%d\n",
			(int) (current_time_val.tv_sec - start_time_val.tv_sec),
			(int) current_time_val.tv_usec / USEC_PER_MSEC);

	printf("System has %d processors configured and %d available.\n",
			get_nprocs_conf(), get_nprocs());

	CPU_ZERO(&allcpuset);

	for (i = 0; i < NUM_CPU_CORES; i++)
		CPU_SET(i, &allcpuset);

	printf("Using CPUS=%d from total available.\n", CPU_COUNT(&allcpuset));

	// initialize the sequencer semaphores
	//
	if (sem_init(&semS1, 0, 0)) {
		printf("Failed to initialize S1 semaphore\n");
		exit(-1);
	}
	if (sem_init(&semS2, 0, 0)) {
		printf("Failed to initialize S2 semaphore\n");
		exit(-1);
	}
	if (sem_init(&semS3, 0, 0)) {
		printf("Failed to initialize S3 semaphore\n");
		exit(-1);
	}

	mainpid = getpid();

	rt_max_prio = sched_get_priority_max(SCHED_FIFO);
	rt_min_prio = sched_get_priority_min(SCHED_FIFO);

	rc = sched_getparam(mainpid, &main_param);
	main_param.sched_priority = rt_max_prio;
	rc = sched_setscheduler(getpid(), SCHED_FIFO, &main_param);
	if (rc < 0)
		perror("main_param");
	print_scheduler();

	pthread_attr_getscope(&main_attr, &scope);

	if (scope == PTHREAD_SCOPE_SYSTEM)
		printf("PTHREAD SCOPE SYSTEM\n");
	else if (scope == PTHREAD_SCOPE_PROCESS)
		printf("PTHREAD SCOPE PROCESS\n");
	else
		printf("PTHREAD SCOPE UNKNOWN\n");

	printf("rt_max_prio=%d\n", rt_max_prio);
	printf("rt_min_prio=%d\n", rt_min_prio);

	// Initialize the global frame queue
	if (initQueue(&frameQueue, 256) == false) {
		destructQueue(&frameQueue);
		return 1;
	}
	CvCapture *camera = cvCreateCameraCapture(0);

	for (i = 0; i < NUM_THREADS; i++) {

		CPU_ZERO(&threadcpu);
		CPU_SET(3, &threadcpu);

		rc = pthread_attr_init(&rt_sched_attr[i]);
		rc = pthread_attr_setinheritsched(&rt_sched_attr[i],
				PTHREAD_EXPLICIT_SCHED);
		rc = pthread_attr_setschedpolicy(&rt_sched_attr[i], SCHED_FIFO);
		//rc=pthread_attr_setaffinity_np(&rt_sched_attr[i], sizeof(cpu_set_t), &threadcpu);

		rt_param[i].sched_priority = rt_max_prio - i;
		pthread_attr_setschedparam(&rt_sched_attr[i], &rt_param[i]);

		threadParams[i].threadIdx = i;
		threadParams[i].camera = camera;
		threadParams[i].frameQueue = &frameQueue;
	}

	printf("Service threads will run on %d CPU cores\n", CPU_COUNT(&threadcpu));

	// Create Service threads which will block awaiting release for:
	//
	// Servcie_1 = RT_MAX-1	@ 3 Hz
	//
	rt_param[1].sched_priority = rt_max_prio - 1;
	pthread_attr_setschedparam(&rt_sched_attr[1], &rt_param[1]);
	rc = pthread_create(&threads[1],             // pointer to thread descriptor
			&rt_sched_attr[1],         // use specific attributes
			//(void *)0,               // default attributes
			Service_1,                 // thread function entry point
			(void*) &(threadParams[1]) // parameters to pass in
			);
	if (rc < 0)
		perror("pthread_create for service 1");
	else
		printf("pthread_create successful for service 1\n");

	// Service_2 = RT_MAX-2	@ 1 Hz
	//
	rt_param[2].sched_priority = rt_max_prio - 2;
	pthread_attr_setschedparam(&rt_sched_attr[2], &rt_param[2]);
	rc = pthread_create(&threads[2], &rt_sched_attr[2], captureFrameService,
			(void*) &(threadParams[2]));
	if (rc < 0)
		perror("pthread_create for service 2");
	else
		printf("pthread_create successful for service 2\n");

	// Service_3 = RT_MAX-3	@ 0.5 Hz
	//
	rt_param[3].sched_priority = rt_max_prio - 3;
	pthread_attr_setschedparam(&rt_sched_attr[3], &rt_param[3]);
	rc = pthread_create(&threads[3], &rt_sched_attr[3], writeBackFrameService,
			(void*) &(threadParams[3]));
	if (rc < 0)
		perror("pthread_create for service 3");
	else
		printf("pthread_create successful for service 3\n");

	// Wait for service threads to initialize and await relese by sequencer.
	//
	// Note that the sleep is not necessary of RT service threads are created wtih
	// correct POSIX SCHED_FIFO priorities compared to non-RT priority of this main
	// program.
	//
	usleep(100000);

	// Create Sequencer thread, which like a cyclic executive, is highest prio
	printf("Start sequencer\n");
	//threadParams[0].sequencePeriods=900;
	threadParams[0].sequencePeriods = SEQUENCER_MAX_CYCLES; //30 seconds of time: 1500 iterations measured at 0.02 seconds per iteration. With image capturing running approximately 2 times per second, this captures 60 images.

	// Sequencer = RT_MAX	@ 30 Hz
	//
	rt_param[0].sched_priority = rt_max_prio;
	pthread_attr_setschedparam(&rt_sched_attr[0], &rt_param[0]);
	rc = pthread_create(&threads[0], &rt_sched_attr[0], Sequencer,
			(void*) &(threadParams[0]));
	if (rc < 0)
		perror("pthread_create for sequencer service 0");
	else
		printf("pthread_create successful for sequeencer service 0\n");

	for (i = 0; i < NUM_THREADS; i++)
		pthread_join(threads[i], NULL);

	printf("\nTEST COMPLETE\n");

	destructQueue(&frameQueue);
	return (EXIT_SUCCESS);
}

// 
void* Sequencer(void *threadp) {
	struct timeval current_time_val;
	//struct timespec delay_time = {0,33333333}; // delay for 33.33 msec, 30 Hz: original sequencer delay
	struct timespec delay_time = { 0, S0_CYCLES }; //delay for 20 msec, 50 Hz
	struct timespec remaining_time;
	double current_time;
	double residual;
	int rc, delay_cnt = 0;
	unsigned long long seqCnt = 0;
	threadParams_t *threadParams = (threadParams_t*) threadp;

	gettimeofday(&current_time_val, (struct timezone*) 0);
	syslog(LOG_CRIT, "Sequencer thread @ sec=%d, msec=%d\n",
			(int) (current_time_val.tv_sec - start_time_val.tv_sec),
			(int) current_time_val.tv_usec / USEC_PER_MSEC);
	printf("Sequencer thread @ sec=%d, msec=%d\n",
			(int) (current_time_val.tv_sec - start_time_val.tv_sec),
			(int) current_time_val.tv_usec / USEC_PER_MSEC);

	do {
		delay_cnt = 0;
		residual = 0.0;

		//gettimeofday(&current_time_val, (struct timezone *)0);
		//syslog(LOG_CRIT, "Sequencer thread prior to delay @ sec=%d, msec=%d\n", (int)(current_time_val.tv_sec-start_time_val.tv_sec), (int)current_time_val.tv_usec/USEC_PER_MSEC);
		do {
			rc = nanosleep(&delay_time, &remaining_time);

			if (rc == EINTR) {
				residual = remaining_time.tv_sec
						+ ((double) remaining_time.tv_nsec
								/ (double) NANOSEC_PER_SEC);

				if (residual > 0.0)
					printf("residual=%lf, sec=%d, nsec=%d\n", residual,
							(int) remaining_time.tv_sec,
							(int) remaining_time.tv_nsec);

				delay_cnt++;
			} else if (rc < 0) {
				perror("Sequencer nanosleep");
				exit(-1);
			}

		} while ((residual > 0.0) && (delay_cnt < 100));

		seqCnt++;
		gettimeofday(&current_time_val, (struct timezone*) 0);
		syslog(LOG_CRIT, "Sequencer cycle %llu @ sec=%d, msec=%d\n", seqCnt,
				(int) (current_time_val.tv_sec - start_time_val.tv_sec),
				(int) current_time_val.tv_usec / USEC_PER_MSEC);

		if (delay_cnt > 1)
			printf("Sequencer looping delay %d\n", delay_cnt);

		// Release each service at a sub-rate of the generic sequencer rate

		// Servcie_1 = RT_MAX-1	@ 3 Hz: original example sequencer runtime for S1
		if ((seqCnt % S1_CYCLES) == 0)
			sem_post(&semS1); //originally 10

		// Service_2 = RT_MAX-2	@ 1 Hz: original example sequencer runtime for S2
		if ((seqCnt % S2_CYCLES) == 0)
			sem_post(&semS2); //originally 25

		// Service_3 = RT_MAX-3	@ 0.5 Hz: original example sequencer runtime for S3
		if ((seqCnt % S3_CYCLES) == 0)
			sem_post(&semS3); //originally 50

		//gettimeofday(&current_time_val, (struct timezone *)0);
		//syslog(LOG_CRIT, "Sequencer release all sub-services @ sec=%d, msec=%d\n", (int)(current_time_val.tv_sec-start_time_val.tv_sec), (int)current_time_val.tv_usec/USEC_PER_MSEC);

	} while (!abortTest && (seqCnt < threadParams->sequencePeriods));

	sem_post(&semS1);
	sem_post(&semS2);
	sem_post(&semS3);

	abortS1 = TRUE;
	abortS2 = TRUE;
	abortS3 = TRUE;

	pthread_exit((void*) 0);
}

// 
void* Service_1(void *threadp) {
	return NULL;
}
