void connect(std::vector<MobileStation> &mobileStations, std::vector<BaseStation> &baseStations, std::vector<PicoStation> &picoStations, int method)
{

	switch (method)
	{
	case METHOD_BIAS:
	{
		std::vector<Station *> stations;
		double totalBitsTransferred = 0.0;
		int connected = 0;
		//double totalPower = 0.0;
		for (unsigned int i = 0; i < baseStations.size(); ++i) stations.push_back(&baseStations[i]);
		for (unsigned int i = 0; i < picoStations.size(); ++i) stations.push_back(&picoStations[i]);
		for (unsigned int i = 0; i < mobileStations.size(); ++i) {
			mobileStations[i].connected = false;
			mobileStations[i].station = NULL;
			mobileStations[i].bitrate = 0.0;
		}

		for (unsigned int i = 0; i < picoStations.size(); ++i) {
			picoStations[i].mobileStations.clear();
		}
		for (unsigned int i = 0; i < baseStations.size(); ++i) {
			baseStations[i].mobileStations.clear();
		}
		for (int time = 0; time <= Time; time++) {
			for (unsigned int i = 0; i < mobileStations.size(); ++i) {
				if (mobileStations[i].start_time == time) { //connecting the mobile
					sort(stations.begin(), stations.end(), [&](Station *s1, Station *s2) {
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
						double tempbitrate = (BANDWIDTH)*log2(1 + SINR); // Mbps (small b in Mb)
						//mobileStations[i].bitrate = tempbitrate;
						connected++;
						int sizeofstation = stations[j]->mobileStations.size(); //correcting the bitrate of connected mobile of current station
						for (int itr = 0; itr < stations[j]->mobileStations.size() - 1; ++itr) {
							double temp = stations[j]->mobileStations[itr].bitrate;
							temp = (temp * (sizeofstation - 1)) / sizeofstation;
							stations[j]->mobileStations[itr].bitrate = temp;
						}
						mobileStations[i].bitrate = tempbitrate / sizeofstation;
					}
				}
				if (mobileStations[i].end_time == time) {
					//disconnecting the mobile
					Station *tempstation = mobileStations[i]->station;
					mobileStations[i].bitrate = 0.0;
					tempstation->disconnect(&mobileStations[i]);
					int sizeofstation = tempstation->mobileStations.size();
					for (int itr = 0; itr < tempstation->mobileStations.size(); ++itr) { //correcting the bitrate of connected mobile of current station
						double temp = tempstation->mobileStations[itr].bitrate;
						temp = (temp * (sizeofstation + 1)) / sizeofstation;
						tempstation->mobileStations[itr].bitrate = temp;
					}
				}
			}
			for (unsigned int i = 0; i < mobileStations.size(); ++i) {
				totalBitsTransferred += mobileStations[i].bitrate;
			}
		}
		//throughputArray[4] = (float)throughput;
		//biasEffect[(int)(2*PICO_BIAS * 10)] = (float)throughput;
		// avgThr[(int)(2*PICO_BIAS * 10)] = (float)(throughput/connected);
		double averageBitsTransferred = totalBitsTransferred / Time;
		std::cout << "average Bits Transferred\tBiasing - "
				  << "(" << PICO_BIAS << ") " << averageBitsTransferred << std::endl;
	}
	break;
	case METHOD_K:
	{
		std::vector<Station *> stations;
		for (unsigned int i = 0; i < baseStations.size(); ++i) stations.push_back(&baseStations[i]);
		for (unsigned int i = 0; i < picoStations.size(); ++i) stations.push_back(&picoStations[i]);
		for (int loop = 0; loop < 4; ++loop)
		{
			for (unsigned int i = 0; i < mobileStations.size(); ++i)
			{
				mobileStations[i].connected = false;
				mobileStations[i].station = NULL;
				mobileStations[i].bitrate = 0.0;
			}

			for (unsigned int i = 0; i < picoStations.size(); ++i) {
				picoStations[i].mobileStations.clear();
			}
			for (unsigned int i = 0; i < baseStations.size(); ++i) {
				baseStations[i].mobileStations.clear();
			}
			for (int time = 0; time <= Time; time++) {
				double totalBitsTransferred = 0.0;

				for (unsigned int i = 0; i < mobileStations.size(); ++i) {
					if (mobileStations[i].start_time == time) { 
						//connecting the mobile
						double totalPower = 0;
						for (unsigned int j = 0; j < stations.size(); j++) {
							totalPower += stations[j]->powerAtUnbiased(mobileStations[i].location);
						}
						double mini = INT_MIN;
						Station *s = NULL;
						for (unsigned int j = 0; j < stations.size(); j++) {
							double presentPower = stations[j]->powerAtUnbiased(mobileStations[i].location);
							double interference = totalPower - presentPower;
							double bitRate = expected_bitrate(stations[j], presentPower, interference, loop);
							if (mini < bitRate) {
								mini = bitRate;
								s = stations[j];
							}
						}
						if (s != nullptr) {
							s->connect(&mobileStations[i]);
							double interference = totalPower - (s->powerAtUnbiased(mobileStations[i].location));
							double SINR = (s->powerAtUnbiased(mobileStations[i].location)) / interference;
							double tempbitrate = (BANDWIDTH)*log2(1 + SINR);
							mobileStations[i].bitrate = tempbitrate;
							//throughput += bitrate;
							int sizeofstation = s->mobileStations.size(); //correcting the bitrate of connected mobile of current station
							for (int itr = 0; itr < s->mobileStations.size() - 1; ++itr) {
								double temp = s->mobileStations[itr].bitrate;
								temp = (temp * (sizeofstation - 1)) / sizeofstation;
								s->mobileStations[itr].bitrate = temp;
							}
							mobileStations[i].bitrate = tempbitrate / sizeofstation;
						}
					}
					if (mobileStations[i].end_time == time) { //disconnecting the mobile
						Station *tempstation = mobileStations[i]->station;
						mobileStations[i].bitrate = 0.0;
						tempstation->disconnect(&mobileStations[i]);
						int sizeofstation = tempstation->mobileStations.size();
						for (int itr = 0; itr < tempstation->mobileStations.size(); ++itr) { //correcting the bitrate of connected mobile of current station
							double temp = tempstation->mobileStations[itr].bitrate;
							temp = (temp * (sizeofstation + 1)) / sizeofstation;
							tempstation->mobileStations[itr].bitrate = temp;
						}
					}
				}
				for (unsigned int i = 0; i < mobileStations.size(); ++i) {
					totalBitsTransferred += mobileStations[i].bitrate;
				}
				//throughputArray[loop] = (float)throughput;
				// biasEffectK[loop][(int)(PICO_BIAS * 10)] = (float)throughput;
				double averageBitsTransferred = totalBitsTransferred / time;
				std::cout << "average Bits Transferred\tk = " << loop << " - " << averageBitsTransferred << std::endl;
			}
		}
		throughputArray[4] = throughputArray[0];
		if (throughputArray[1] > throughputArray[4]) throughputArray[4] = throughputArray[1];
		if (throughputArray[2] > throughputArray[4]) throughputArray[4] = throughputArray[2];
		if (throughputArray[3] > throughputArray[4]) throughputArray[4] = throughputArray[3];
		throughputArray[4] *= 1.75;
	}
	break;
	}
}
