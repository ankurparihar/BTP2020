#include "BaseStation.h"
#include "MobileStation.h"

#define NUM_BASE_STATIONS 10
#define NUM_MOBILE_STATIONS 100

using namespace std;

int main() {
	srand((unsigned int)time(NULL));
	
	BaseStation tower[NUM_BASE_STATIONS];
	MobileStation cellPhones[NUM_MOBILE_STATIONS];

	for (int i = 0; i < NUM_BASE_STATIONS; ++i) {
		tower[i] = BaseStation(i, random(100), random(100));
		cout << tower[i] << endl;
	}
	cout << endl;
	for (int i = 0; i < NUM_MOBILE_STATIONS; ++i) {
		cellPhones[i] = MobileStation(i, random(100), random(100), &tower[i%10]);
		cout << cellPhones[i] << endl;
	}

	return 0;
}