
// Global include section
#include <syslog.h>
#include <sys/time.h>
#include <iostream>


// Local include section
#include "logging.h"


// 
using namespace std;
struct timeval tv;


// Simple override function for logging at LOG_INFO level
void log(char * thingToLog) {
   log(thingToLog, LOG_INFO);
}


// Function for logging the wanted text at the wanted logging level
void log(char * thingToLog, int logLevel) {
   gettimeofday(&tv, (struct timezone *) 0);
   
   #ifdef USE_PRINTF
   //if(USE_PRINTF) {
     cout << PROJECT_TAG << ": SEC:USEC; " << tv.tv_sec << ":" << tv.tv_nsec 
        << "; " << thingToLog << endl;
   //}
   #else
   #warning "Should provide -DUSE_PRINTF while compiling this file to minimize potential errors."
   #endif
   
   syslog(logLevel, "%s; SEC:USEC; %lu:%lu; %s", PROJECT_TAG, tv.tv_sec, tv.tv_usec, thingToLog);
}
