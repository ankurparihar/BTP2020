#pragma once

const int WINDOW_WIDTH = 1366;	// Window width
const int WINDOW_HEIGHT = 768;	// Window height
const int BORDER = 25;			// Border of window where stations don't exists

const float BS_R = 0.95f, BS_G = 0.77f, BS_B = 0.41f, BS_A = 1.0f;	// BaseStation color
const float PS_R = 0.15f, PS_G = 0.47f, PS_B = 0.44f, PS_A = 1.0f;	// MobileStation color
const float MS_R = 1.0f, MS_G = 1.0f, MS_B = 1.0f, MS_A = 1.0f;	// MobileStation color
//const float CN_R = 

const int NUM_BASE = 5;
const int NUM_PICO = 10;
const int NUM_MOBILE = 100;

const float BASE_STATION_RADIUS = 12.0;		// pixels
const float PICO_STATION_RADIUS = 8.0;		// pixels
const float MOBILE_STATION_RADIUS = 4.0;	// pixels
const double STATION_SEPARATION = 10.0;		// pixels

// No of maximum connected devices devices
const int PICO_STATION_CAPACITY = 10;
const int BASE_STATION_CAPACITY = 25;

// Experimental
const int BASE_POWER = 1000;	// mW
const int PICO_POWER = 200;		// mW
const double PICO_BIAS = 0.0;	// mW