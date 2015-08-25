
#include "GfxArrowButton.h"


// Constructor
GfxArrowButton::GfxArrowButton() {

    x1 = 0,y1 = 0,x2 = 0,y2 = 0,x3 = 0,y3 = 0;
}

void GfxArrowButton::create(Adafruit_TFTLCD* _tft,unsigned int _x, unsigned int _y, unsigned int _w, unsigned int _h, byte _dir, unsigned int _fgColour, unsigned int _bgColour) {
    
    GfxButton::create(_tft,_x,_y,_w,_h,_fgColour,_bgColour);
    
    if (_dir == GFXARROW_DOWN)
    {
        x1 = x;
        y1 = y;
        x2 = x+w;
        y2 = y;
        x3 = x+(w/2);
        y3 = y+h;
    }
    
    if (_dir == GFXARROW_UP)
    {
        x1 = x;
        y1 = y+h;
        x2 = x+(w/2);
        y2 = y;
        x3 = x+w;
        y3 = y+h;
    }
}

void GfxArrowButton::draw()
{
    tft->fillTriangle(x1, y1, x2 , y2, x3 ,y3, fgColour);
    tft->drawTriangle(x1, y1, x2 , y2, x3 ,y3, bgColour);
}


void GfxArrowButton::drawPressed()
{
    tft->fillTriangle(x1, y1, x2 , y2, x3 ,y3, bgColour);
}
