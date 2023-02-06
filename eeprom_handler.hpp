//int word_address = 0;
//
//
//void addRoutePoint(const double &lon, const double &lat) {
//  if (gps.utc_time.valid) // If the utc_time passed from the GPS is valid...
//  {
//    EEPROM.put(word_address, gps.utc_time.hour);
//    word_address += 2;
//    EEPROM.put(word_address, gps.utc_time.minute);
//    word_address += 2;
//    EEPROM.put(word_address, gps.utc_time.second);
//    word_address += 2;
//
//    EEPROM.put(word_address, lon);
//    word_address += 4;
//    EEPROM.put(word_address, lat);
//    word_address += 4;
//  }
//  else
//    Serial.println(F("Error saving route data - cannot collect time data from GPS satelite"));
//}
//
//void eraseRoute() {
//  routeIsRecorded = false;
//  for (int i = 0 ; i < EEPROM.length() ; i++)
//    EEPROM.write(i, 0);
//  word_address = 0;
//}
//
//void printRoute() {
//  
//  double dbl = 0;
//  int it = 0 ;
//  
//  for (int i = 0; i < ( word_address / 14 ); i++){
//    for(int j = 0; j < 6 ; j+=2 ){
//      Serial.print(EEPROM.get(j, it));
//      Serial.print(':');
//    }
//    Serial.print(':');
//    for(int j = 0; i < 8; j++) {
//      Serial.print(EEPROM.get(j, dbl));
//      Serial.print(':');
//    }
//    Serial.print('\n');
//  }
//}
