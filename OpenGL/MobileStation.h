#pragma once
#include "Station.h"

class MobileStation : public Station {
public:
	bool connected = false;
	unsigned int power = 0;
	Station* station;
	int start_time = rand() % 3600;
	int end_time = start_time + 1 + (rand() % 3600);
	double bitrate = 0.0;

	MobileStation() {}
	MobileStation(const int& id) : Station(id) {}
	MobileStation(const int& id, std::string name) : Station(id, name) {}
	MobileStation(const int& id, const Point<int>& location) : Station(id, location) {}

	std::string getInfo();
	bool connect(MobileStation* mobile);
	void disconnect();
	void disconnect(MobileStation* mobile);
	double powerAt(const Point<int>& p);
	double powerAtUnbiased(const Point<int>& p);
	std::ostream& info(std::ostream& stream) const;
	friend std::ostream& operator<<(std::ostream& stream, const MobileStation& station);
};