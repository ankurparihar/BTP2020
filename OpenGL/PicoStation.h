#pragma once
#include "Station.h"
#include "BaseStation.h"
#include "MobileStation.h"

class MobileStation;

class PicoStation : public Station {
public:
	unsigned int capacity = PICO_STATION_CAPACITY;
	unsigned int power = PICO_POWER;
	double bias = PICO_BIAS;
	std::vector <MobileStation*> mobileStations;

	PicoStation() {}
	PicoStation(const int& id) : Station(id) {}
	PicoStation(const int& id, std::string name) : Station(id, name) {}
	PicoStation(const int& id, const Point<int>& location) : Station(id, location) {}

	std::string getInfo();
	bool connect(MobileStation* mobile);
	void disconnect();
	void disconnect(MobileStation* mobile);
	double powerAt(const Point<int>& p);
	double powerAtUnbiased(const Point<int>& p);
	std::ostream& info(std::ostream& stream) const;
	friend std::ostream& operator<<(std::ostream& stream, const PicoStation& station);
};