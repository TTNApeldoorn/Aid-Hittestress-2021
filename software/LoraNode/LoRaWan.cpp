/*--------------------------------------------------------------------
  LoRaWAN_APP wrapper class
  Hide he Heltec framework for the user and expose the convenient methods
  Author Marcel Meek
  Date 13/1/2021
  Update 25/4/2021 Use Unique ChipID for TTN DEVEUI
  --------------------------------------------------------------------*/

#include "LoRaWan.h"

/*
   set LoraWan_RGB to Active,the RGB active in loraWan
   RGB red means sending;
   RGB purple means joined done;
   RGB blue means RxWindow1;
   RGB yellow means RxWindow2;
   RGB green means received done;
*/

/* OTAA para*/
uint8_t devEui[8];  // Leave empty, the chip-id of thec Heltec board is filled in after start-up
uint8_t appEui[] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x01, 0xFE, 0x1F };
uint8_t appKey[] = { 0x46, 0xBF, 0x7A, 0x1A, 0x2E, 0x95, 0x9F, 0xE9, 0xA6, 0xE8, 0xAE, 0x12, 0x12, 0xE5, 0x22, 0x35};

/* ABP para*/
uint8_t nwkSKey[] = {};
uint8_t appSKey[] = {};
uint32_t devAddr = 0;

/*LoraWan channelsmask, default channels 0-7*/
uint16_t userChannelsMask[6] = { 0x00FF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 };

LoRaMacRegion_t loraWanRegion = ACTIVE_REGION;   /*LoraWan region, select in arduino IDE tools*/
DeviceClass_t  loraWanClass = LORAWAN_CLASS;   /*LoraWan Class, Class A and Class C are supported*/
uint32_t appTxDutyCycle = 20000;  /*the application data transmission duty cycle.  value in [ms].*/
/*OTAA or ABP*/
bool overTheAirActivation = LORAWAN_NETMODE;     /*ADR enable*/
bool loraWanAdr = LORAWAN_ADR;
/* set LORAWAN_Net_Reserve ON, the node could save the network info to flash, when node reset not need to join again */
bool keepNet = LORAWAN_NET_RESERVE;
bool isTxConfirmed = LORAWAN_UPLINKMODE;    /* Indicates if the node is sending confirmed or unconfirmed messages */
uint8_t appPort = 2;   /* Application port */
/*!
  Number of trials to transmit the frame, if the LoRaMAC layer did not
  receive an acknowledgment. The MAC performs a datarate adaptation,
  according to the LoRaWAN Specification V1.0.2, chapter 18.4, according
  to the following table:

  Transmission nb | Data Rate
  ----------------|-----------
  1 (first)       | DR
  2               | DR
  3               | max(DR-1,0)
  4               | max(DR-1,0)
  5               | max(DR-2,0)
  6               | max(DR-2,0)
  7               | max(DR-3,0)
  8               | max(DR-3,0)

  Note, that if NbTrials is set to 1 or 2, the MAC will not decrease
  the datarate, in case the LoRaMAC layer did not receive an acknowledgment
*/
uint8_t confirmedNbTrials = 4;

uint16_t baseline;
int maxtry = 50;

static void (*_callback)() = NULL; 

LoRaWan::LoRaWan() {
  _callback = NULL;
}

LoRaWan::~LoRaWan() {}

void LoRaWan::begin() {

  #if(AT_SUPPORT)
    enableAt();
  #endif

// Print chip id and copy it to DEVEUI
  uint64_t chipID = getID();
  printf("DEVEUI: ");
  uint8_t *ptr = (uint8_t *)&chipID;
  for( int i=0; i<8; i++) {    // reverse copy
    devEui[i] = ptr[ 7-i];
    printf( "%02X ", devEui[i]);
  }
  printf("\n");

  deviceState = DEVICE_STATE_INIT;
  printf("deviceState=%d\n", deviceState);
  LoRaWAN.ifskipjoin();
}

void LoRaWan::setSleepTime( int sleepTime) {
  appTxDutyCycle = sleepTime;
}

void LoRaWan::setWorker( void (*callback)()) {
  _callback = callback;
}

void LoRaWan::sendMsg(int port, void* buf, int len){
  appPort = port;
  appDataSize = len;
  memcpy( appData, buf, len);
}

void LoRaWan::process() {
  switch ( deviceState )
  {
    case DEVICE_STATE_INIT:
      {
        printf("DEVICE_STATE_INIT\n");
#if(AT_SUPPORT)
        getDevParam();
#endif
        printDevParam();
        LoRaWAN.init(loraWanClass, loraWanRegion);
        deviceState = DEVICE_STATE_JOIN;
        break;
      }
    case DEVICE_STATE_JOIN:
      {
        printf("DEVICE_STATE_JOIN\n");
        LoRaWAN.join();
        break;
      }
    case DEVICE_STATE_SEND:
      {
        printf("DEVICE_STATE_SEND\n");
        if( _callback) {
          _callback();
          LoRaWAN.send();
        }
        deviceState = DEVICE_STATE_CYCLE;
        break;
      }
    case DEVICE_STATE_CYCLE:
      {
        printf("DEVICE_STATE_CYCLE\n");
        // Schedule next packet transmission
        LoRaWAN.cycle( appTxDutyCycle /*+ randr( 0, APP_TX_DUTYCYCLE_RND )*/);  // APP_TX_DUTYCYCLE_RND = 1000
        deviceState = DEVICE_STATE_SLEEP;
        break;
      }
    case DEVICE_STATE_SLEEP:
      {
        //printf(".");
        LoRaWAN.sleep();
        break;
      }
    default:
      {
        printf("DEFAULT\n");
        deviceState = DEVICE_STATE_INIT;
        break;
      }
  }
}
