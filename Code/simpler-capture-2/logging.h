#ifndef LOGGING_BG
#define LOGGING_BG

// Defines section
#define PROJECT_TAG "BGRTR_RT_Final"

// External variables section
bool USE_PRINTF;

// Functions for logging specific information
void log(char *thingToLog, int logLevel);
void log(char *thingToLog);
void setPrintf(bool usePrintf) {
	USE_PRINTF = usePrintf;
}
#endif // LOGGING_BG
