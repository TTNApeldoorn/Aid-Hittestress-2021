// SDS011 dust sensor PM2.5 and PM10
// ---------------------
//
// By R. Zschiegner (rz@madavi.de)
// April 2016
//
// Documentation:
//    - The iNovaFitness SDS011 datasheet
//
// refactored (M. Meek)

#include "SDS011.h"

static byte SDSCMD[19] = {
  0xAA, // head
  0xB4, // command id
  0x06, // data byte 1
  0x01, // data byte 2 (set mode)
  0x00, // data byte 3 (0 = sleep, 1 = wake)
  0x00, // data byte 4
  0x00, // data byte 5
  0x00, // data byte 6
  0x00, // data byte 7
  0x00, // data byte 8
  0x00, // data byte 9
  0x00, // data byte 10
  0x00, // data byte 11
  0x00, // data byte 12
  0x00, // data byte 13
  0xFF, // data byte 14 (device id byte 1)
  0xFF, // data byte 15 (device id byte 2)
  0x05, // checksum
  0xAB  // tail
};


SDS011::SDS011( Stream &s) {
  i = 0;
  stream  = &s;
}

// --------------------------------------------------------
// SDS011:read
// --------------------------------------------------------
int SDS011::read(float *p25, float *p10) {
  i=0;
  while( stream->available() && i<100) {
    buf[ i] = stream->read();
    if( i >= 9 && buf[ i] == 171 && buf[i-9] == 170 && buf[i-8] == 192  ) {  // match
      int pm25_serial, pm10_serial;
      pm25_serial = buf[ i-7];
      pm25_serial += 256 * buf[ i-6];
      pm10_serial = buf[ i-5];
      pm10_serial += 256 * buf[i-4];
      *p10 = (float)pm10_serial /10.0;
      *p25 = (float)pm25_serial /10.0;
      //Serial.print("i="); Serial.println(i);
      return true;
    }
    else { 
      i++;
    }
  }
  return false;
}

// --------------------------------------------------------
// SDS011:sleep
// --------------------------------------------------------
void SDS011::sleep() {
  SDSCMD[4] = 0x00;  //sleep code
  SDSCMD[17] = 0x05;  //checksum
  
  for (uint8_t i = 0; i < 19; i++) {
    stream->write(SDSCMD[i]);
  }
  stream->flush();
}

// --------------------------------------------------------
// SDS011:wakeup
// by Marcel Meek 
// --------------------------------------------------------
void SDS011::wakeup() {
  SDSCMD[4] = 0x01;  //wake code
  SDSCMD[17] = 0x06;  //checksum

  for (uint8_t i = 0; i < 19; i++) {
    stream->write(SDSCMD[i]);
  }
  stream->flush();
  
}
