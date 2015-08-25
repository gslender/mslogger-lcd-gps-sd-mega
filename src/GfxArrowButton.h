
#ifndef _GFXARROWBUTTON_H_
#define _GFXARROWBUTTON_H_

#include "GfxButton.h"

#define GFXARROW_UP 0
#define GFXARROW_DOWN 1
#define GFXARROW_LEFT 2
#define GFXARROW_RIGHT 3

class GfxArrowButton : public GfxButton  {
    
    
public:
    
    GfxArrowButton();
    virtual ~GfxArrowButton() {};
    void create(Adafruit_TFTLCD* _tft,unsigned int _x, unsigned int _y, unsigned int _w, unsigned int _h, byte _dir, unsigned int _fgColour, unsigned int _bgColour);
    void draw();
    void drawPressed();
    
private:
    int x1,y1,x2,y2,x3,y3;
};

#endif
