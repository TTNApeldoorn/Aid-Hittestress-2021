/*--------------------------------------------------------------------
  LoRaWAN_APP wrapper class
  Hide he Heltec framework for the user and expose the convenient methods
  Author Marcel Meek
  Date 13/1/2021
  --------------------------------------------------------------------*/

#include "LoRaWan_APP.h"

class LoRaWan {
  public:
    LoRaWan();
    ~LoRaWan();
    
    void begin();
    void sendMsg(int port, void* buf, int len);
    void setWorker( void (*callback)());
    void setSleepTime( int sleepTime);
    void process();
   
  private:

};
