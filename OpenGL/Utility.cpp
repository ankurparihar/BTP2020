#include "Utility.h"

int random(int x) {
	/* return random number in range [0-x] */
	return rand() % (x + 1);
}
int random(int x, int y) {
	/* return random number in range [x-y] */
	return random(y - x) + x;
}

std::vector<BaseStation> generateBaseStations(int n, int length, int width, int border) {
	std::vector<BaseStation> v(n);
	for (int id = 1; id <= n; ++id) {
		v[id - 1] = BaseStation(id, Point<int>(random(border, length - 1 - border), random(border, width - 1 - border)) * SCALE);
	}
	return v;
}

std::vector<PicoStation> generatePicoStations(int n, int length, int width, int border) {
	std::vector<PicoStation> v(n);
	for (int id = 1; id <= n; ++id) {
		v[id - 1] = PicoStation(id, Point<int>(random(border, length - 1 - border), random(border, width - 1 - border)) * SCALE);
	}
	return v;
}

std::vector<MobileStation> generateMobileStations(int n, int lenght, int width, int border) {
	std::vector<MobileStation> v(n);
	for (int id = 1; id <= n; ++id) {
		v[id - 1] = MobileStation(id, Point<int>(random(border, lenght - 1 - border), random(border, width - 1 - border)) * SCALE);
		int s = rand() % (TIME-1) + 1;			// s = [1, TIME-1]
		int e = s + 1 + rand() % (TIME - s);	// e = s + 1 + [0, TIME-1-s] => [s+1, TIME]
		v[id - 1].initial_start_time = s;
		v[id - 1].start_time = s;
		v[id - 1].end_time = e;
		v[id - 1].bitrate = 0.0;
		v[id - 1].interference = INT_MAX;
	}
	return v;
}

std::vector<std::vector<Station*>> generateGrid(int base, int pico, int mobile) {
	std::vector<std::vector<Station*>> v(mobile);
	int i, j;
	for (i = 0; i < mobile; ++i) {
		std::vector<Station*> k(base+pico);
		for (j = 0; j < base + pico; ++j) k[j] = NULL;
		v[i] = k;
	}
	return v;
}

void connect(std::vector<MobileStation>& mobileStations, std::vector<BaseStation>& baseStations, std::vector<PicoStation>& picoStations, int method) {

	switch (method) {
	case METHOD_BIAS:
	{
		std::vector<Station*> stations;
		double totalBitsTransferred = 0.0;
		double instantBits = 0.0;
		int connected = 0;
		double throughput = 0.0;
		double averageBitsTransferred;
		//double totalPower = 0.0;
		for (unsigned int i = 0; i < baseStations.size(); ++i) {
			baseStations[i].mobileStations.clear();
			stations.push_back(&baseStations[i]);
		}
		for (unsigned int i = 0; i < picoStations.size(); ++i) {
			picoStations[i].mobileStations.clear();
			stations.push_back(&picoStations[i]);
		}
		for (unsigned int i = 0; i < mobileStations.size(); ++i) {
			mobileStations[i].start_time = mobileStations[i].initial_start_time;
			mobileStations[i].connected = false;
			mobileStations[i].bitrate = 0.0;
			mobileStations[i].station = NULL;
			mobileStations[i].interference = INT_MAX;
		}

		for (int time = 1; time <= TIME; ++time) {
			for (unsigned int i = 0; i < mobileStations.size(); ++i) {
				if (mobileStations[i].start_time == time) {
					// Connect the mobile
					sort(stations.begin(), stations.end(), [&](Station* s1, Station* s2) {
						return s1->powerAt(mobileStations[i].location) > s2->powerAt(mobileStations[i].location);
					});
					unsigned int j = 0;
					double totalPower = 0.0;
					for (j = 0; j < stations.size(); ++j) {
						totalPower += stations[j]->powerAtUnbiased(mobileStations[i].location);
					}
					j = 0;
					while (j < stations.size() && !stations[j]->connect(&mobileStations[i])) j++;
					if (mobileStations[i].connected) {
						double presentPower = stations[j]->powerAtUnbiased(mobileStations[i].location);
						double interference = totalPower - presentPower;
						double SINR = presentPower / interference;
						double tempbitrate;	// Mbps (small b in Mb)
						mobileStations[i].interference = interference;
						connected++;
						// Correcting the bitrate of connected mobile of current station
						int sizeOfStation = stations[j]->mobileStations.size();
						for (int itr = 0; itr < sizeOfStation; ++itr) {
							presentPower = stations[j]->powerAtUnbiased(stations[j]->mobileStations[itr]->location);
							interference = stations[j]->mobileStations[itr]->interference;
							SINR = presentPower / interference;
							tempbitrate = (BANDWIDTH / sizeOfStation)*log2(1 + SINR);
							stations[j]->mobileStations[itr]->bitrate = tempbitrate;
						}
					}
					else {
						// if (mobileStations[i].end_time - mobileStations[i].start_time > 1) mobileStations[i].start_time++;
					}
				}
				if (mobileStations[i].end_time == time && mobileStations[i].connected) {
					// Disconnecting the mobile
					Station* tempStation = mobileStations[i].station;
					tempStation->disconnect(&mobileStations[i]);
					connected--;
					// Correcting the bitrate of connected mobile of current station
					int sizeOfStation = tempStation->mobileStations.size();
					for (int itr = 0; itr < sizeOfStation; ++itr) {
						double presentPower = tempStation->powerAtUnbiased(tempStation->mobileStations[itr]->location);
						double interference = tempStation->mobileStations[itr]->interference;
						double SINR = presentPower / interference;
						double tempbitrate = (BANDWIDTH / sizeOfStation)*log2(1 + SINR);
						tempStation->mobileStations[itr]->bitrate = tempbitrate;
					}
				}
			}
			instantBits = 0.0;
			for (unsigned int i = 0; i < mobileStations.size(); ++i) {
				instantBits += mobileStations[i].bitrate;
			}
			totalBitsTransferred += instantBits;

			averageBitsTransferred = totalBitsTransferred / time;
			// ss << "average Bits Transferred\tBiasing - (" << PICO_BIAS << ") " << averageBitsTransferred << std::endl;
			// timeThroughput[time] = (float)averageBitsTransferred;
			// instantThroughput[time] = (float)instantBits;
		}

		if (averageBitsTransferred > maxBitsTransferred) {
			maxBitsTransferred = averageBitsTransferred;
			PICO_BIAS_MAX = PICO_BIAS_INT;
		}

		// timeThroughput[TIME + 1] = 0.0;
		// instantThroughput[TIME + 1] = 0.0;
		// for (int i = 1; i <= TIME; ++i) {
		// 	if (timeThroughput[i] > timeThroughput[TIME + 1]) timeThroughput[TIME + 1] = timeThroughput[i];
		// 	if (instantThroughput[i] > instantThroughput[TIME + 1]) instantThroughput[TIME + 1] = instantThroughput[i];
		// }
		// timeThroughput[TIME + 1] *= 1.75;
		// instantThroughput[TIME + 1] *= 1.75;
		// 
		// biasEffect[(int)(2*PICO_BIAS * 10)] = (float)averageBitsTransferred;
		// ss << "Throughput\tBiasing - " << "(" << PICO_BIAS << ") " << throughput << std::endl;
		if (PICO_BIAS_INT == 10) {
			txt << "average Bits Transferred\tBiasing - (" << (double)PICO_BIAS_MAX/10 << ") " << maxBitsTransferred << std::endl;
			csv << maxBitsTransferred << ',';
			METHOD = METHOD_K;
			PICO_BIAS = 0;
			PICO_BIAS_INT = 0;
			maxBitsTransferred = 0.0;
		}
		else {
			PICO_BIAS_INT++;
			PICO_BIAS = (double)PICO_BIAS_INT / 10;
		}
		refresh = true;
	}
	break;
	case METHOD_K:
	{
		std::vector<Station*> stations;
		double throughput = 0.0;
		int connected;
		for (unsigned int i = 0; i < baseStations.size(); ++i) stations.push_back(&baseStations[i]);
		for (unsigned int i = 0; i < picoStations.size(); ++i) stations.push_back(&picoStations[i]);
		for (int loop = 0; loop < 4; ++loop) {
			connected = 0;
			double totalBitsTransferred = 0.0;
			double averageBitsTransferred = 0.0;
			double instantBits = 0.0;
			
			for (unsigned int i = 0; i < baseStations.size(); ++i) {
				baseStations[i].mobileStations.clear();
			}
			for (unsigned int i = 0; i < picoStations.size(); ++i) {
				picoStations[i].mobileStations.clear();
			}
			for (unsigned int i = 0; i < mobileStations.size(); ++i) {
				mobileStations[i].start_time = mobileStations[i].initial_start_time;
				mobileStations[i].connected = false;
				mobileStations[i].bitrate = 0.0;
				mobileStations[i].station = NULL;
				mobileStations[i].interference = INT_MAX;
			}

			for (int time = 1; time <= TIME; ++time) {
				for (unsigned int i = 0; i < mobileStations.size(); ++i) {
					if (mobileStations[i].start_time == time) {
						// Connecting the mobile
						double totalPower = 0;
						for (unsigned int j = 0; j < stations.size(); ++j) {
							totalPower += stations[j]->powerAtUnbiased(mobileStations[i].location);
						}
						double mini = INT_MIN;
						Station *s = NULL;
						for (unsigned int j = 0; j < stations.size(); ++j) {
							if (stations[j]->canConnect(&mobileStations[i])) {
								double presentPower = stations[j]->powerAtUnbiased(mobileStations[i].location);
								double interference = totalPower - presentPower;
								double bitRate = expected_bitrate(stations[j], presentPower, interference, loop);
								if (mini < bitRate && bitRate>0.0) {
									mini = bitRate;
									s = stations[j];
								}
							}
						}
						if (s != nullptr) {
							s->connect(&mobileStations[i]);
							double presentPower = s->powerAtUnbiased(mobileStations[i].location);
							double interference = totalPower - presentPower;
							double SINR = (s->powerAtUnbiased(mobileStations[i].location)) / interference;
							double tempbitrate;
							mobileStations[i].interference = interference;
							connected++;
							// Correcting the bitrate of connected mobile of current station
							int sizeOfStation = s->mobileStations.size();
							for (int itr = 0; itr < sizeOfStation; ++itr) {
								presentPower = s->powerAtUnbiased(s->mobileStations[itr]->location);
								interference = s->mobileStations[itr]->interference;
								SINR = presentPower / interference;
								tempbitrate = (BANDWIDTH / sizeOfStation)*log2(1 + SINR);
								s->mobileStations[itr]->bitrate = tempbitrate;
							}
						}
						else {
							// if (mobileStations[i].end_time - mobileStations[i].start_time > 1) mobileStations[i].start_time++;
						}
					}
					if (mobileStations[i].end_time == time && mobileStations[i].connected) {
						// Disconnecting the mobile
						connected--;
						Station* tempStation = mobileStations[i].station;
						tempStation->disconnect(&mobileStations[i]);
						int sizeOfStation = tempStation->mobileStations.size();
						for (int itr = 0; itr < sizeOfStation; ++itr) {
							double presentPower = tempStation->powerAtUnbiased(tempStation->mobileStations[itr]->location);
							double interference = tempStation->mobileStations[itr]->interference;
							double SINR = presentPower / interference;
							double tempbitrate = (BANDWIDTH / sizeOfStation)*log2(1 + SINR);
							tempStation->mobileStations[itr]->bitrate = tempbitrate;
						}
					}
				}

				instantBits = 0.0;
				for (unsigned int i = 0; i < mobileStations.size(); ++i) {
					instantBits += mobileStations[i].bitrate;
				}
				totalBitsTransferred += instantBits;

				averageBitsTransferred = totalBitsTransferred / time;
				// ss << "average Bits Transferred\tk = " << loop << " - " << averageBitsTransferred << std::endl;
				// timeThroughput[time] = (float)averageBitsTransferred;
				// instantThroughput[time] = (float)instantBits;
			}

			txt << "average Bits Transferred\tk = " << loop << " - " << averageBitsTransferred << std::endl;
			csv << averageBitsTransferred << ((loop == 3) ? '\n' : ',');
		}
		// will show for only last loop
		// timeThroughput[TIME + 1] = 0.0;
		// instantThroughput[TIME + 1] = 0.0;
		// for (int i = 1; i <= TIME; ++i) {
		// 	if (timeThroughput[i] > timeThroughput[TIME + 1]) timeThroughput[TIME + 1] = timeThroughput[i];
		// 	if (instantThroughput[i] > instantThroughput[TIME + 1]) instantThroughput[TIME + 1] = instantThroughput[i];
		// }
		// timeThroughput[TIME + 1] *= 1.75;
		// instantThroughput[TIME + 1] *= 1.75;
		METHOD = METHOD_BIAS;
		refresh = true;
		randomize = true;
	}
	break;
	}

	// std::ofstream file;
	// file.open("result.txt");
	// file << ss.rdbuf();
	// ss.str("");
}

// Have freedom to analyze result for every K, K will be picked from globals
void connectWithK(std::vector<MobileStation>& mobileStations, std::vector<BaseStation>& baseStations, std::vector<PicoStation>& picoStations, int method) {

	switch (method) {
	case METHOD_BIAS:
	{
		std::vector<Station*> stations;
		double totalBitsTransferred = 0.0;
		double instantBits = 0.0;
		int connected = 0;
		double throughput = 0.0;
		double averageBitsTransferred;
		//double totalPower = 0.0;
		for (unsigned int i = 0; i < baseStations.size(); ++i) {
			baseStations[i].mobileStations.clear();
			stations.push_back(&baseStations[i]);
		}
		for (unsigned int i = 0; i < picoStations.size(); ++i) {
			picoStations[i].mobileStations.clear();
			stations.push_back(&picoStations[i]);
		}
		for (unsigned int i = 0; i < mobileStations.size(); ++i) {
			mobileStations[i].start_time = mobileStations[i].initial_start_time;
			mobileStations[i].connected = false;
			mobileStations[i].bitrate = 0.0;
			mobileStations[i].station = NULL;
		}

		for (int time = 1; time <= TIME; ++time) {
			for (unsigned int i = 0; i < mobileStations.size(); ++i) {
				if (mobileStations[i].start_time == time) {
					// Connect the mobile
					sort(stations.begin(), stations.end(), [&](Station* s1, Station* s2) {
						return s1->powerAt(mobileStations[i].location) > s2->powerAt(mobileStations[i].location);
					});
					unsigned int j = 0;
					double totalPower = 0.0;
					for (j = 0; j < stations.size(); ++j) {
						totalPower += stations[j]->powerAtUnbiased(mobileStations[i].location);
					}
					j = 0;
					while (j < stations.size() && stations[j]->powerAt(mobileStations[i].location) >= MIN_POWER && !stations[j]->connect(&mobileStations[i])) j++;
					if (mobileStations[i].connected) {
						double presentPower = stations[j]->powerAtUnbiased(mobileStations[i].location);
						double interference = totalPower - presentPower;
						double SINR = presentPower / interference;
						double tempbitrate = (BANDWIDTH)*log2(1 + SINR);	// Mbps (small b in Mb)
						connected++;
						// Correcting the bitrate of connected mobile of current station
						int sizeOfStation = stations[j]->mobileStations.size();
						for (int itr = 0; itr < sizeOfStation; ++itr) {
							double temp = stations[j]->mobileStations[itr]->bitrate;
							temp = (temp*(sizeOfStation - 1)) / sizeOfStation;
							stations[j]->mobileStations[itr]->bitrate = temp;
						}
						mobileStations[i].bitrate = tempbitrate / sizeOfStation;
					}
					else {
						if (mobileStations[i].end_time - mobileStations[i].start_time > 1) mobileStations[i].start_time++;
					}
				}
				if (mobileStations[i].end_time == time && mobileStations[i].connected) {
					// Disconnecting the mobile
					connected--;
					Station* tempStation = mobileStations[i].station;
					tempStation->disconnect(&mobileStations[i]);
					// Correcting the bitrate of connected mobile of current station
					int sizeOfStation = tempStation->mobileStations.size();
					for (int itr = 0; itr < sizeOfStation; ++itr) {
						double temp = tempStation->mobileStations[itr]->bitrate;
						temp = (temp*(sizeOfStation + 1)) / sizeOfStation;
						tempStation->mobileStations[itr]->bitrate = temp;
					}
				}
			}

			instantBits = 0.0;
			for (unsigned int i = 0; i < mobileStations.size(); ++i) {
				instantBits += mobileStations[i].bitrate;
			}
			totalBitsTransferred += instantBits;

			averageBitsTransferred = totalBitsTransferred / time;
			//std::cout << "average Bits Transferred\tBiasing - (" << PICO_BIAS << ") " << averageBitsTransferred << std::endl;
			
			timeThroughput[time] = (float)averageBitsTransferred;
			instantThroughput[time] = (float)instantBits;
			if (PER_DEVICE_THROUGHPUT && connected > 0) {
				instantThroughput[time] /= connected;
			}
		}

		timeThroughput[TIME + 1] = 0.0;
		instantThroughput[TIME + 1] = 0.0;
		for (int i = 1; i <= TIME; ++i) {
			if (timeThroughput[i] > timeThroughput[TIME + 1]) timeThroughput[TIME + 1] = timeThroughput[i];
			if (instantThroughput[i] > instantThroughput[TIME + 1]) instantThroughput[TIME + 1] = instantThroughput[i];
		}
		timeThroughput[TIME + 1] *= 1.75;
		instantThroughput[TIME + 1] *= 1.75;

		biasEffect[(int)(2 * PICO_BIAS * 10)] = (float)throughput;
		std::cout << "toal Bits Transferred\tBiasing = " << PICO_BIAS << " - " << totalBitsTransferred << std::endl;
	}
	break;
	case METHOD_K:
	{
		std::vector<Station*> stations;
		double totalBitsTransferred = 0.0;
		double instantBits = 0.0;
		double throughput = 0.0;
		int connected = 0;
		for (unsigned int i = 0; i < baseStations.size(); ++i) stations.push_back(&baseStations[i]);
		for (unsigned int i = 0; i < picoStations.size(); ++i) stations.push_back(&picoStations[i]);

		for (unsigned int i = 0; i < baseStations.size(); ++i) {
			baseStations[i].mobileStations.clear();
		}
		for (unsigned int i = 0; i < picoStations.size(); ++i) {
			picoStations[i].mobileStations.clear();
		}
		for (unsigned int i = 0; i < mobileStations.size(); ++i) {
			mobileStations[i].start_time = mobileStations[i].initial_start_time;
			mobileStations[i].connected = false;
			mobileStations[i].bitrate = 0.0;
			mobileStations[i].station = NULL;
		}

		for (int time = 1; time <= TIME; ++time) {
			for (unsigned int i = 0; i < mobileStations.size(); ++i) {
				if (mobileStations[i].start_time == time) {
					// Connecting the mobile
					double totalPower = 0;
					for (unsigned int j = 0; j < stations.size(); ++j) {
						totalPower += stations[j]->powerAtUnbiased(mobileStations[i].location);
					}
					double mini = INT_MIN;
					Station *s = NULL;
					for (unsigned int j = 0; j < stations.size(); ++j) {
						if (stations[j]->canConnect(&mobileStations[i])) {
							double presentPower = stations[j]->powerAtUnbiased(mobileStations[i].location);
							double interference = totalPower - presentPower;
							double bitRate = expected_bitrate(stations[j], presentPower, interference, K);
							if (mini < bitRate && bitRate>0.0) {
								mini = bitRate;
								s = stations[j];
							}
						}
					}
					if (s != nullptr) {
						s->connect(&mobileStations[i]);
						connected++;
						double interference = totalPower - (s->powerAtUnbiased(mobileStations[i].location));
						double SINR = (s->powerAtUnbiased(mobileStations[i].location)) / interference;
						double tempbitrate = (BANDWIDTH)*log2(1 + SINR);
						// Correcting the bitrate of connected mobile of current station
						int sizeOfStation = s->mobileStations.size();
						for (int itr = 0; itr < sizeOfStation - 1; ++itr) {
							double temp = s->mobileStations[itr]->bitrate;
							temp = (temp*(sizeOfStation - 1)) / sizeOfStation;
							s->mobileStations[itr]->bitrate = temp;
						}
						mobileStations[i].bitrate = tempbitrate / sizeOfStation;
					}
					else {
						if (mobileStations[i].end_time - mobileStations[i].start_time > 1) mobileStations[i].start_time++;
					}
				}
				if (mobileStations[i].end_time == time && mobileStations[i].connected) {
					// Disconnecting the mobile
					connected--;
					Station* tempStation = mobileStations[i].station;
					tempStation->disconnect(&mobileStations[i]);
					int sizeOfStation = tempStation->mobileStations.size();
					for (int itr = 0; itr < sizeOfStation; ++itr) {
						double temp = tempStation->mobileStations[itr]->bitrate;
						temp = (temp*(sizeOfStation + 1)) / sizeOfStation;
						tempStation->mobileStations[itr]->bitrate = temp;
					}
				}
			}

			instantBits = 0.0;
			for (unsigned int i = 0; i < mobileStations.size(); ++i) {
				instantBits += mobileStations[i].bitrate;
			}
			totalBitsTransferred += instantBits;

			double averageBitsTransferred = totalBitsTransferred / time;
			//std::cout << "average Bits Transferred\tk = " << K << " - " << averageBitsTransferred << std::endl;
			
			timeThroughput[time] = (float)averageBitsTransferred;
			instantThroughput[time] = (float)instantBits;
			if (PER_DEVICE_THROUGHPUT && connected > 0) {
				instantThroughput[time] /= connected;
			}
		}
		
		std::cout << "toal Bits Transferred\tk = " << K << " - " << totalBitsTransferred << std::endl;

		timeThroughput[TIME + 1] = 0.0;
		instantThroughput[TIME + 1] = 0.0;
		for (int i = 1; i <= TIME; ++i) {
			if (timeThroughput[i] > timeThroughput[TIME + 1]) timeThroughput[TIME + 1] = timeThroughput[i];
			if (instantThroughput[i] > instantThroughput[TIME + 1]) instantThroughput[TIME + 1] = instantThroughput[i];
		}
		timeThroughput[TIME + 1] *= 1.75;
		instantThroughput[TIME + 1] *= 1.75;
	}
	break;
	}
}


// Highly toxic method - Do Not Touch
// Timing will be moved outside (OpenGL.cpp), K will be picked from globals
// Will become easy to visualize, smooth simulation
void connectInteractive(std::vector<MobileStation>& mobileStations, std::vector<BaseStation>& baseStations, std::vector<PicoStation>& picoStations, int method, int time) {
	if (time > 0 && time <= TIME) {
		refresh = true;
		switch (method) {
		case METHOD_BIAS:
		{
			std::vector<Station*> stations;
			double totalBitsTransferred = 0.0;
			int connected = 0;
			double throughput = 0.0;
			double averageBitsTransferred;
			//double totalPower = 0.0;
			for (unsigned int i = 0; i < baseStations.size(); ++i) {
				baseStations[i].mobileStations.clear();
				stations.push_back(&baseStations[i]);
			}
			for (unsigned int i = 0; i < picoStations.size(); ++i) {
				picoStations[i].mobileStations.clear();
				stations.push_back(&picoStations[i]);
			}
			for (unsigned int i = 0; i < mobileStations.size(); ++i) {
				mobileStations[i].connected = false;
				mobileStations[i].bitrate = 0.0;
				mobileStations[i].station = NULL;
			}

			for (unsigned int i = 0; i < mobileStations.size(); ++i) {
				if (mobileStations[i].start_time == time) {
					// Connect the mobile
					sort(stations.begin(), stations.end(), [&](Station* s1, Station* s2) {
						return s1->powerAt(mobileStations[i].location) > s2->powerAt(mobileStations[i].location);
					});
					unsigned int j = 0;
					double totalPower = 0.0;
					for (j = 0; j < stations.size(); ++j) {
						totalPower += stations[j]->powerAtUnbiased(mobileStations[i].location);
					}
					j = 0;
					while (j < stations.size() && stations[j]->powerAt(mobileStations[i].location) >= MIN_POWER && !stations[j]->connect(&mobileStations[i])) j++;
					if (mobileStations[i].connected) {
						double presentPower = stations[j]->powerAtUnbiased(mobileStations[i].location);
						double interference = totalPower - presentPower;
						double SINR = presentPower / interference;
						double tempbitrate = (BANDWIDTH)*log2(1 + SINR);	// Mbps (small b in Mb)
						connected++;
						// Correcting the bitrate of connected mobile of current station
						int sizeOfStation = stations[j]->mobileStations.size();
						for (int itr = 0; itr < sizeOfStation; ++itr) {
							double temp = stations[j]->mobileStations[itr]->bitrate;
							temp = (temp*(sizeOfStation - 1)) / sizeOfStation;
							stations[j]->mobileStations[itr]->bitrate = temp;
						}
						mobileStations[i].bitrate = tempbitrate / sizeOfStation;
					}
				}
				if (mobileStations[i].end_time == time) {
					// Disconnecting the mobile
					Station* tempStation = mobileStations[i].station;
					tempStation->disconnect(&mobileStations[i]);
					// Correcting the bitrate of connected mobile of current station
					int sizeOfStation = tempStation->mobileStations.size();
					for (int itr = 0; itr < sizeOfStation; ++itr) {
						double temp = tempStation->mobileStations[itr]->bitrate;
						temp = (temp*(sizeOfStation + 1)) / sizeOfStation;
						tempStation->mobileStations[itr]->bitrate = temp;
					}
				}
			}
			for (unsigned int i = 0; i < mobileStations.size(); ++i) {
				totalBitsTransferred += mobileStations[i].bitrate;
			}
			averageBitsTransferred = totalBitsTransferred / time;
			std::cout << "average Bits Transferred\tBiasing - (" << PICO_BIAS << ") " << averageBitsTransferred << std::endl;

			//throughputArray[4] = (float)throughput;
			biasEffect[(int)(2 * PICO_BIAS * 10)] = (float)throughput;
			// avgThr[(int)(2*PICO_BIAS * 10)] = (float)(throughput/connected);
			std::cout << "Throughput\tBiasing - " << "(" << PICO_BIAS << ") " << throughput << std::endl;
		}
		break;
		case METHOD_K:
		{
			std::vector<Station*> stations;
			double totalBitsTransferred = 0.0;
			double throughput = 0.0;
			for (unsigned int i = 0; i < baseStations.size(); ++i) stations.push_back(&baseStations[i]);
			for (unsigned int i = 0; i < picoStations.size(); ++i) stations.push_back(&picoStations[i]);
			for (unsigned int i = 0; i < baseStations.size(); ++i) {
				baseStations[i].mobileStations.clear();
			}
			for (unsigned int i = 0; i < picoStations.size(); ++i) {
				picoStations[i].mobileStations.clear();
			}
			for (unsigned int i = 0; i < mobileStations.size(); ++i) {
				mobileStations[i].connected = false;
				mobileStations[i].bitrate = 0.0;
				mobileStations[i].station = NULL;
			}

			for (unsigned int i = 0; i < mobileStations.size(); ++i) {
				if (mobileStations[i].start_time == time) {
					// Connecting the mobile
					double totalPower = 0;
					for (unsigned int j = 0; j < stations.size(); ++j) {
						totalPower += stations[j]->powerAtUnbiased(mobileStations[i].location);
					}
					double mini = INT_MIN;
					Station *s = NULL;
					for (unsigned int j = 0; j < stations.size(); ++j) {
						if (stations[j]->canConnect(&mobileStations[i])) {
							double presentPower = stations[j]->powerAtUnbiased(mobileStations[i].location);
							double interference = totalPower - presentPower;
							double bitRate = expected_bitrate(stations[j], presentPower, interference, K);
							if (mini < bitRate && bitRate>0.0) {
								mini = bitRate;
								s = stations[j];
							}
						}
					}
					if (s != nullptr) {
						s->connect(&mobileStations[i]);
						double interference = totalPower - (s->powerAtUnbiased(mobileStations[i].location));
						double SINR = (s->powerAtUnbiased(mobileStations[i].location)) / interference;
						double tempbitrate = (BANDWIDTH)*log2(1 + SINR);
						// Correcting the bitrate of connected mobile of current station
						int sizeOfStation = s->mobileStations.size();
						for (int itr = 0; itr < sizeOfStation - 1; ++itr) {
							double temp = s->mobileStations[itr]->bitrate;
							temp = (temp*(sizeOfStation - 1)) / sizeOfStation;
							s->mobileStations[itr]->bitrate = temp;
						}
						mobileStations[i].bitrate = tempbitrate / sizeOfStation;
					}
				}
				if (mobileStations[i].end_time == time) {
					// Disconnecting the mobile
					Station* tempStation = mobileStations[i].station;
					tempStation->disconnect(&mobileStations[i]);
					int sizeOfStation = tempStation->mobileStations.size();
					for (int itr = 0; itr < sizeOfStation; ++itr) {
						double temp = tempStation->mobileStations[itr]->bitrate;
						temp = (temp*(sizeOfStation + 1)) / sizeOfStation;
						tempStation->mobileStations[itr]->bitrate = temp;
					}
				}
			}
			for (unsigned int i = 0; i < mobileStations.size(); ++i) {
				totalBitsTransferred += mobileStations[i].bitrate;
			}
			double averageBitsTransferred = totalBitsTransferred / time;
			std::cout << "average Bits Transferred\tk = " << K << " - " << averageBitsTransferred << std::endl;

			// throughputArray[4] = throughputArray[0];
			// if (throughputArray[1] > throughputArray[4]) throughputArray[4] = throughputArray[1];
			// if (throughputArray[2] > throughputArray[4]) throughputArray[4] = throughputArray[2];
			// if (throughputArray[3] > throughputArray[4]) throughputArray[4] = throughputArray[3];
			// throughputArray[4] *= 1.75;
		}
		break;
		}
	}
}

void disconnect(std::vector<MobileStation>& mobileStations, std::vector<BaseStation>& baseStations, std::vector<PicoStation>& picoStations) {
	// disconnect all basestations and picostations. corresponding mobile stations will automatically disconnect
	for (BaseStation& base : baseStations) {
		base.disconnect();
	}
	for (PicoStation& pico : picoStations) {
		pico.disconnect();
	}
}

// This function will change assign modified variables like bias, capacity etc.
void reconfigure(std::vector<MobileStation>& mobileStations, std::vector<BaseStation>& baseStations, std::vector<PicoStation>& picoStations) {
	/*for (MobileStation& mobile : mobileStations) {
		
	}*/
	for (BaseStation& base : baseStations) {
		base.capacity = BASE_STATION_CAPACITY;
		base.power = BASE_POWER;
		base.bias = BASE_BIAS;
	}
	for (PicoStation& pico : picoStations) {
		pico.capacity = PICO_STATION_CAPACITY;
		pico.power = PICO_POWER;
		pico.bias = PICO_BIAS;
	}
}


float toGLX(int x) {
	return ((float)(2 * x) / WINDOW_WIDTH) - 1;
}

float toGLY(int y) {
	return ((float)(2 * y) / WINDOW_HEIGHT) - 1;
}

//ye wala function upar daal dena
double expected_bitrate(Station* s, double presentPower, double interference, int k) {
	if (s->getInfo() == "BaseStation") {
		BaseStation* bs = dynamic_cast<BaseStation*>(s);
		double nb = bs->mobileStations.size();
		double SINR = presentPower / interference;
		double p = (BANDWIDTH / (nb + nbPlusOne)) * log2(1 + SINR);
		if (nb == bs->capacity) return -1;
		if (k == 0) return p;
		if (k == 1) {
			double q = (BANDWIDTH / bs->capacity) * log2(1 + SINR);
			return q;
		}
		if (k == 2) {
			return (p * ((nb + 1) / bs->capacity));
		}
		if (k == 3) {
			double temp = bs->capacity / (bs->capacity - nb - 1);
			return (p * temp);
		}
		return -2;
	}
	else if (s->getInfo() == "PicoStation") {
		PicoStation* ps = dynamic_cast<PicoStation*>(s);
		double nb = ps->mobileStations.size();
		double SINR = presentPower / interference;
		double p = (BANDWIDTH / (nb + nbPlusOne)) * log2(1 + SINR);
		if (nb == ps->capacity) return -1;
		if (k == 0) return p;
		if (k == 1) {
			double q = (BANDWIDTH / ps->capacity) * log2(1 + SINR);
			return q;
		}
		if (k == 2) {
			return (p * ((nb + 1) / ps->capacity));
		}
		if (k == 3) {
			double temp = ps->capacity / (ps->capacity - nb - 1);
			return (p * temp);
		}
		return -2;
	}
	return -2;
}
