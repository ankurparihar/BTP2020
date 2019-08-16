#pragma once
#include "math.h"
#include "MobileStation.h"

using namespace std;

class MobileStation;

class BaseStation {
public:
	int id;
	string name;
	Point<int> location;
	vector <MobileStation*> mobileStations;

	BaseStation() {}
	BaseStation(const int& id) : id(id) {}
	BaseStation(const Point<int>& location) : location(location) {}
	BaseStation(const int& locX, const int& locY) : location{ locX, locY } {}
	BaseStation(const vector<MobileStation*> mobileStations) : mobileStations(mobileStations) {}
	BaseStation(const int& id, const int& locX, const int& locY) : id(id), location{ locX, locY } {}

	friend std::ostream& operator<<(std::ostream& stream, const BaseStation& station);
};