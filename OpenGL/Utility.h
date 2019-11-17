#pragma once

#include "BaseStation.h"
#include "PicoStation.h"
#include "MobileStation.h"


int random(int x);
int random(int x, int y);

std::vector<BaseStation> generateBaseStations(int n, int length, int width, int border = 0);
std::vector<PicoStation> generatePicoStations(int n, int length, int width, int border = 0);
std::vector<MobileStation> generateMobileStations(int n, int length, int width, int border = 0);

std::vector<std::vector<Station*>> generateGrid(int numBase, int numPico, int numMobile);
void connect(std::vector<MobileStation>& mobileStations, std::vector<BaseStation>& baseStations, std::vector<PicoStation>& picoStations);

float toGLX(int x);
float toGLY(int y);