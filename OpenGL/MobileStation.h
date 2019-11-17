#pragma once
#include "Station.h"

class MobileStation : public Station {
public:
	bool connected = false;
	unsigned int power = 0;
	Station* station;

	MobileStation() {}
	MobileStation(const int& id) : Station(id) {}
	MobileStation(const int& id, std::string name) : Station(id, name) {}
	MobileStation(const int& id, const Point<int>& location) : Station(id, location) {}

	bool connect(MobileStation* mobile);
	double powerAt(const Point<int>& p);
	std::ostream& info(std::ostream& stream) const;
	friend std::ostream& operator<<(std::ostream& stream, const MobileStation& station);
};