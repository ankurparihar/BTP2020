#include "MobileStation.h"

std::string MobileStation::getInfo() {
	return "MobileStation";
}

bool MobileStation::connect(MobileStation* mobile) {
	return false;
}

void MobileStation::disconnect() {
	if (connected) {
		station->disconnect(this);
	}
}

void MobileStation::disconnect(MobileStation* mobile) {
}

double MobileStation::powerAt(const Point<int>& p) {
	double squareDistance = location.squraredDist(p);
	if (squareDistance <= 1.0) return power;
	return power / squareDistance;
}

double MobileStation::powerAtUnbiased(const Point<int>& p) {
	double squareDistance = location.squraredDist(p);
	if (squareDistance <= 1.0) return power;
	return (power / squareDistance);
}

std::ostream& MobileStation::info(std::ostream& stream) const {
	stream << "MobileStation: " << id << " (" << location.x << ", " << location.y << ")";
	return stream;
}

std::ostream& operator<<(std::ostream& stream, const MobileStation& station) {
	return station.info(stream);
}