#include "Constants.h"

bool refresh = true;
int METHOD = STARTUP_METHOD;
float PICO_BIAS = 2.0;
float MIN_POWER = 2.0;
bool randomize = true;
bool printConnections = false;
int BASE_POWER = 1000000;
int PICO_POWER = 200000;
// float avgThr[sampleSize];
float throughputArray[5] = { 0.0, 0.0, 0.0, 0.0, 0.0 };
float biasEffect[sampleSize];
// float biasEffectK[4][sampleSize];
// float x_data[sampleSize];