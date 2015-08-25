
#include "GfxDataField.h"

// Constructor
GfxDataField::GfxDataField() {}

void GfxDataField::create(Adafruit_TFTLCD* _tft,unsigned int _x, unsigned int _y, char _fntsize, char _place, unsigned int _bgColour, unsigned int _fgColour) {
    
    GfxLabel::create(_tft, _bgColour, _fgColour);
    x = _x;
    y = _y;
    fntsize = _fntsize;
    place = _place;
    setValue(0);
}

void GfxDataField::setValue(int value)
{
    prepDrawText(x,y,fntsize);
    checkShrinkage((char)tft->print(value));
}

void GfxDataField::setValue(float value)
{
    prepDrawText(x,y,fntsize);
    checkShrinkage((char)tft->print(value,place));
}

void GfxDataField::setUnknown()
{
    prepDrawText(x,y,fntsize);
    checkShrinkage((char)tft->print(F("?")));
}

void GfxDataField::checkShrinkage(char newLength)
{
    if (newLength < lastLength)
    {
        tft->fillRect(x+(newLength*fntsize*6), y, (lastLength-newLength)*fntsize*6, fntsize*8, bgColour);
    }
    lastLength = newLength;
}
