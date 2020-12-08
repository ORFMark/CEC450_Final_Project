
// Global include section
#include <syslog.h>
#include <sys/time.h>
#include <iostream>


// Local include section
#include "logging.h"


// Local global variables
struct timeval tv;


// Simple override function for logging at LOG_INFO level
void log(const char * thingToLog) {
      log(thingToLog, LOG_INFO);
}


// Function for logging the wanted text at the wanted logging level
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
}
