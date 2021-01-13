/*--------------------------------------------------------------------
  Hittestress 2021 LoRaNode based on Heltec CubeCell LoRa board
  Sensor is equipped with Temp/Hum AM2305, Dust sensor SDS011 and GPS ATGM33
  Libraries used: Heltec LoRaWan_APP, DHT, SDS011(refactored), SoftwareSerial, TinyGPS++

  Author Marcel Meek
  Date 13/1/2021
  --------------------------------------------------------------------*/
#include "LoRaWan.h"
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

#include <DHT.h>
#include "SDS011.h"

#define CYCLETIME 120000     // LoRa message cycle time (2 minutes)
#define GPSTIME   100000    // max timeout to get a GPS fix
#define BAUDRATE_GPS 9600
#define BAUDRATE_SDS 9600

// payload structs
struct Measurement {
  int16_t  temp, hum, pm2_5, pm10, batt;
};
struct Location {
  float lat, lng;
};

// NOTE: there are two SofwtareSerials, one for the SDS and one for the GPS. 
// They can't be used simultaneously. Select them exclusively by "softwareSerial.begin()" function.
SoftwareSerial portGPS(GPIO0, GPIO5);  // RX / TX    // GPS pinning
SoftwareSerial portSDS(GPIO3, GPIO2);   // RX / TX   // SDS pinning
SDS011 sds( portSDS);

long msgCount = 0;           // message count
DHT dht(GPIO1, DHT22);       // Initialize DHT (in our case AM2302) sensor for normal 16mhz Arduino

TinyGPSPlus gps;  // The TinyGPS++ object
LoRaWan loRaWan;  // The LoRaWan object

// Read DHT sensor
void readDHT( float &t, float &h) {
  t = dht.readTemperature();
  h = dht.readHumidity();
  if (isnan(h) || isnan(t)) {
    printf("Failed to read from DHT22 sensor!\n");
  }
}

// read SDS dust sensor
// first the fan is started for 5 seconds, to have a steady airflow
// subsequently the sensor is read 5 times, the average value will be returnd
void readSDS( float &pm25, float &pm10) {
  printf("readSDS\n");
  portSDS.begin( BAUDRATE_SDS);     // switch to SDS serial port
  delay(200);
  sds.wakeup();
  delay( 5000); // let air flow for 5 sec.

  // read 5 times and take average
  float sum25 = 0.0, sum10 = 0.0;
  int count = 0;

  for(int i=0; i<5; i++) {
    delay(1000);
    if( sds.read( &pm25, &pm10) ) {
      sum10 += pm10;
      sum25 += pm25;
      count++;
    }
  }
  // calculate average
  pm10 = sum10 / count;
  pm25 = sum25 / count;
  printf("count=%d\n", count);
  
  sds.sleep();
  delay(100);
}

// read GPS sensor
// return true, if a fix is detected within the timeout.
bool processGPS( int timeout, Location &loc) {
  printf("processGPS\n");
  portGPS.begin(BAUDRATE_GPS);        // switch to GPS serial port

  bool fix = false;
  int i = 0;
  long start = millis();
  while( millis() - start < timeout) {   // leave loop after timout
    if( portGPS.available() > 0 ) {
      char c = portGPS.read();
      //Serial.print(c);
      gps.encode( c); i++;
      if( gps.location.isUpdated()) {
        loc.lat = gps.location.lat();
        loc.lng = gps.location.lng();
        Serial.print("lat: ");Serial.print( loc.lat);
        Serial.print(" lon: ");Serial.println( loc.lng);
        fix = true;
        break;
      }
    }
  }
  printf("GPS chars read:%d\n", i);
  return fix;
}

// process measurement
// read temperature, humidity, pm10, pm2.5 and battery
void processMeasurement( Measurement &m) {
  printf("processMeasurement\n");
  float t, h, pm25, pm10;
  readDHT( t, h);
  readSDS( pm25, pm10);  
  m.temp = t*100;
  m.hum = h*100;
  m.pm2_5 = pm25*100;
  m.pm10 = pm10*100;
  m.batt = getBatteryVoltage();;
  Serial.print( "temp="); Serial.print( t);
  Serial.print( " hum="); Serial.print( h);
  Serial.print( " pm10="); Serial.print( pm10);
  Serial.print( " pm25="); Serial.print( pm25);
  Serial.println( " batt="); Serial.print( m.batt);
}

// work process, called by the Heltec framework after deepsleep timeout
// prepare Lora Message
// Note: The message prepared in the worker will be sent afterwards (so only one message at the time)
void worker( ) {
  printf( "worker %d\n", msgCount);  
  char payload[80];
  Measurement measurement;
  Location location;
  long start = millis();
  
  if( msgCount % 60 == 1)        // a GPS cycle
    digitalWrite(Vext, LOW);    // switch the gps power on in advance (gain is 10 sec)
  
  processMeasurement( measurement);   // read temp, hum, and PM
  int i = 0;
  memcpy( payload+i, &measurement, sizeof( Measurement));
  i += sizeof( Measurement);

  bool gpsfix = false;
  if( msgCount % 60 == 1) {     // a GPS cycle, so get GPS
    gpsfix = processGPS( GPSTIME, location);
    
    if (gpsfix) {              // if a fix, append GPS info in the payload 
      memcpy( payload+i, &location, sizeof( Location));
      i += sizeof( Location);
    }
    digitalWrite(Vext, HIGH);   // switch gps power off   
  }
  printf("msg len %d\n", i);
  loRaWan.sendMsg( 2, (void*)payload, i);

// calculate sleep time
  int sleeptime = CYCLETIME - (millis() - start);     // cycletime minus processtime
  if(sleeptime <20000)
     sleeptime = 20000;         // minimum sleeptime must be 20 sec, needed for the Heltec framework
  loRaWan.setSleepTime( sleeptime);
  printf("sleep for %d ms\n", sleeptime);
  msgCount++;
}

// setup
// initialize devices
void setup() {
  boardInitMcu();
  Serial.begin(115200); delay(100);
  pinMode(Vext,OUTPUT);
  digitalWrite(Vext, HIGH);  // GPS off

  loRaWan.begin();
  loRaWan.setWorker( worker);
  loRaWan.setSleepTime( CYCLETIME);
}

// run loop
void loop() {
  loRaWan.process();
}
