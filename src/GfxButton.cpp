#include "debug.h"

#include "GfxButton.h"


// Constructor
GfxButton::GfxButton() {
	tft = NULL;
	x = 0, y = 0, w = 0, h = 0, fgColour = 0, bgColour = 0, armed = false;
}

void GfxButton::create(Adafruit_TFTLCD* _tft,unsigned int _x, unsigned int _y, unsigned int _w, unsigned int _h, unsigned int _fgColour, unsigned int _bgColour) {
    tft = _tft;
    x = _x;
    y = _y;
    w = _w;
    h = _h;
    fgColour = _fgColour;
    bgColour = _bgColour;
}

void GfxButton::swapColours()
{
    int c = fgColour;
    fgColour = bgColour;
    bgColour = c;
}

void GfxButton::draw()
{
    tft->fillRoundRect(x, y, w, h, 4, fgColour);
    tft->drawRoundRect(x, y, w, h, 4, bgColour);
}

void GfxButton::drawPressed()
{
    tft->fillRoundRect(x, y, w, h, 4, bgColour);
}

bool GfxButton::isPressed(Point p)
{    
    // TouchScreen doesn't follow TFT orientation nor dimensions
    int p_x = p.x;
    p.x = map(p.y, TS_MINY, TS_MAXY, 0, tft->width());
    p.y = map(p_x, TS_MINX, TS_MAXX, tft->height(), 0);
    
    if (armed && p.z < MINPRESSURE)
    {
        armed = false;
        delay(100);
        draw();
        return true;
    }
    
    if (p.z < MINPRESSURE) return false;

	D(debugSerial->printf(F("%d, %d, %d\r\n"),p.x,p.y,p.z);)
    if (p.x > x && p.x < x+w && p.y > y && p.y < y+h)
    {
        if (p.z > MINPRESSURE && p.z < MAXPRESSURE && !armed)
        {
            armed = true;
            drawPressed();
        }
    }
    else if (armed)
    {
        armed = false;
        draw();
    }
    return false;
}
