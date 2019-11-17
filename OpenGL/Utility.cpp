#include "Utility.h"

int random(int x) {
	/* return random number in range [0-x] */
	return rand() % (x + 1);
}
int random(int x, int y) {
	/* return random number in range [x-y] */
	return random(y - x) + x;
}

std::vector<BaseStation> generateBaseStations(int n, int length, int width, int border) {
	std::vector<BaseStation> v(n);
	for (int id = 1; id <= n; ++id) {
		v[id - 1] = BaseStation(id, Point<int>(random(border, length - 1 - border), random(border, width - 1 - border)));
		v[id - 1].power = BASE_POWER;
		// v[id - 1] = BaseStation(id, Point<int>(random(border + (((id - 1) / 2) * x), x + border + (((id - 1) / 2) * x)/*length - 1 - border*/), random(border + (((id - 1) & 1) * y), y + border + (((id - 1) & 1) * y)/*width - 1 - border*/)));
		// v[id - 1] = BaseStation(id, Point<int>(500, 100));
	}
	return v;
}

std::vector<PicoStation> generatePicoStations(int n, int length, int width, int border) {
	std::vector<PicoStation> v(n);
	for (int id = 1; id <= n; ++id) {
		v[id - 1] = PicoStation(id, Point<int>(random(border, length - 1 - border), random(border, width - 1 - border)));
		v[id - 1].power = PICO_POWER;
		// v[id - 1] = PicoStation(id, Point<int>(500, 100));
	}
	return v;
}

std::vector<MobileStation> generateMobileStations(int n, int lenght, int width, int border) {
	std::vector<MobileStation> v(n);
	for (int id = 1; id <= n; ++id) {
		v[id-1] = MobileStation(id, Point<int>(random(border, lenght - 1 - border), random(border, width - 1 - border)));
		// v[id - 1] = MobileStation(id, Point<int>(100, 100));
	}
	return v;
}

std::vector<std::vector<Station*>> generateGrid(int base, int pico, int mobile) {
	std::vector<std::vector<Station*>> v(mobile);
	int i, j;
	for (i = 0; i < mobile; ++i) {
		std::vector<Station*> k(base+pico);
		for (j = 0; j < base + pico; ++j) k[j] = NULL;
		v[i] = k;
	}
	return v;
}

void connect(std::vector<MobileStation>& mobileStations, std::vector<BaseStation>& baseStations, std::vector<PicoStation>& picoStations) {
	std::vector<Station*> stations;
	for (unsigned int i = 0; i < baseStations.size(); ++i) stations.push_back(&baseStations[i]);
	for (unsigned int i = 0; i < picoStations.size(); ++i) stations.push_back(&picoStations[i]);
	for (unsigned int i = 0; i < mobileStations.size(); ++i) {
		sort(stations.begin(), stations.end(), [&](Station* s1, Station* s2) {
			return s1->powerAt(mobileStations[i].location) > s2->powerAt(mobileStations[i].location);
		});
		unsigned int j = 0;
		while (j < stations.size() && !stations[j++]->connect(&mobileStations[i]));
	}
}

float toGLX(int x) {
	return ((float)(2 * x) / WINDOW_WIDTH) - 1;
}

float toGLY(int y) {
	return ((float)(2 * y) / WINDOW_HEIGHT) - 1;
}