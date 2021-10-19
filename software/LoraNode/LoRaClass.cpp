/*--------------------------------------------------------------------
  LoRaClass wrapper class
  Hide he Heltec framework for the user and expose the convenient methods
  Author Marcel Meek
  Date 7/10/2021
  --------------------------------------------------------------------*/
#include "LoRaWanMinimal_APP.h"
#include "LoRaClass.h"

//Set these OTAA parameters to match your app/node in TTN
uint8_t devEui[8];  // Leave empty, the chip-id of the Heltec board is filled in after start-up
uint8_t appEui[] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x01, 0xFE, 0x1F };
uint8_t appKey[] = { 0x46, 0xBF, 0x7A, 0x1A, 0x2E, 0x95, 0x9F, 0xE9, 0xA6, 0xE8, 0xAE, 0x12, 0x12, 0xE5, 0x22, 0x35};

/*LoraWan channelsmask, default channels 0-7*/
uint16_t userChannelsMask[6] = { 0x00FF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 };

static void (*_rxCallback)(int, uint8_t*, int) = NULL;

///////////////////////////////////////////////////
//Some utilities for going into low power mode
//TimerEvent_t sleepTimer;
//Records whether our sleep/low power timer expired
static bool sleepTimerExpired;
static TimerEvent_t sleepTimer;

static void wakeUp() {
  sleepTimerExpired=true;
}

static void lowPowerSleep(uint32_t sleeptime) {
  delay(10);  // some time to flush serial data
  sleepTimerExpired=false;
  TimerInit( &sleepTimer, &wakeUp );
  TimerSetValue( &sleepTimer, sleeptime );
  TimerStart( &sleepTimer );
  //Low power handler also gets interrupted by other timers
  //So wait until our timer had expired
  while (!sleepTimerExpired)
    lowPowerHandler();
  TimerStop( &sleepTimer );
}


//downlink data handle function
void downLinkDataHandle(McpsIndication_t *mcpsIndication)
{
  Serial.printf("Download message slot: %s\n", mcpsIndication->RxSlot?"RXWIN2":"RXWIN1");
  if( _rxCallback) {
    _rxCallback( mcpsIndication->Port, mcpsIndication->Buffer , mcpsIndication->BufferSize);
  }
}

LoRaClass::LoRaClass() {
  _rxCallback = NULL;
}

LoRaClass::~LoRaClass() {}

void LoRaClass::begin() {
  // get chip id and copy it to DEVEUI
  uint64_t chipID = getID();
  printf("DEVEUI: ");
  uint8_t *ptr = (uint8_t *)&chipID;
  for( int i=0; i<8; i++) {    // reverse copy
    devEui[i] = ptr[ 7-i];
    printf( "%02X ", devEui[i]);
  }
  printf("\n");
  
  LoRaWAN.begin(LORAWAN_CLASS, ACTIVE_REGION);  /*LoraWan region, select in arduino IDE tools*/
  //Enable ADR
  LoRaWAN.setAdaptiveDR(true);

  while( true) {
    Serial.print("Joining... ");
    LoRaWAN.joinOTAA(appEui, appKey, devEui);
    if (!LoRaWAN.isJoined()) {
      //In this example we just loop until we're joined, but you could
      //also go and start doing other things and try again later
      Serial.println("JOIN FAILED! Sleeping for 60 seconds");
      lowPowerSleep(60000);
    } else {
      Serial.println("JOINED");
      break;
    }
  }
}

void LoRaClass::sleep( int sleepTime) {
  lowPowerSleep( sleepTime);
}

void LoRaClass::setRxHandler( void (*callback)(int, uint8_t*, int)) {
  _rxCallback = callback;
}

void LoRaClass::send(int port, void *buf, int len) {
  if (LoRaWAN.send( len, (uint8_t *)buf, port, false))   // send unconfirmed
    Serial.println("Send OK");
  else
   Serial.println("Send FAILED");
}
