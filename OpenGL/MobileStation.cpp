#include "MobileStation.h"

std::ostream& MobileStation::operator<<(std::ostream& stream) {
	stream << "MobileStation: " << id << " (" << location.x << ", " << location.y << ")";
	return stream;
}