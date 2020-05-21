#include "BaseStation.h"

std::string BaseStation::getInfo() {
	return "BaseStation";
}

bool BaseStation::canConnect(MobileStation* mobile) {
	return mobileStations.size() < capacity;
}

bool BaseStation::connect(MobileStation* mobile) {
	if (mobileStations.size() < capacity) {
		mobileStations.push_back(mobile);
		mobile->station = this;
		mobile->connected = true;
		return true;
	}
	return false;
}

void BaseStation::disconnect() {
	for (MobileStation* mobile : mobileStations) {
		mobile->connected = false;
		mobile->station = NULL;
		mobile->bitrate = 0.0;
	}
	mobileStations.clear();
}

void BaseStation::disconnect(MobileStation* mobile) {
	if (mobile->connected && mobile->station == this) {
		mobile->connected = false;
		mobile->station = NULL;
		mobile->bitrate = 0.0;
	}
	std::vector<MobileStation*> ::iterator itr;
	for (itr = mobileStations.begin(); itr != mobileStations.end(); ++itr) {
		if (*itr == mobile) {
			mobileStations.erase(itr);
			break;
		}
	}
}

double BaseStation::powerAt(const Point<int>& p) {
	double squareDistance = location.squraredDist(p);
	if (squareDistance <= 1.0) return power;
	return bias + (power / squareDistance);
}

double BaseStation::powerAtUnbiased(const Point<int>& p) {
	double squareDistance = location.squraredDist(p);
	if (squareDistance <= 1.0) return power;
	return (power / squareDistance);
}

std::ostream& BaseStation::info(std::ostream& stream) const {
	stream << "BaseStation: " << id << " (" << location.x << ", " << location.y << ")";
	return stream;
}

std::ostream& operator<<(std::ostream& stream, const BaseStation& station) {
	return station.info(stream);
}