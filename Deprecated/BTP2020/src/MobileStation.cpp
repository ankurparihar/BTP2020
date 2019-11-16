#include "MobileStation.h"

std::ostream& operator<<(std::ostream& stream, const MobileStation& station) {
	stream << "Mobile Station: " << station.id << " (" << station.location.x << ", " << station.location.y << ") -> BaseStation: " << station.baseStation->id;
	return stream;
}