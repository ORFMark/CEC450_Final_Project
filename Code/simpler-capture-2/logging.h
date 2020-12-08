#ifndef LOGGING_BG
#define LOGGING_BG

// Defines section
#define PROJECT_TAG "BGRTR_RT_Final"

// External variables section
<<<<<<< HEAD
//extern bool USE_PRINTF;


// Functions for logging specific information - inlined for performance
void log(const char * thingToLog, int logLevel);
void log(const char * thingToLog);

=======
// Functions for logging specific information
void log(char *thingToLog, int logLevel);
void log(char *thingToLog);
>>>>>>> 33a0cad75190ecd462740fece63a11cbae4682bf
#endif // LOGGING_BG
