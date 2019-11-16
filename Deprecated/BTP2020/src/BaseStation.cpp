#include "BaseStation.h"

std::ostream& operator<<(std::ostream& stream, const BaseStation& station) {
	stream << "Base Station: " << station.id << " (" << station.location.x << ", " << station.location.y << ")";
	return stream;
}