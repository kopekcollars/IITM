/*
Automation bd
Facebook:         https://web.facebook.com/groups/844063765941384
Website:          https://automationbd1.blogspot.com
YouTube:          https://www.youtube.com/channel/UCz4uQlukt3UfEjh2oEqpySg
Map Address check: https://www.maps.ie/coordinates.html
*/

#include "TinyGPS++.h"
#include "SoftwareSerial.h"

SoftwareSerial serial_connection(5,6); //tx,rx 
TinyGPSPlus gps;// GPS object to process the NMEA data

// Placeholder for starting latitude and longitude
double startLat = 0, startLng = 0;
float totalDistance = 0; // in meters
int totalSteps = 0;

void setup()
{
  Serial.begin(9600);                //This opens up communications to the Serial monitor in the Arduino IDE
  serial_connection.begin(9600);     //This opens up communications to the GPS
  Serial.println("GPS Start")        //To show in the serial monitor that the sketch has started

  // Wait for GPS connection
  Serial.print(F("Waiting for GPS signal "));
  while (!gps.location.isValid()) {
    while (serial_connection.available() > 0) {
      gps.encode(serial_connection.read());
    }
  }

  Serial.println(F("GPS signal acquired!"));

  startLat = gps.location.lat();
  startLng = gps.location.lng();

}

void loop()
{
  while(serial_connection.available())              //While there are incoming characters  from the GPS
  {
    gps.encode(serial_connection.read());           //This feeds the serial NMEA data into the library one char at a time
  }
  if(gps.location.isUpdated())          //This will pretty much be fired all the time anyway but will at least reduce it to only after a package of NMEA data comes in
  {
    //Get the latest info from the gps object which it derived from the data sent by the GPS unit
    Serial.print("Satellite Count:");
    Serial.println(gps.satellites.value());
    Serial.print("Latitude:");
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitude:");
    Serial.println(gps.location.lng(), 6);

    
    Serial.print("Altitude Feet:");
    Serial.println(gps.altitude.feet());
    Serial.println("");

    Serial.print("Date: ");
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print("/");
    Serial.print(gps.date.day());
    Serial.print("/");
    Serial.println(gps.date.year());
  }
  
  Serial.print("Time: ");
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(":");
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(":");
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(".GMT");
  }

  Serial.println("");

  // Current position
  double currentLat = gps.location.lat();
  double currentLng = gps.location.lng();

  // Calculate distance travelled since last check
  float distanceTravelled = TinyGPSPlus::distanceBetween(
    startLat, startLng,
    currentLat, currentLng
  );

  // Convert meters to steps assuming 0.5 meter stride length
  int steps = distanceTravelled / 0.5;

  // Update total distance and steps
  totalDistance += distanceTravelled;
  totalSteps += steps;

  // Display current distance and steps
  Serial.print(F("Distance: "));
  Serial.print(totalDistance);
  Serial.print(F(" m, Steps: "));
  Serial.println(totalSteps);

  // Update starting position for next calculation
  startLat = currentLat;
  startLng = currentLng;
  
  delay(5000);
  }
  
}
