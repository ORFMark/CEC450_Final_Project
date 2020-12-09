
// Global include section
#include <syslog.h>
#include <sys/time.h>
#include <iostream>


// Local include section
#include "logging.h"


// Local global variables section
struct timeval tv;


// Simple override function for logging at LOG_INFO level
void log(char * thingToLog) {
	log(thingToLog, LOG_INFO);
}


// Function for logging the wanted text at the wanted logging level
void log(char * thingToLog, int32 logLevel) {
	gettimeofday(&tv, (struct timezone*)NULL);

#ifdef WITH_PRINTF
   printf("%s; SEC:USEC; %lu:%lu; %s\n", PROJECT_TAG, tv.tv_sec,
			tv.tv_usec, thingToLog);
#endif

	syslog(logLevel, "%s; SEC:USEC; %lu:%lu; %s\n", PROJECT_TAG, tv.tv_sec,
			tv.tv_usec, thingToLog);
}
