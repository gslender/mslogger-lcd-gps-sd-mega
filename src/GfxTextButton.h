
#ifndef _GFXTEXTBUTTON_H_
#define _GFXTEXTBUTTON_H_

#include "GfxButton.h"

class GfxTextButton : public GfxButton  {
    
    
public:
    
    GfxTextButton();
    void create(Adafruit_TFTLCD* _tft,unsigned int _x, unsigned int _y, unsigned int _w, unsigned int _h, byte _fntsize, const __FlashStringHelper *ifsh, unsigned int _fgColour, unsigned int _bgColour);
    void draw();
    void drawPressed();
        
private:
    void drawText(unsigned int col);
    byte fntsize;
    const __FlashStringHelper *ifsh;
};

#endif
