#include "PicoStation.h"

bool PicoStation::connect(MobileStation* mobile) {
	if (mobileStations.size() < capacity) {
		mobileStations.push_back(mobile);
		mobile->station = this;
		mobile->connected = true;
		return true;
	}
	return false;
}

double PicoStation::powerAt(const Point<int>& p) {
	double squareDistance = location.squraredDist(p);
	if (squareDistance <= 1.0) return power;
	return bias + (power / squareDistance);
}

std::ostream& PicoStation::info(std::ostream& stream) const {
	stream << "PicoStation: " << id << " (" << location.x << ", " << location.y << ")";
	return stream;
}

std::ostream& operator<<(std::ostream& stream, const PicoStation& station) {
	return station.info(stream);
}