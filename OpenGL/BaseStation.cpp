#include "BaseStation.h"

std::ostream& BaseStation::operator<<(std::ostream& stream) {
	stream << "BaseStation: " << id << " (" << location.x << ", " << location.y << ")";
	return stream;
}