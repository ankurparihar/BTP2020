#include "Constants.h"

bool refresh = true;
int METHOD = STARTUP_METHOD;
int K = 0;
float PICO_BIAS = 0.0;
float MIN_POWER = 2.0;
bool randomize = true;
bool printConnections = false;
bool PER_DEVICE_THROUGHPUT = false;
int BASE_POWER = 1000000;
int PICO_POWER = 200000;
// float avgThr[sampleSize];
float throughputArray[5] = { 0.0, 0.0, 0.0, 0.0, 0.0 };
float biasEffect[sampleSize];
// float biasEffectK[4][sampleSize];
// float x_data[sampleSize];
bool nbPlusOne = true;
float timeThroughput[TIME + 2] = {0.0};
float instantThroughput[TIME + 2] = {0.0};

std::stringstream txt;
std::stringstream csv;
int simulationCount = 0;

int PICO_BIAS_INT = 0;
int PICO_BIAS_MAX = 0;
double maxBitsTransferred = 0.0;