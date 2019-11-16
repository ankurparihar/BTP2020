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
		// v[id - 1] = BaseStation(id, Point<int>(random(border + (((id - 1) / 2) * x), x + border + (((id - 1) / 2) * x)/*length - 1 - border*/), random(border + (((id - 1) & 1) * y), y + border + (((id - 1) & 1) * y)/*width - 1 - border*/)));
		// v[id - 1] = BaseStation(id, Point<int>(500, 100));
	}
	return v;
}

std::vector<PicoStation> generatePicoStations(int n, int length, int width, int border) {
	std::vector<PicoStation> v(n);
	for (int id = 1; id <= n; ++id) {
		v[id - 1] = PicoStation(id, Point<int>(random(border, length - 1 - border), random(border, width - 1 - border)));
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

std::vector<std::vector<int>> generateGrid(int n, int m) {
	std::vector<std::vector<int>> v(n);
	int i, j;
	for (i = 0; i < n; ++i) {
		std::vector<int> k(m);
		for (j = 0; j < m; ++j) k[j] = 0;
		v[i] = k;
	}
	return v;
}

float toGLX(int x) {
	return ((float)(2 * x) / WINDOW_WIDTH) - 1;
}

float toGLY(int y) {
	return ((float)(2 * y) / WINDOW_HEIGHT) - 1;
}