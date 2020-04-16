#pragma once
#include "Station.h"
#include "PicoStation.h"
#include "MobileStation.h"

class MobileStation;

class BaseStation : public Station {
public:
	unsigned int capacity = BASE_STATION_CAPACITY;
	unsigned int power = 0;
	double bias = 0.0;
	std::vector <MobileStation*> mobileStations;

	BaseStation() {}
	BaseStation(const int& id) : Station(id) {}
	BaseStation(const int& id, std::string name) : Station(id, name) {}
	BaseStation(const int& id, const Point<int>& location) : Station(id, location) {}

	std::string getInfo();
	bool connect(MobileStation* mobile);
	void disconnect();
	void disconnect(MobileStation* mobile);
	double powerAt(const Point<int>& p);
	std::ostream& info(std::ostream& stream) const;
	friend std::ostream& operator<<(std::ostream& stream, const BaseStation& station);
};