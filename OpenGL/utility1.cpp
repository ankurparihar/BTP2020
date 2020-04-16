//ye wala function upar daal dena
double expected_bitrate(Station* s, double presentPower, double interference, int k){
  double nb = s->mobileStations.size();
  double SINR = presentPower / interference;
  double Bandwidth = 10;
  double p = ( Bandwidth / nb ) * log2( 1 + SINR );
  if(nb == s->capacity) return -1;
  if(k == 0) return p;
  if(k == 1){
    double q = ( Bandwidth / s->capacity ) * log2( 1 + SINR );
    return q;
  }
  if(k == 2){
    return (p * ( (nb + 1) / s->capacity));
  }
  if(k == 3){
    double temp = s->capacity / (s->capacity - nb - 1) ;
    return (p * temp);
  }
  return -2;
}

// yaha se pura void connect wale function me jayega. github me line no 65 ke neeche.
for (unsigned int i = 0; i < mobileStations.size(); ++i){
  mobileStations[i]->connected = false;
}
for (unsigned int i = 0; i < stations.size(); ++i){
  stations[i]->mobileStations.clear;
}
for (unsigned int i = 0; i < mobileStations.size(); ++i) {
    double totalPower=0;
    for(j = 0; j < stations.size(); j++){
      totalPower += stations[j]->powerAt(mobileStations[i].location);
    }
    double mini = INT_MIN;
    Station* s;
    for(j = 0; j < stations.size(); j++){
      double presentPower = stations[j]->powerAt(mobileStations[i].location);
      double interference = totalPower - presentPower;
      double bitRate = expected_bitrate(stations[j], presentPower, interference, 2);
      if(mini < bitRate){
        mini = bitRate;
        s = stations[j];
      }
    }
    s->connect(&mobileStations[i]);
}
