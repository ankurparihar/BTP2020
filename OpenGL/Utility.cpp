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
		// v[id - 1].power = BASE_POWER;
		// v[id - 1].bias = 0.0;
		// v[id - 1] = BaseStation(id, Point<int>(random(border + (((id - 1) / 2) * x), x + border + (((id - 1) / 2) * x)/*length - 1 - border*/), random(border + (((id - 1) & 1) * y), y + border + (((id - 1) & 1) * y)/*width - 1 - border*/)));
		// v[id - 1] = BaseStation(id, Point<int>(500, 100));
	}
	return v;
}

std::vector<PicoStation> generatePicoStations(int n, int length, int width, int border) {
	std::vector<PicoStation> v(n);
	for (int id = 1; id <= n; ++id) {
		v[id - 1] = PicoStation(id, Point<int>(random(border, length - 1 - border), random(border, width - 1 - border)));
		// v[id - 1].power = PICO_POWER;
		// v[id - 1].bias = PICO_BIAS;
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

void connect(std::vector<MobileStation>& mobileStations, std::vector<BaseStation>& baseStations, std::vector<PicoStation>& picoStations, int method) {
	
	switch (method) {
	case METHOD1:
	{
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
	break;
	case METHOD2:
	{
		std::vector<Station*> stations;
		for (unsigned int i = 0; i < baseStations.size(); ++i) stations.push_back(&baseStations[i]);
		for (unsigned int i = 0; i < picoStations.size(); ++i) stations.push_back(&picoStations[i]);
		for (unsigned int i = 0; i < mobileStations.size(); ++i) {
			mobileStations[i].connected = false;
			mobileStations[i].station = NULL;
		}

		for (unsigned int i = 0; i < picoStations.size(); ++i) {
			picoStations[i].mobileStations.clear();
		}
		for (unsigned int i = 0; i < baseStations.size(); ++i) {
			baseStations[i].mobileStations.clear();
		}

		for (unsigned int i = 0; i < mobileStations.size(); ++i) {
			double totalPower = 0;
			for (unsigned int j = 0; j < stations.size(); j++) {
				totalPower += stations[j]->powerAt(mobileStations[i].location);
			}
			double mini = INT_MIN;
			Station* s = NULL;
			for (unsigned int j = 0; j < stations.size(); j++) {
				double presentPower = stations[j]->powerAt(mobileStations[i].location);
				double interference = totalPower - presentPower;
				double bitRate = expected_bitrate(stations[j], presentPower, interference, 3);
				if (mini < bitRate) {
					mini = bitRate;
					s = stations[j];
				}
			}
			if (s != nullptr) s->connect(&mobileStations[i]);
		}
	}
	break;
	}
}

void disconnect(std::vector<MobileStation>& mobileStations, std::vector<BaseStation>& baseStations, std::vector<PicoStation>& picoStations) {
	// disconnect all basestations and picostations. corresponding mobile stations will automatically disconnect
	for (BaseStation& base : baseStations) {
		base.disconnect();
	}
	for (PicoStation& pico : picoStations) {
		pico.disconnect();
	}
}

// This function will change assign modified variables like bias, capacity etc.
void reconfigure(std::vector<MobileStation>& mobileStations, std::vector<BaseStation>& baseStations, std::vector<PicoStation>& picoStations) {
	/*for (MobileStation& mobile : mobileStations) {
		
	}*/
	for (BaseStation& base : baseStations) {
		base.capacity = BASE_STATION_CAPACITY;
		base.power = BASE_POWER;
		base.bias = BASE_BIAS;
	}
	for (PicoStation& pico : picoStations) {
		pico.capacity = PICO_STATION_CAPACITY;
		pico.power = PICO_POWER;
		pico.bias = PICO_BIAS;
	}
}


float toGLX(int x) {
	return ((float)(2 * x) / WINDOW_WIDTH) - 1;
}

float toGLY(int y) {
	return ((float)(2 * y) / WINDOW_HEIGHT) - 1;
}

//ye wala function upar daal dena
double expected_bitrate(Station* s, double presentPower, double interference, int k) {
	if (s->getInfo() == "BaseStation") {
		BaseStation* bs = dynamic_cast<BaseStation*>(s);
		double nb = bs->mobileStations.size();
		double SINR = presentPower / interference;
		double Bandwidth = 10;
		double p = (Bandwidth / nb) * log2(1 + SINR);
		if (nb == bs->capacity) return -1;
		if (k == 0) return p;
		if (k == 1) {
			double q = (Bandwidth / bs->capacity) * log2(1 + SINR);
			return q;
		}
		if (k == 2) {
			return (p * ((nb + 1) / bs->capacity));
		}
		if (k == 3) {
			double temp = bs->capacity / (bs->capacity - nb - 1);
			return (p * temp);
		}
		return -2;
	}
	else if (s->getInfo() == "PicoStation") {
		PicoStation* ps = dynamic_cast<PicoStation*>(s);
		double nb = ps->mobileStations.size();
		double SINR = presentPower / interference;
		double Bandwidth = 10;
		double p = (Bandwidth / nb) * log2(1 + SINR);
		if (nb == ps->capacity) return -1;
		if (k == 0) return p;
		if (k == 1) {
			double q = (Bandwidth / ps->capacity) * log2(1 + SINR);
			return q;
		}
		if (k == 2) {
			return (p * ((nb + 1) / ps->capacity));
		}
		if (k == 3) {
			double temp = ps->capacity / (ps->capacity - nb - 1);
			return (p * temp);
		}
		return -2;
	}
	return -2;
}
