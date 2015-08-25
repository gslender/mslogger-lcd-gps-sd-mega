#ifndef megasquirt_h
#define megasquirt_h

#define MS_CELSIUS 1
#define MS_FAHRENHEIT 0
#define MS_ENGINE_READY 1
#define MS_ENGINE_CRANKING 2
#define MS_ENGINE_STARTW 4
#define MS_ENGINE_WARMUP 8
#define MS_ENGINE_TPSACCAEN 16
#define MS_ENGINE_TPSACCDEN 32
#define MS_ENGINE_MAPACCAEN 64
#define MS_ENGINE_MAPACCDEN 128

#include "Arduino.h"

class MegaSquirt {
    
public:
    
    MegaSquirt();
    byte getEngine();
    unsigned int getRpm();
    float getMap();
    float getMat(byte celsius);
    float getClt(byte celsius);
    float getTps();
    float getAfr();
    float getTaf();
    float getAdv();
    float getPwp();
    int requestData();
    void requestSignature();
    
private:
    byte data[400];
    int runCommand(byte cmd[], byte cmdLength,  byte data[], byte dataLength);
};


#endif

