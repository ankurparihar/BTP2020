#pragma once

#include "BaseStation.h"
#include "PicoStation.h"
#include "MobileStation.h"


int random(int x);
int random(int x, int y);

std::vector<BaseStation> generateBaseStations(int n, int length, int width, int border = 0);
std::vector<PicoStation> generatePicoStations(int n, int length, int width, int border = 0);
std::vector<MobileStation> generateMobileStations(int n, int length, int width, int border = 0);

std::vector<std::vector<int>> generateGrid(int n, int m);

float toGLX(int x);
float toGLY(int y);