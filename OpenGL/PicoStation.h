#pragma once
#include "Station.h"
#include "BaseStation.h"
#include "MobileStation.h"

class MobileStation;

class PicoStation : public Station {
public:
	std::vector<MobileStation> mobileStation;

	PicoStation() {}
	PicoStation(int id) : Station(id) {}
	PicoStation(int id, std::string name) : Station(id, name) {}
	PicoStation(int id, const Point<int>& location) : Station(id, location) {}

	std::ostream& operator<<(std::ostream& stream);
};