#pragma once
#include "math.h"
#include "BaseStation.h"
using namespace std;

class BaseStation;

class MobileStation {
public:
	int id;
	Point<int> location;
	BaseStation* baseStation;

	MobileStation() {}
	MobileStation(const int& id) : id(id) {}
	MobileStation(const Point<int>& location) : location(location) {}
	MobileStation(const int& locX, const int& locY) : location{ locX, locY } {}
	MobileStation(BaseStation* baseStation) : baseStation(baseStation) {}
	MobileStation(const int& id, const Point<int> location) : id(id), location(location) {}
	MobileStation(const int& id, const int& locX, const int& locY) : id(id), location{ locX, locY } {}
	MobileStation(const int& id, const Point<int> location, BaseStation* baseStation) : id(id), location(location), baseStation(baseStation) {}
	MobileStation(const int& id, const int& locX, const int& locY, BaseStation* baseStation) : id(id), location{ locX, locY }, baseStation(baseStation) {}

	friend std::ostream& operator<<(std::ostream& stream, const MobileStation& station);
};