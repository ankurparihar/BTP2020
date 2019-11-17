#include "BaseStation.h"

bool BaseStation::connect(MobileStation* mobile) {
	if (mobileStations.size() < capacity) {
		mobileStations.push_back(mobile);
		mobile->station = this;
		mobile->connected = true;
		return true;
	}
	return false;
}

double BaseStation::powerAt(const Point<int>& p) {
	double squareDistance = location.distance(p);
	if (squareDistance <= 1.0) return power;
	return bias + (power / squareDistance);
}

std::ostream& BaseStation::info(std::ostream& stream) const {
	stream << "BaseStation: " << id << " (" << location.x << ", " << location.y << ")";
	return stream;
}

std::ostream& operator<<(std::ostream& stream, const BaseStation& station) {
	return station.info(stream);
}