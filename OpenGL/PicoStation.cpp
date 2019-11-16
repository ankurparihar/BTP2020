#include "PicoStation.h"

std::ostream& PicoStation::operator<<(std::ostream& stream) {
	stream << "PicoStation: " << id << " (" << location.x << ", " << location.y << ")";
	return stream;
}