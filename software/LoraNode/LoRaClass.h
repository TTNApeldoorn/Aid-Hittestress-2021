/*--------------------------------------------------------------------
  LoRaClass wrapper class
  Hide he Heltec framework for the user and expose the convenient methods
  Author Marcel Meek
  Date 7/10/2021
  --------------------------------------------------------------------*/

class LoRaClass {
  public:
    LoRaClass();
    ~LoRaClass();
    
    void begin();
    void send( int port, void* buf, int len);
    void setRxHandler( void (*callback)( int, uint8_t*, int));
    void sleep( int sleepTime);
};
