#include <SPI.h>
#include <EEPROM.h>
#include "epd2in9.h"
#include "epdpaint.h"
#include "VMA430_GPS.h"
#include "dist_calc.hpp"
#include "eeprom_handler.hpp"

#define COLORED     0
#define UNCOLORED   1

unsigned long time_start_ms;
unsigned long time_now_s;

SoftwareSerial ss(2, 3); // RX, TX
VMA430_GPS gps(&ss);

double dist = 0, avg_spd = 0, lon = 0, lat = 0, prev_lon = 0, prev_lat = 0;

void(*resetFunc) (void) = 0;

int word_address = 0;
bool routeIsRecorded = false;

bool UBX_packet_ready = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println(F("Hello user!\n"));
  gps.begin(9600);
  gps.setUBXNav();
  epaperTest();
  epaperClear();

  delay(50);
  time_start_ms = millis();
}

void loop() {
  if (gps.getUBX_packet()) // If a valid GPS UBX data packet is received...
  {
    lat = gps.location.latitude;
    lon = gps.location.longitude;
  }

  if(lon == 0 && lat == 0)
    Serial.println("Waiting for GPS signal...");
  else {
    dist = gps_dist(lat, lon, prev_lat, prev_lon);
    epaperPrint();
    bluetoothPrintData();

  //  if (routeIsRecorded)
  //    addRoutePoint(lat, lon);
  }

  prev_lon = lon;
  prev_lat = lat;

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


void epaperTest() {

  unsigned char image[1024];
  Paint paint(image, 0, 0);    // width should be the multiple of 8
  Epd epd;
  if (epd.Init(lut_full_update) != 0) {
    Serial.print(F("e-Paper init failed"));
    return;
  }

  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();
  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();

  if (epd.Init(lut_partial_update) != 0) {
    Serial.print(F("e-Paper init failed"));
    return;
  }
}

void epaperClear() {
  unsigned char image[1024];
  Paint paint(image, 0, 0);    // width should be the multiple of 8
  Epd epd;

  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();
  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();

  char message_l1[] = "Waiting for ";
  char message_l2[] = "GPS signal...";

  paint.SetWidth(24);
  paint.SetHeight(290);
  paint.SetRotate(ROTATE_90);

  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 4, message_l2, &Font20, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 72, 0, paint.GetWidth(), paint.GetHeight());

  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 4, message_l1, &Font20, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 96, 0, paint.GetWidth(), paint.GetHeight());

  epd.DisplayFrame();
}
void epaperPrint() {
  unsigned char image[1024];
  Paint paint(image, 0, 0);    // width should be the multiple of 8
  Epd epd;

  if (epd.Init(lut_partial_update) != 0) {
    Serial.print(F("e-Paper update failed"));
    return;
  }

  if (UBX_packet_ready) {


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

    dtostrf(lat, 2, 5, &distance[5]);
    dtostrf(lon, 2, 5, &azimuth[5]);
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
  }
}

void bluetoothPrintData() {
    Serial.print(F("\nLat: "));
    Serial.print(lat, 8); // to 8 decimals
    Serial.print(F("\nLong: ") );
    Serial.print(lon, 8); // to 8 decimals
    Serial.print(F("\nAvg spd: ") );
    Serial.print(avg_spd); // to 8 decimals
    Serial.print(F("\nDist: ") );
    Serial.print(dist); // to 8 decimals
}

void calculateCalories() {
  Serial.println(F("Too few. Move your ass fatty!"));

}
