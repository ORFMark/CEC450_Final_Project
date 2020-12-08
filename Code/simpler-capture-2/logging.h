
#ifndef LOGGING_BG
#define LOGGING_BG


// Defines section
#define PROJECT_TAG "BGRTR_RT_Final"


// External variables section
//extern bool USE_PRINTF;


// Functions for logging specific information - inlined for performance
void log(const char * thingToLog, int logLevel);
void log(const char * thingToLog);

#endif // LOGGING_BG
