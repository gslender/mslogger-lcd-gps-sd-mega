
#ifndef _GFXINDICATOR_H_
#define _GFXINDICATOR_H_

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "Adafruit_TFTLCD.h"

class GfxIndicator {
    
public:
    
    GfxIndicator();
    void create(Adafruit_TFTLCD* _tft,unsigned int _x, unsigned int _y, unsigned int _w, unsigned int _h, byte _fntsize);
    void setState(byte newstate, unsigned int fgColour, unsigned int bgColour, const __FlashStringHelper *ifsh);
        
private:
    Adafruit_TFTLCD* tft;
    unsigned int x;
    unsigned int y;
    unsigned int w;
    unsigned int h;
    byte fntsize;
    byte state;
};

#endif
