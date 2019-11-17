#include "MobileStation.h"

bool MobileStation::connect(MobileStation* mobile) {
	return false;
}
double MobileStation::powerAt(const Point<int>& p) {
	double squareDistance = location.distance(p);
	if (squareDistance <= 1.0) return power;
	return power / squareDistance;
}

std::ostream& MobileStation::info(std::ostream& stream) const {
	stream << "MobileStation: " << id << " (" << location.x << ", " << location.y << ")";
	return stream;
}

std::ostream& operator<<(std::ostream& stream, const MobileStation& station) {
	return station.info(stream);
}