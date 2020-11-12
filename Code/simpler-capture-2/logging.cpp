#include "logging.h"
#include <syslog.h>
#include <sys/time.h>
#include <iostream>
using namespace std;

void log(string thingToLog) {
   log(thingToLog, LOG_INFO);
}

void log(string thingToLog, int logLevel) {
    struct timeval tv;
   gettimeofday(&tv, (struct timezone *) 0);
   if(USE_PRINTF) {
     cout << PROJECT_TAG << ": SEC:USEC; " << tv.tv_sec << ":" << tv.tv_usec 
        << "; " << thingToLog << endl;
   }
     syslog(logLevel, "%s; SEC:USEC; %d:%d; %s", PROJECT_TAG
        , tv.tv_sec, tv.tv_usec, thingToLog);
   
}
