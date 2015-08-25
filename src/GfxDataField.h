
#ifndef _GFXDATAFIELD_H_
#define _GFXDATAFIELD_H_

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "Adafruit_TFTLCD.h"
#include "GfxLabel.h"

class GfxDataField : public GfxLabel {
    
public:
    
    GfxDataField();
    void create(Adafruit_TFTLCD* _tft,unsigned int _x, unsigned int _y, char _fntsize, char _place, unsigned int _bgColour, unsigned int _fgColour);
    void setValue(int value);
    void setValue(float value);
    void setUnknown();
        
private:
    unsigned int x;
    unsigned int y;
    char fntsize;
    char place;
    char lastLength;
    void checkShrinkage(char newLength);
};

#endif
