
#include "GfxIndicator.h"

// Constructor
GfxIndicator::GfxIndicator() {}

void GfxIndicator::create(Adafruit_TFTLCD* _tft,unsigned int _x, unsigned int _y, unsigned int _w, unsigned int _h, byte _fntsize) {
    tft = _tft;
    x = _x;
    y = _y;
    w = _w;
    h = _h;
    fntsize = _fntsize;
    state = -1;
}

void GfxIndicator::setState(byte newstate, unsigned int fgColour, unsigned int bgColour, const __FlashStringHelper *ifsh)
{
	String s = String(ifsh);
    if (newstate != state)
    {
    	const char* text = s.c_str();
        tft->fillRect(x, y,w,h, bgColour);  
        unsigned int x1 = x+(w-(strlen(text)*6*fntsize))/2;
        unsigned int y1 = y+(h-8*fntsize)/2;
        tft->setCursor(x1,y1);
        tft->setTextSize(fntsize);
        tft->setTextColor(fgColour, bgColour);
        tft->print(text);
        state = newstate;
    }
}
