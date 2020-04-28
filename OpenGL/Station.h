#pragma once
#include "Math.h"

class MobileStation;

class Station {
public:
	int id;
	std::string name;
	Point<int> location;
	
	Station() {}
	Station(int id) : id(id) {}
	Station(int id, std::string name) : id(id), name(name) {}
	Station(int id, const Point<int>& location) : id(id), location(location) {}

	virtual std::string getInfo() = 0;
	virtual bool connect(MobileStation* mobile) = 0;
	virtual void disconnect() = 0;
	virtual void disconnect(MobileStation* mobile) = 0;
	virtual double powerAt(const Point<int>& p) = 0;
	virtual double powerAtUnbiased(const Point<int>& p) = 0;
	virtual std::ostream& info(std::ostream& stream) const;
	friend std::ostream& operator<<(std::ostream& stream, const Station& station);
};