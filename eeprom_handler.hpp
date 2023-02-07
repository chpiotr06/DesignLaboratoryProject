int word_address = 0;


void addRoutePoint(const double &lon, const double &lat, int lhours,  int lminutes, int lseconds) {
    EEPROM.put(word_address, lhours);
    word_address += 2;
    EEPROM.put(word_address, lminutes);
    word_address += 2;
    EEPROM.put(word_address, lseconds);
    word_address += 2;

    EEPROM.put(word_address, lon);
    word_address += 4;
    EEPROM.put(word_address, lat);
    word_address += 4;
}

void eraseRoute() {
  Serial.println(F("\nErasing route...\n"));
  for (int i = 0 ; i < EEPROM.length() ; i++)
    EEPROM.write(i, 0);
  word_address = 0;
}

void printRoute() {

  Serial.println(F("\nYour route:\n"));
  double dbl = 0;
  int it = 0 ;
  
  for (int i = 0; i < ( word_address / 14 ); i++){
    for(int j = 0; j < 6 ; j+=2 ){
      Serial.print(EEPROM.get(j, it));
      Serial.print(':');
    }
    Serial.print(':');
    for(int j = 0; i < 8; j++) {
      Serial.print(EEPROM.get(j, dbl));
      Serial.print(':');
    }
    Serial.print('\n');
  }
}
