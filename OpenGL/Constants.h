#pragma once

/*
 * Variables declared with extern keyword are global and accessible to all files
 * There must not be value assignment to such variables in header (.h) files
 * There must be one value assignment to such variables in cpp (.cpp) files
 *
 * We will use Constatnts.h file to declare and Constants.cpp file to define such variables

*/

const int WINDOW_WIDTH = 1366;	// Window width
const int WINDOW_HEIGHT = 768;	// Window height
const int BORDER = 25;			// Border of window where stations don't exists
const int SCALE = 4;

const float BS_R = 0.95f, BS_G = 0.77f, BS_B = 0.41f, BS_A = 1.0f;	// BaseStation color
const float PS_R = 0.15f, PS_G = 0.47f, PS_B = 0.44f, PS_A = 1.0f;	// MobileStation color
const float MS_R = 1.0f, MS_G = 1.0f, MS_B = 1.0f, MS_A = 1.0f;		// MobileStation color

const int NUM_BASE = 1;
const int NUM_PICO = 5;
const int NUM_MOBILE = 100;

const float BASE_STATION_RADIUS = 12.0;		// pixels
const float PICO_STATION_RADIUS = 8.0;		// pixels
const float MOBILE_STATION_RADIUS = 4.0;	// pixels
const float STATION_SEPARATION = 10.0;		// pixels

// No of maximum connected devices devices
const int BASE_STATION_CAPACITY = 25;
const int PICO_STATION_CAPACITY = 10;

// Experimental
extern int BASE_POWER;		// mW
extern int PICO_POWER;		// mW
const float BASE_BIAS = 0.0;// mW
extern float PICO_BIAS;		// mW
extern float MIN_POWER;

// different methods
const int METHOD1 = 0;
const int METHOD2 = 1;
const int STARTUP_METHOD = METHOD1;

// Global variables
extern int method;
extern bool refresh;
extern bool randomize;
extern bool printConnections;
extern float throughputArray[5];
const int sampleSize = 101;
extern float avgThr[sampleSize];
extern float biasEffect[sampleSize];
extern float biasEffectK[4][sampleSize];
extern float x_data[sampleSize];