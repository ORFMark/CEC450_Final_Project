// Global include section
#include <syslog.h>
#include <sys/time.h>
#include <iostream>
// Local include section
#include "logging.h"

<<<<<<< HEAD

// Local global variables
=======
// 
using namespace std;
>>>>>>> 33a0cad75190ecd462740fece63a11cbae4682bf
struct timeval tv;

// Simple override function for logging at LOG_INFO level
<<<<<<< HEAD
void log(const char * thingToLog) {
      log(thingToLog, LOG_INFO);
=======
void log(char *thingToLog) {
	log(thingToLog, LOG_INFO);
>>>>>>> 33a0cad75190ecd462740fece63a11cbae4682bf
}

// Function for logging the wanted text at the wanted logging level
<<<<<<< HEAD
void log(const char * thingToLog, int logLevel) {
      gettimeofday(&tv, (struct timezone *)NULL);
         
#ifdef USE_PRINTF
      std::cout << PROJECT_TAG << ": SEC:USEC; " << tv.tv_sec << ":" <<
         tv.tv_usec << "; " << thingToLog << std::endl;
#else
      #warning "Should provide -DUSE_PRINTF while compiling this file to print logged information."
#endif

      syslog(logLevel, "%s; SEC:USEC; %lu:%lu; %s", PROJECT_TAG,
         tv.tv_sec, tv.tv_usec, thingToLog);
=======
void log(char *thingToLog, int logLevel) {
	gettimeofday(&tv, (struct timezone*) 0);
/*
	if (RT_USE_PRINTF) {
		cout << PROJECT_TAG << ": SEC:USEC; " << tv.tv_sec << ":" << tv.tv_usec
				<< "; " << thingToLog << endl;
	}
*/
	syslog(logLevel, "%s; SEC:USEC; %lu:%lu; %s", PROJECT_TAG, tv.tv_sec,
			tv.tv_usec, thingToLog);
>>>>>>> 33a0cad75190ecd462740fece63a11cbae4682bf
}
