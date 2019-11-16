#include "Graphics\Graphics.h"
#include "BaseStation.h"
#include "MobileStation.h"

#define NUM_BASE_STATIONS 10
#define NUM_MOBILE_STATIONS 100
#define USED_LEN_X 100
#define USED_LEN_Y 100
#define SCALESHIFT_X(i) SCREEN_OFFSET_X + i * (SCREEN_WIDTH - (SCREEN_OFFSET_X << 1)) / USED_LEN_X
#define SCALESHIFT_Y(i) SCREEN_OFFSET_Y + i * (SCREEN_HEIGHT - (SCREEN_OFFSET_Y << 1)) / USED_LEN_Y
#define SCALESHIFT_P(p) Point<int>(SCALESHIFT_X(p.x), SCALESHIFT_Y(p.y))

#define DeployPoint(type, i) graphics->drawFilledRect(SCALESHIFT_X(type[i].location.x) - (POINT_WIDTH >> 1), SCALESHIFT_Y(type[i].location.y) - (POINT_WIDTH >> 1), POINT_WIDTH, POINT_WIDTH);

using namespace std;

void randomizeStations(BaseStation tower[], MobileStation cellPhones[]);

int main(int argc, char** argv) {

	srand((unsigned int)time(NULL));
	
	BaseStation tower[NUM_BASE_STATIONS];
	MobileStation cellPhones[NUM_MOBILE_STATIONS];

	for (int i = 0; i < NUM_BASE_STATIONS; ++i) {
		tower[i] = BaseStation(i, random(100), random(100));
		// cout << tower[i] << endl;
	}

	for (int i = 0; i < NUM_MOBILE_STATIONS; ++i) {
		cellPhones[i] = MobileStation(i, random(100), random(100));
		double dist = 1000.0f;
		double tempDist;
		for (int j = 0; j < NUM_BASE_STATIONS; ++j) {
			tempDist = cellPhones[i].location.distance(tower[j].location);
			if (tempDist < dist) {
				cellPhones[i].baseStation = &tower[j];
				dist = tempDist;
			}
		}
		// cout << cellPhones[i] << endl;
	}

	// Graphics Components
	Graphics* graphics = new Graphics();
	graphics->init();

	// Main loop flag
	bool quit = false;

	// While application is running
	while (!quit)
	{
		// Clear screen
		graphics->setDrawColor(0x3A, 0x3C, 0x3F, 0xFF);
		graphics->clearRenderScreen();
		
		// Handle events on queue
		while (graphics->pollEvent() != 0)
		{
			if (graphics->handleEvent() == SCREEN_QUIT_EVENT) quit = true;
			else if (graphics->handleEvent() == NEW_SHOW) {
				randomizeStations(tower, cellPhones);
			}
		}
		
		// Draw connections
		graphics->setDrawColor(0xff, 0xff, 0xff, 0x55);
		for (int i = 0; i < NUM_MOBILE_STATIONS; ++i) {
			graphics->drawLine(SCALESHIFT_P(cellPhones[i].location), SCALESHIFT_P(cellPhones[i].baseStation->location));
		}

		// Draw MobileStations
		graphics->setDrawColor(0x00, 0xFF, 0xFF, 0x00);
		for (int i = 0; i < NUM_MOBILE_STATIONS; ++i) {
			DeployPoint(cellPhones, i);
		}
		// Draw BaseStations
		graphics->setDrawColor(0x00, 0xff, 0x00, 0x00);
		for (int i = 0; i < NUM_BASE_STATIONS; ++i) {
			DeployPoint(tower, i);
		}

		// Update the surface
		graphics->update();
	}

	graphics->exit();

	return 0;
}

void randomizeStations(BaseStation tower[], MobileStation cellPhones[]) {
	for (int i = 0; i < NUM_BASE_STATIONS; ++i) {
		tower[i] = BaseStation(i, random(100), random(100));
		// cout << tower[i] << endl;
	}

	for (int i = 0; i < NUM_MOBILE_STATIONS; ++i) {
		cellPhones[i] = MobileStation(i, random(100), random(100));
		double dist = 1000.0f;
		double tempDist;
		for (int j = 0; j < NUM_BASE_STATIONS; ++j) {
			tempDist = cellPhones[i].location.distance(tower[j].location);
			if (tempDist < dist) {
				cellPhones[i].baseStation = &tower[j];
				dist = tempDist;
			}
		}
		// cout << cellPhones[i] << endl;
	}
}