#include "Station.h"

std::ostream& Station::operator<<(std::ostream& stream) {
	stream << "Station: " << id << "( " << location.x << ", " << location.y << ")";
	return stream;
}