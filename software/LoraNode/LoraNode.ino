/*--------------------------------------------------------------------
  Hittestress 2021 LoRaNode based on Heltec CubeCell LoRa board
  Sensor is equipped with Temp/Hum AM2305, Dust sensor SDS011 and GPS ATGM33
  Libraries used: Heltec LoRaWan_APP, DHT, SDS011(refactored), SoftwareSerial, TinyGPS++

  Author Marcel Meek
  Date 13/1/2021
  Update 25/4/2021  measurement and status message separated resp. port 15 and 16
  --------------------------------------------------------------------*/
#include <TinyGPS++.h>
#include <softSerial.h>
#include "LoRaClass.h"

#include <DHT.h>
#include "SDS011.h"

#define CYCLETIME  150000     // LoRa message cycle time  24 messages per hour
#define GPSTIMEOUT 120000    // max timeout to get a GPS fix
#define VERSION 1.5

// payload structs
struct Measurement {
  int16_t  temp, hum, pm2_5, pm10, batt;
};
struct Status {
  float lat, lng;
  int16_t alt, hdop, batt, version;
};

#define GPS_RX  GPIO0
#define GPS_TX  GPIO5
#define SDS_RX  GPIO3
#define SDS_TX  GPIO2

// NOTE: there are two SofwtareSerials, one for the SDS and one for the GPS. 
// They can't be used simultaneously. Instantiate them exclusively.
softSerial* portGPS = NULL;
softSerial* portSDS = NULL;
SDS011* sds = NULL;
LoRaClass lora;

static long msgCount = 0;           // message count
static int vbatCurr = INT_MAX, vbatPrev = INT_MAX;  // mV
static float prevPm10 = -1.0, prevPm25 = -1.0;
static bool resetRequest = false;

DHT dht(GPIO1, DHT22, 2);       // Initialize DHT (in our case AM2302) sensor for normal 16mhz Arduino

TinyGPSPlus tinyGpsPlus;  // The TinyGPS++ object


 // attach soft Serial to SDS and deattach GPS
void attachSDS() {
  if( portSDS == NULL) {
    delete portGPS;  portGPS = NULL;
    portSDS = new softSerial( SDS_TX, SDS_RX);
    portSDS->begin( 9600); delay( 100);
    sds = new SDS011( *portSDS);
  }
}

 // attach soft Serial to GPS and detach SDS
void attachGPS() {
  if( portGPS == NULL) {
    delete portSDS;  portSDS = NULL;
    delete sds; sds = NULL;
    portGPS = new softSerial( GPS_TX, GPS_RX);
    portGPS->begin( 9600); delay( 100);
  }
}

// check if battery is OK or battery is charging
bool isPowerOk() {
   vbatPrev = vbatCurr;
   vbatCurr = getBatteryVoltage();
printf("Vbat curr=%d\n", vbatCurr);
   return( vbatCurr > 3550 || vbatCurr-2 > vbatPrev); 
}


bool readDHT( float &t, float &h) {
   t = dht.readTemperature();
   h = dht.readHumidity();
   if( isnan(t)|| isnan(h)) {
     delay(1700);  Serial.println("AM2305 retry");  // minimum time to get new values from AM2305
     t = dht.readTemperature();
     h = dht.readHumidity();
   }
//Serial.print(t); Serial.print(" ");  Serial.println(h);
}

// read SDS dust sensor
// the sensor is read 5 times, the average value will be returnd
void readSDS( float &pm25, float &pm10) {
  // read 10 times and take average
  float sum25 = 0.0, sum10 = 0.0;
  int count = 0;

// 5 reads
  for(int i=0; i<5; i++) {
    delay( 1200);  
//Serial.print("."); // minimum time to get new values from SDS011
    if( sds->read( &pm25, &pm10) ) {
      sum10 += pm10;
      sum25 += pm25;   
//Serial.print(pm10); Serial.print(" "); 
      count++;
    }
  }
//Serial.println();
  // calculate average
  pm10 = sum10 / count;
  pm25 = sum25 / count;
  //printf("count=%d\n", count);

// take average with previous cycle
  if( prevPm10 > 0.0 ) {
    pm10 = (pm10+ prevPm10) / 2.0;
    pm25 = (pm25 + prevPm25) / 2.0;
  }
  prevPm10 = pm10;
  prevPm25 = pm25;
}

void processMeasurement( Measurement &m) {
  printf("processMeasurement\r\n");
  float t, h, pm25, pm10;
  readDHT( t, h);
  // start SDS fan to start airflow
  sds->wakeup();
  delay( 5000); // take 5 + 1.2 seconds for a good airflow
 
  //readDHT( t, h);  // take 5 seconds (also delay for airflow)
  // assume airflow is on speed
  readSDS( pm25, pm10);  
 
  m.temp = t * 100;
  m.hum = h * 100;
  m.pm2_5 = pm25 * 100;
  m.pm10 = pm10 * 100;
  m.batt = vbatCurr; 
  Serial.print( "temp="); Serial.print( t);
  Serial.print( " hum="); Serial.print(  h);
  Serial.print( " pm10="); Serial.print( pm10 );
  Serial.print( " pm25="); Serial.print( pm25);
  Serial.print( " batt="); Serial.println( vbatCurr);
  sds->sleep();
  delay(100);
}

// read Status
// return true, if a fix is detected within the timeout.
bool processStatus( Status &status) {
  printf("processStatus\n");
  bool fix = false;
  int i = 0;
  long start = millis();

  // leave loop after timout or
  // if Vext power is switched off, exit the loop (caused by RGB_LORAWAN Led, be sure that it is deactivated)
  while( millis() - start < GPSTIMEOUT && digitalRead(Vext) == 0) {   
    if( portGPS->available() > 0 ) {
      char c = portGPS->read();
      //Serial.print(c);
      tinyGpsPlus.encode( c); i++;
      if( tinyGpsPlus.location.isUpdated()) {
        status.lat = tinyGpsPlus.location.lat();
        status.lng = tinyGpsPlus.location.lng();
        status.hdop = 1000 * tinyGpsPlus.hdop.hdop();
        status.alt = tinyGpsPlus.altitude.meters();
        fix = true;
        break;
      }
    }
  }
  printf("GPS chars read:%d\n", i);
  status.batt = vbatCurr;
  status.version = VERSION * 100;
  Serial.print("lat: ");Serial.print( status.lat);
  Serial.print(" lon: ");Serial.print( status.lng);
  Serial.print(" alt: "); Serial.print( status.alt);
  Serial.print(" hdop: "); Serial.print( status.hdop / 1000.0);
  Serial.print(" batt: "); Serial.print( status.batt / 1000.0);
  Serial.print(" version: "); Serial.println( status.version / 100.0);

  return fix;
}

// prepare and send Lora Message
void procesSensors( ) {
  printf( "procesSensors\n");  

  if( msgCount % 96 == 1) {  // a status cycle (each 96 cycles, is each 4 hours)  
    digitalWrite(Vext, LOW);    // switch the gps power on
    attachGPS();
    Status status = {0.0, 0.0, 0, 0, 0, 0};
    processStatus( status);
    printf("send status len=%d\n", sizeof(status));
    attachSDS();
    digitalWrite(Vext, HIGH);   // switch gps power off 
    //loRaWan.sendMsg( 16, (void*)&status, sizeof(status));     // status lora port 16
    lora.send( 16, &status, sizeof(status));
  }
  else {  // send measurement
    Measurement measurement;
    processMeasurement( measurement);   // read temp, hum, and PM
    printf("send measurement len=%d\n", sizeof(measurement));
    //loRaWan.sendMsg( 15, (void*)&measurement, sizeof(measurement));  // data lora port 15
    lora.send( 15, &measurement, sizeof(measurement));
  }
}

// work process, called by the Heltec framework after deepsleep timeout
void worker( ) {
  printf("Worker %d\n", msgCount);
  long start = millis();

// check power
  if( isPowerOk() ) {
    procesSensors();
    msgCount++;
  }

  int sleeptime = CYCLETIME - (millis() - start);     // cycletime minus processtime
  if(sleeptime <20000)
     sleeptime = 20000;         // minimum sleeptime must be 20 sec, needed for the Heltec framework
  
  printf("sleep for %d ms\n", sleeptime);
  lora.sleep( sleeptime);
}

// setup
// initialize devices
void setup() {
  Serial.begin(115200); delay(500);
  Serial.println( "*** Starting Hittestress 2021 V" + String(VERSION) + " ***");

 printf( "resetRequest=%d\n", resetRequest);
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, HIGH);  // GPS off

 // assign soft Serial to SDS
  attachSDS();
  delay(1000); // wait for SDS is powered
  sds->sleep();  // v1.21
  
  while( !isPowerOk() ) {
    printf("Battery is too empty or not charging, sleep for %d ms\n", CYCLETIME);
    lora.sleep( CYCLETIME);
  }
  lora.setRxHandler( loraRxCallback); 
  lora.begin();

  Serial.println( "end setup");
}

// run loop
void loop() {
  worker();
  
  if( resetRequest) {   
    if( msgCount > 4) {    // prevent reset loop, caused by previous downlink reset command
      printf("HW RESET..\n"); delay(100);
      HW_Reset(0);
    }
    resetRequest = false;
  }

// periodical reset after 5000 cycles
  if( msgCount > 5000) {
     resetRequest = true;
  }
    
}


// LoRa receive handler (downnlink)
void loraRxCallback( int port, uint8_t* msg, int len) {
  printf("lora download message received port=%d cmd=%d len=%d\n", port, msg[0], len);
  // if port is 01 and command byte is 0xaa then reset the sensor
  if ( port == 1 && len == 1 && msg[0] == 0xaa) {
    resetRequest = true;
    printf("RESET COMMAND REVEIVED, planned in next cycle\n");
  }
}
