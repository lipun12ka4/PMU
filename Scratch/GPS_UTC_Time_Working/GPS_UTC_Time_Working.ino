#include <TinyGPS++.h>
static const uint32_t GPSBaud = 38400;

TinyGPSPlus gps;

void setup() {
  // GPS connected to Serial 2
  Serial.begin(115200);
  Serial2.begin(GPSBaud);
  
  
}

void loop() {
  // put your main code here, to run repeatedly:
  while (Serial2.available() > 0)
  gps.encode(Serial2.read());
  
  if (gps.location.isUpdated())
  {
    Serial.print("LAT="); Serial.print(gps.location.lat(), 6);
    Serial.print("LNG="); Serial.println(gps.location.lng(), 6);
  }
  if (gps.date.isValid())
  {
    Serial.print("Date="); 
    Serial.print(gps.date.value());
    Serial.print("  ");
    Serial.print(gps.date.day()); // Day (1-31) (u8)
    Serial.print("/");
    Serial.print(gps.date.month()); // Month (1-12) (u8)
    Serial.print("/");
    Serial.println(gps.date.year()); // Year (2000+) (u16)

  }
  if (gps.time.isValid())
  {
    Serial.print("Time="); 
    Serial.print(gps.time.value());
    Serial.print("  ");
    Serial.print(gps.time.hour());
    Serial.print(":");
    Serial.print(gps.time.minute());
    Serial.print(":");
    Serial.print(gps.time.second());
    Serial.print(":");
    Serial.println(gps.time.centisecond());
    
  }
  
}
