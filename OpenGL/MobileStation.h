#pragma once
#include "Station.h"

class MobileStation : public Station {
public:
	Station* station;

	MobileStation() {}
	MobileStation(const int& id) : Station(id) {}
	MobileStation(const int& id, std::string name) : Station(id, name) {}
	MobileStation(const int& id, const Point<int>& location) : Station(id, location) {}

	std::ostream& operator<<(std::ostream& stream);
};