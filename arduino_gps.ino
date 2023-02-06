#include <SPI.h>
#include <EEPROM.h>
#include "epd2in9.h"
#include "epdpaint.h"
#include "VMA430_GPS.h"



#define COLORED     0
#define UNCOLORED   1

unsigned char image[1024];
Paint paint(image, 0, 0);    // width should be the multiple of 8
Epd epd;

unsigned long time_start_ms;
unsigned long time_now_s;

SoftwareSerial ss(2, 3); // RX, TX
VMA430_GPS gps(&ss);

double dist = 0, avg_spd, prev_lon, prev_lat;


void(*resetFunc) (void) = 0;

int word_address = 0;
bool routeIsRecorded = false;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println(F("Hello user! Here is your GPS data"));
  gps.begin(9600);
  gps.setUBXNav();
  if (epd.Init(lut_full_update) != 0) {
    Serial.print(F("e-Paper init failed"));
    return;
  }
  if (epd.Init(lut_partial_update) != 0) {
    Serial.print(F("e-Paper init failed"));
    return;
  }

  time_start_ms = millis();
  delay(500);
}


void loop() {


  dist = gps_dist(gps.location.latitude, gps.location.longitude, prev_lon, prev_lat);

  time_now_s = (millis() - time_start_ms) / 1000;

  char time_string[] = "Time: 00:00:00";
  char distance[] = "Lon:        N";
  char azimuth[]  = "Lat:        E";
  char avgVelocity[] = "AVG Spd:          ";
  char velocity[] = "Dist: 00.0   ";
  time_string[9] = time_now_s / 60 / 10 + '0';
  time_string[10] = time_now_s / 60 % 10 + '0';
  time_string[12] = time_now_s % 60 / 10 + '0';
  time_string[13] = time_now_s % 60 % 10 + '0';

  dtostrf(gps.location.latitude, 2, 5, &distance[5]);
  dtostrf(gps.location.longitude, 2, 5, &azimuth[5]);
  distance[12] = 'N';
  azimuth[12] = 'E';

  dtostrf(dist, 2, 5, &velocity[5]);
  velocity[12] = 'm';

  avg_spd = (dist) / (double)time_now_s;
  dtostrf( avg_spd, 2, 5, &avgVelocity[8]);
  avgVelocity[15] = 'm';
  avgVelocity[16] = '/';
  avgVelocity[17] = 's';


  paint.SetWidth(24);
  paint.SetHeight(290);
  paint.SetRotate(ROTATE_90);

  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 4, time_string, &Font20, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());

  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 4, distance, &Font20, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 24, 0, paint.GetWidth(), paint.GetHeight());

  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 4, azimuth, &Font20, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 48, 0, paint.GetWidth(), paint.GetHeight());

  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 4, velocity, &Font20, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 72, 0, paint.GetWidth(), paint.GetHeight());

  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 4, avgVelocity, &Font20, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 96, 0, paint.GetWidth(), paint.GetHeight());

  epd.DisplayFrame();

  // put your main code here, to run repeatedly:

  if (gps.getUBX_packet()) // If a valid GPS UBX data packet is received...
  {
    gps.parse_ubx_data();
    // Print location (latitude/longitude)
    Serial.print(F("Lat: "));
    Serial.print(gps.location.latitude, 8); // to 8 decimals
    Serial.print(F(" Long: ") );
    Serial.print(gps.location.longitude, 8); // to 8 decimals
    Serial.print(F("Avg spd: ") );
    Serial.print(avg_spd); // to 8 decimals
    Serial.print(F("Dist: ") );
    Serial.print(dist); // to 8 decimals


  }

  if (routeIsRecorded)
    addRoutePoint(gps.location.latitude, gps.location.longitude);


  prev_lon = gps.location.longitude;
  prev_lat = gps.location.latitude;



  if (Serial.available()) {
    char code = Serial.read();
    switch (code) {
      case '!':
        resetFunc();
        break;
      case 'r':
        eraseRoute();
        break;
      case 's':
        routeIsRecorded = true;
        break;
      case 'f':
        routeIsRecorded = false;
        break;
      case 'p':
        printRoute();
        break;
      case 'c': 
        calculateCalories();
        break;
      default:
        break;
    }
  }
}



long double toRadians(const long double degree)
{
  // cmath library in C++
  // defines the constant
  // M_PI as the value of
  // pi accurate to 1e-30
  long double one_deg = (M_PI) / 180;
  return (one_deg * degree);
}

long double gps_dist(long double lat1, long double long1,
                     long double lat2, long double long2)
{
  // Convert the latitudes
  // and longitudes
  // from degree to radians.
  lat1 = toRadians(lat1);
  long1 = toRadians(long1);
  lat2 = toRadians(lat2);
  long2 = toRadians(long2);

  // Haversine Formula
  long double dlong = long2 - long1;
  long double dlat = lat2 - lat1;

  long double ans = pow(sin(dlat / 2), 2) +
                    cos(lat1) * cos(lat2) *
                    pow(sin(dlong / 2), 2);

  ans = 2 * asin(sqrt(ans));

  // Radius of Earth in
  // Kilometers, R = 6371
  // Use R = 3956 for miles
  long double R = 6371;

  // Calculate the result
  ans = ans * R;

  return ans ;
}

void Hello() {
  paint.Clear(UNCOLORED);

  char hello[] = "Waiting for data...";
  paint.DrawStringAt(0, 4, hello, &Font20, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());

}


void addRoutePoint(const double &lon, const double &lat) {
  if (gps.utc_time.valid) // If the utc_time passed from the GPS is valid...
  {
    EEPROM.put(word_address, gps.utc_time.hour);
    word_address += 2;
    EEPROM.put(word_address, gps.utc_time.minute);
    word_address += 2;
    EEPROM.put(word_address, gps.utc_time.second);
    word_address += 2;

    EEPROM.put(word_address, lon);
    word_address += 4;
    EEPROM.put(word_address, lat);
    word_address += 4;
  }
  else
    Serial.println(F("Error saving route data - cannot collect time data from GPS satelite"));
}

void eraseRoute() {
  routeIsRecorded = false;
  for (int i = 0 ; i < EEPROM.length() ; i++)
    EEPROM.write(i, 0);
  word_address = 0;
}

void printRoute() {
  
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

void calculateCalories() { 
  Serial.println(F("Too few! Move your ass fatty"));
}
