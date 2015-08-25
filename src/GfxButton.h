
#ifndef _GFXBUTTON_H_
#define _GFXBUTTON_H_

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "Adafruit_TFTLCD.h"
#include "TouchScreen.h"
#include <SoftwareSerial.h>

#define MINPRESSURE 400
#define MAXPRESSURE 1000
#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940

class GfxButton {
    
    
public:
    
    GfxButton();
    virtual ~GfxButton() {};
    virtual void create(Adafruit_TFTLCD* _tft,unsigned int _x, unsigned int _y, unsigned int _w, unsigned int _h, unsigned int _fgColour, unsigned int _bgColour);
    virtual void draw();
    virtual void drawPressed();
    virtual bool isPressed(Point p);
//    virtual void setDebug(SoftwareSerial* _debug);
    virtual void swapColours();
    
protected:
//    SoftwareSerial* debug;
    Adafruit_TFTLCD* tft;
    int16_t x;
    int16_t y;
    int16_t w;
    int16_t h;
    unsigned int fgColour;
    unsigned int bgColour;
    bool armed;
};

#endif
