#pragma once
#include "Math.h"

class Station {
public:
	int id;
	std::string name;
	Point<int> location;

	Station() {}
	Station(int id) : id(id) {}
	Station(int id, std::string name) : id(id), name(name) {}
	Station(int id, const Point<int>& location) : id(id), location(location) {}

	virtual std::ostream& operator<<(std::ostream& stream);
};