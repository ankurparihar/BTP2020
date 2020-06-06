#include "PicoStation.h"

std::string PicoStation::getInfo() {
	return "PicoStation";
}

bool PicoStation::canConnect(MobileStation* mobile) {
	return mobileStations.size() < capacity;
}

bool PicoStation::connect(MobileStation* mobile) {
	if (mobileStations.size() < capacity) {
		mobileStations.push_back(mobile);
		mobile->station = this;
		mobile->connected = true;
		return true;
	}
	return false;
}

void PicoStation::disconnect() {
	for (MobileStation* mobile : mobileStations) {
		mobile->connected = false;
		mobile->station = NULL;
		mobile->bitrate = 0.0;
	}
	mobileStations.clear();
}

void PicoStation::disconnect(MobileStation* mobile) {
	if (mobile->connected && mobile->station == this) {
		mobile->connected = false;
		mobile->station = NULL;
		mobile->bitrate = 0.0;
		mobile->interference = INT_MAX;
	}
	std::vector<MobileStation*> ::iterator itr;
	for (itr = mobileStations.begin(); itr != mobileStations.end(); ++itr) {
		if (*itr == mobile) {
			mobileStations.erase(itr);
			break;
		}
	}
}

double PicoStation::powerAt(const Point<int>& p) {
	double squareDistance = location.squraredDist(p);
	if (squareDistance <= 1.0) return power;
	return bias + (power / squareDistance);
}

double PicoStation::powerAtUnbiased(const Point<int>& p) {
	double squareDistance = location.squraredDist(p);
	if (squareDistance <= 1.0) return power;
	return (power / squareDistance);
}

std::ostream& PicoStation::info(std::ostream& stream) const {
	stream << "PicoStation: " << id << " (" << location.x << ", " << location.y << ")";
	return stream;
}

std::ostream& operator<<(std::ostream& stream, const PicoStation& station) {
	return station.info(stream);
}