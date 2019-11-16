#pragma once
#include "Station.h"
#include "PicoStation.h"
#include "MobileStation.h"

class MobileStation;

class BaseStation : public Station{
public:
	std::vector <MobileStation*> mobileStations;

	BaseStation() {}
	BaseStation(const int& id) : Station(id) {}
	BaseStation(const int& id, std::string name) : Station(id, name) {}
	BaseStation(const int& id, const Point<int>& location) : Station(id, location) {}

	std::ostream& operator<<(std::ostream& stream);
};