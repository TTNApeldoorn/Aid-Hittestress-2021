// SDS011 dust sensor PM2.5 and PM10
// ---------------------------------
//
// By R. Zschiegner (rz@madavi.de)
// April 2016
//
// Documentation:
//		- The iNovaFitness SDS011 datasheet
//
// refactored (M. Meek)

#include <SoftwareSerial.h>

class SDS011 {
	public:
		SDS011( Stream &s);
    ~SDS011() {};
		int read(float *p25, float *p10);
		void sleep();
		void wakeup();  
    unsigned int rawRead();
    
	private:
		uint8_t _pin_rx, _pin_tx;
		Stream *stream;		
    unsigned char buf[256];
    unsigned int i;
};
