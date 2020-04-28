void connect(std::vector<MobileStation>& mobileStations, std::vector<BaseStation>& baseStations, std::vector<PicoStation>& picoStations, int method) {

	switch (method) {
	case METHOD1:
	{
		std::vector<Station*> stations;
		for (unsigned int i = 0; i < baseStations.size(); ++i) stations.push_back(&baseStations[i]);
		for (unsigned int i = 0; i < picoStations.size(); ++i) stations.push_back(&picoStations[i]);
		double throughput = 0, Bandwidth = 20.0;
		for (unsigned int i = 0; i < mobileStations.size(); ++i) {
			sort(stations.begin(), stations.end(), [&](Station* s1, Station* s2) {
				return s1->powerAt(mobileStations[i].location) > s2->powerAt(mobileStations[i].location);
			});
			unsigned int j = 0;
			double totalPower = 0;
			for (unsigned int j = 0; j < stations.size(); j++) {
				totalPower += stations[j]->powerAt(mobileStations[i].location);
			}
			j = 0;
			while (j < stations.size() && !stations[j++]->connect(&mobileStations[i]));
			double presentPower = stations[j-1]->powerAt(mobileStations[i].location);
			double interference = totalPower - presentPower;
			double SINR = presentPower / interference;
			double bitrate = (Bandwidth) * log2(1 + SINR);
			throughput += bitRate;
		}
    cout<<"Throughput\tBiasing - "<<throughput<<endl;
	}
	break;
	case METHOD2:
	{
		std::vector<Station*> stations;
		for (unsigned int i = 0; i < baseStations.size(); ++i) stations.push_back(&baseStations[i]);
		for (unsigned int i = 0; i < picoStations.size(); ++i) stations.push_back(&picoStations[i]);
    for(int loop = 0; loop < 4; loop++){
      double throughput = 0, Bandwidth = 20;
  		for (unsigned int i = 0; i < mobileStations.size(); ++i) {
  			mobileStations[i].connected = false;
  			mobileStations[i].station = NULL;
  		}

  		for (unsigned int i = 0; i < picoStations.size(); ++i) {
  			picoStations[i].mobileStations.clear();
  		}
  		for (unsigned int i = 0; i < baseStations.size(); ++i) {
  			baseStations[i].mobileStations.clear();
  		}

  		for (unsigned int i = 0; i < mobileStations.size(); ++i) {
  			double totalPower = 0;
  			for (unsigned int j = 0; j < stations.size(); j++) {
  				totalPower += stations[j]->powerAt(mobileStations[i].location);
  			}
  			double mini = INT_MIN;
  			Station* s = NULL;
  			for (unsigned int j = 0; j < stations.size(); j++) {
  				double presentPower = stations[j]->powerAt(mobileStations[i].location);
  				double interference = totalPower - presentPower;
  				double bitRate = expected_bitrate(stations[j], presentPower, interference, loop);
  				if (mini < bitRate) {
  					mini = bitRate;
  					s = stations[j];
  				}
  			}
  			if (s != nullptr) {
          s->connect(&mobileStations[i]);
          double interference = totalPower - (s->powerAt(mobileStations[i].location));
          double SINR = (s->powerAt(mobileStations[i].location)) / interference;
          double bitrate = (Bandwidth) * log2(1 + SINR);
          throughput += bitRate;
        }
  		}
    }
    cout<<"Throughput\tk = "<<loop<<" - "<<throughput<<endl;
	}
	break;
	}
}
