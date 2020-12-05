#ifndef LOGGING_BG
#define LOGGING_BG
#include <string>
#define PROJECT_TAG "BGRTR_RT_Final"
extern bool USE_PRINTF;

using namespace std;

void log(string thingToLog, int logLevel);
void log(string thingToLog);
#endif
