#include "debug.h"
D(SoftwareSerial* debugSerial;)

#include "TempSensor.h"
#include "MegaSquirt.h"
#include "GfxDataField.h"
#include "GfxIndicator.h"
#include "GfxTextButton.h"
#include "GfxArrowButton.h"

#include "Adafruit_GFX.h"    // Core graphics library
#include "Adafruit_TFTLCD.h" // Hardware-specific library
#include "TouchScreen.h"	// Touch screen
#include "Adafruit_GPS.h" // GPS library
#include <SdFat.h>		// SD Card library
#include <EEPROM.h>
#include <SPI.h>

#define DATAFLD_TMP 0
#define DATAFLD_RPM 1
#define DATAFLD_CLT 2
#define DATAFLD_MAT 3
#define DATAFLD_MAP 4
#define DATAFLD_TPS 5
#define DATAFLD_AFR 6
#define DATAFLD_TAF 7
#define DATAFLD_ADV 8
#define DATAFLD_PWP 9

// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	DRKGRAY 0x2124
#define	LTGRAY  0xBDD7
#define	BLUE    0x001F
#define	LTBLUE  0x94DF
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF


// These are the pins for the TouchScreen
#define YP A1  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 7   // can be a digital pin
#define XP 6   // can be a digital pin

Adafruit_TFTLCD tft;   // 320 x 240
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
SoftwareSerial gpsSerial(3, 2);
Adafruit_GPS gps(&gpsSerial);
TempSensor tempSensor;
MegaSquirt megaSquirt;
SdFat sdcard;
SdFile logfile;

GfxDataField datafields[10];
GfxIndicator engIndicator;
GfxTextButton setupButton;
GfxTextButton exitButton;
unsigned long time = millis();
byte rpmWarn;
byte rpmLimit;
byte tempScale;
byte flashtime;
bool showRpmWarning = false;
bool showRpmLimit = false;
bool showLogo = true;
GfxDataField rpmWarnFld;
GfxDataField rpmLimitFld;
GfxLabel tempScaleLabel;
GfxTextButton celsiusButton;
GfxTextButton fahrenheitButton;
GfxArrowButton rpmWarnUp;
GfxArrowButton rpmWarnDn;
GfxArrowButton rpmLimitUp;
GfxArrowButton rpmLimitDn;
GfxLabel serialLabel;


void drawMainScreen()
{
	D(debugSerial->println(F(">> Main"));)
    
    //STRIPES
    byte yrow = 30;
    byte c;
    for (c=0; c<5;c++)
    {
        tft.fillRect(0, (yrow+18)+2*c*18, 320, 16, DRKGRAY);
    }

    //DATAFIELDS
    datafields[DATAFLD_TMP].create(&tft,230,yrow,2,1,BLACK,LTGRAY);
    datafields[DATAFLD_TMP].drawLabel(10,yrow,2,F("Outside Temp:"));

    yrow +=18;
    datafields[DATAFLD_RPM].create(&tft,230,yrow,2,1,DRKGRAY,WHITE);
    datafields[DATAFLD_RPM].drawLabel(10,yrow,2,F("Engine RPM:"));
    
    yrow +=18;
    datafields[DATAFLD_CLT].create(&tft,230,yrow,2,1,BLACK,LTGRAY);
    datafields[DATAFLD_CLT].drawLabel(10,yrow,2,F("Coolant Temp:"));
    
    yrow +=18;
    datafields[DATAFLD_MAT].create(&tft,230,yrow,2,1,DRKGRAY,WHITE);
    datafields[DATAFLD_MAT].drawLabel(10,yrow,2,F("Manifold Temp:"));
    
    yrow +=18;
    datafields[DATAFLD_MAP].create(&tft,230,yrow,2,1,BLACK,LTGRAY);
    datafields[DATAFLD_MAP].drawLabel(10,yrow,2,F("Manifold kPa:"));
    
    yrow +=18;
    datafields[DATAFLD_TPS].create(&tft,230,yrow,2,1,DRKGRAY,WHITE);
    datafields[DATAFLD_TPS].drawLabel(10,yrow,2,F("Throttle Pos:"));
    
    yrow +=18;
    datafields[DATAFLD_AFR].create(&tft,230,yrow,2,1,BLACK,LTGRAY);
    datafields[DATAFLD_AFR].drawLabel(10,yrow,2,F("O2 / AFR:"));
    
    yrow +=18;
    datafields[DATAFLD_TAF].create(&tft,230,yrow,2,1,DRKGRAY,WHITE);
    datafields[DATAFLD_TAF].drawLabel(10,yrow,2,F("Target AFR:"));
    
    yrow +=18;
    datafields[DATAFLD_ADV].create(&tft,230,yrow,2,1,BLACK,LTGRAY);
    datafields[DATAFLD_ADV].drawLabel(10,yrow,2,F("Spark Adv:"));
    
    yrow +=18;
    datafields[DATAFLD_PWP].create(&tft,230,yrow,2,3,DRKGRAY,WHITE);
    datafields[DATAFLD_PWP].drawLabel(10,yrow,2,F("Pulse Width:"));
    
    //INDICATORS / BUTTONS
    yrow +=20;
    engIndicator.create(&tft, 0, yrow, 140, 24, 2);
    engIndicator.setState(0, DRKGRAY, LTGRAY, F("No ECU?"));
    
    setupButton.create(&tft, 240, yrow, 80, 24, 2, F("SETUP"), BLACK, LTGRAY);
    setupButton.draw();
}

void drawLogo()
{
    tft.fillRect(0, 0, 320, 25, LTBLUE);
    tft.setCursor(75,4);
    tft.setTextSize(2);
    tft.setTextColor(LTGRAY);
    tft.print(F("RednelsRacing"));
    tft.setCursor(76,5);
    tft.setTextColor(BLACK);
    tft.print(F("RednelsRacing"));
}

void drawWarnRPM()
{
    tft.fillRect(0, 0, 320, 25, YELLOW);
    tft.setCursor(85,4);
    tft.setTextSize(2);
    tft.setTextColor(BLACK);
    tft.print(F("RPM WARNING"));
}

void drawLimitRPM()
{
    tft.fillRect(0, 0, 320, 25, RED);
    tft.setCursor(105,4);
    tft.setTextSize(2);
    tft.setTextColor(WHITE);
    tft.print(F("RPM LIMIT"));
}


bool setupLoop()
{
    Point p = ts.getPoint();
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    if (exitButton.isPressed(p))
    {
        if (rpmWarn != EEPROM.read(2)) EEPROM.write(2, rpmWarn);
        if (rpmLimit != EEPROM.read(3)) EEPROM.write(3, rpmLimit);
        if (tempScale != EEPROM.read(4)) EEPROM.write(4, tempScale);

        tft.fillScreen(BLACK);

        drawLogo();

        drawMainScreen();
        return true;
    }


    if (tempScale == MS_CELSIUS)
    {
        if (fahrenheitButton.isPressed(p))
        {
            tempScale = MS_FAHRENHEIT;
            celsiusButton.swapColours();
            fahrenheitButton.swapColours();
            celsiusButton.draw();
            fahrenheitButton.draw();
        }
    }
    else
    {
        if (celsiusButton.isPressed(p))
        {
            tempScale = MS_CELSIUS;
            celsiusButton.swapColours();
            fahrenheitButton.swapColours();
            celsiusButton.draw();
            fahrenheitButton.draw();
        }
    }

    
    if (rpmWarnUp.isPressed(p))
    {
        rpmWarn++;
        rpmWarnFld.setValue(rpmWarn*100);
    }

    if (rpmWarnDn.isPressed(p))
    {
        rpmWarn--;
        rpmWarnFld.setValue(rpmWarn*100);
    }
    
    if (rpmLimitUp.isPressed(p))
    {
        rpmLimit++;
        rpmLimitFld.setValue(rpmLimit*100);
    }
    
    if (rpmLimitDn.isPressed(p))
    {
        rpmLimit++;
        rpmLimitFld.setValue(rpmLimit*100);
    }
    
    return false;
}

void setup()
{
    Serial.begin(115200); //115200

    D(debugSerial = new SoftwareSerial(2, 3);)
    D(debugSerial->begin(9600);)
	D(debugSerial->println(F("megasquirt-lcd-duino"));)

	if (!sdcard.begin(SS, SPI_HALF_SPEED)) {
		sdcard.initErrorHalt();
	}

	gps.begin(9600);
    // turn on RMC (recommended minimum) and GGA (fix data) including altitude
    gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);

    // Set the update rate
    gps.sendCommand(PMTK_SET_NMEA_UPDATE_5HZ);   // 5 Hz update rate
    gps.sendCommand(PMTK_API_SET_FIX_CTL_5HZ);   // 5 Hz update rate
    // Request updates on antenna status, comment out to keep quiet
    // gps.sendCommand(PGCMD_ANTENNA);

    // enable interrupts to capture gps reads
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);

    tft.reset();
    
    uint16_t identifier = tft.readID();
    
    if(identifier != 0x9328)
    {
    	D(debugSerial->print(F("Unknown LCD driver chip: "));)
		D(debugSerial->println(identifier,HEX);)
    }
    
    tft.begin(identifier);
    tft.setRotation(3);
    
    tft.fillScreen(BLACK);
    
    drawLogo();
    
    byte ver1 = EEPROM.read(0);
    byte ver2 = EEPROM.read(1);
    
    if (ver1 != 0xff || ver2 != 0x01)
    {
        EEPROM.write(0, 0xff);
        EEPROM.write(1, 0x01);

        EEPROM.write(2, 60);
        EEPROM.write(3, 65);
        EEPROM.write(4, MS_FAHRENHEIT);
    }
    
    rpmWarn = EEPROM.read(2); // x100
    rpmLimit = EEPROM.read(3); // x100
    tempScale = EEPROM.read(4);
    
    drawMainScreen();
    
    flashtime = 0;
}

void dataCaptureLoop()
{
    flashtime++;
    if (showRpmWarning)
    {
        if (flashtime % 2)
        {
            drawWarnRPM();
        }
        else
        {
            tft.fillRect(0, 0, 320, 25, BLACK);
        }
    }

    float temp = (tempSensor.getTempInt100()+5)/10/10.0f;

    if (tempScale == MS_FAHRENHEIT) {
        temp = (temp*1.8f) + 32;
    }
	D(debugSerial->print(F("Temp: "));)
	D(debugSerial->println(temp);)

    datafields[DATAFLD_TMP].setValue(temp);

    if (megaSquirt.requestData() == 1)
    {
    	D(debugSerial->print(F("RPM: "));)
    	D(debugSerial->println(megaSquirt.getRpm());)

        unsigned int rpm = megaSquirt.getRpm();

        if ((rpm/100) >= rpmWarn && (rpm/100) < rpmLimit)
        {
            showRpmWarning = true;
            showRpmLimit = false;
            showLogo = false;
        }
        else if ((rpm/100) >= rpmLimit)
        {
            if (!showRpmLimit)
            {
                showRpmLimit = true;
                showRpmWarning = false;
                showLogo = false;
                drawLimitRPM();
            }
        }
        else if (!showLogo)
        {
            drawLogo();
            showLogo = true;
            showRpmWarning = false;
            showRpmLimit = false;
        }

        datafields[DATAFLD_RPM].setValue((int)rpm);
        datafields[DATAFLD_CLT].setValue(megaSquirt.getClt(tempScale));
        datafields[DATAFLD_MAT].setValue(megaSquirt.getMat(tempScale));
        datafields[DATAFLD_MAP].setValue(megaSquirt.getMap());
        datafields[DATAFLD_TPS].setValue(megaSquirt.getTps());
        datafields[DATAFLD_AFR].setValue(megaSquirt.getAfr());
        datafields[DATAFLD_TAF].setValue(megaSquirt.getTaf());
        datafields[DATAFLD_ADV].setValue(megaSquirt.getAdv());
        datafields[DATAFLD_PWP].setValue(megaSquirt.getPwp());

        if (megaSquirt.getEngine() & MS_ENGINE_READY)
        {
            if (megaSquirt.getEngine() & MS_ENGINE_CRANKING)
                engIndicator.setState(2, BLACK, RED, F("Cranking"));
            else
                engIndicator.setState(3, BLACK, GREEN, F("Running"));
        }
        else
            engIndicator.setState(1, DRKGRAY, LTGRAY, F("Not running"));
    }
    else
    {
        byte c;
        for (c=1; c<10;c++)
        {
            datafields[c].setUnknown();
        }
        engIndicator.setState(0, DRKGRAY, LTGRAY, F("No ECU?"));
        if (!showLogo)
        {
            drawLogo();
            showLogo = true;
            showRpmWarning = false;
            showRpmLimit = false;
        }
    }
}

void doSetup()
{
	D(debugSerial->println(F("doSetup"));)
    tft.fillScreen(LTGRAY);

    drawLogo();


    rpmWarnDn.create(&tft,190,32, 25, 20, GFXARROW_DOWN, LTGRAY,BLACK);
    rpmWarnDn.draw();

    rpmWarnFld.create(&tft,225,35,2,1,LTGRAY,BLACK);
    rpmWarnFld.drawLabel(10,35,2,F("Warning RPM:"));
    rpmWarnFld.setValue(rpmWarn*100);

    rpmWarnUp.create(&tft,280,32, 25, 20, GFXARROW_UP, LTGRAY,BLACK);
    rpmWarnUp.draw();

    rpmLimitDn.create(&tft,190,62, 25, 20, GFXARROW_DOWN, LTGRAY,BLACK);
    rpmLimitDn.draw();

    rpmLimitFld.create(&tft,225,65,2,1,LTGRAY,BLACK);
    rpmLimitFld.drawLabel(10,65,2,F("Limit RPM:"));
    rpmLimitFld.setValue(rpmLimit*100);

    rpmLimitUp.create(&tft,280,62, 25, 20, GFXARROW_UP, LTGRAY,BLACK);
    rpmLimitUp.draw();

    tempScaleLabel.create(&tft,LTGRAY,BLACK);
    tempScaleLabel.drawLabel(10,95,2,F("Temp Scale:"));

    celsiusButton.create(&tft, 190, 90, 40, 24, 2, F("C"), LTGRAY, BLACK);
    fahrenheitButton.create(&tft, 260, 90, 40, 24, 2, F("F"), LTGRAY, BLACK);

    if (tempScale == MS_CELSIUS)
        celsiusButton.swapColours();
    else
        fahrenheitButton.swapColours();

    celsiusButton.draw();
    fahrenheitButton.draw();

    exitButton.create(&tft, 120, 212, 80, 24, 2, F("EXIT"), LTGRAY, BLACK);
    exitButton.draw();

    bool exit = false;
    while (!exit)
    {
        exit = setupLoop();
    }
}

void loop()
{
    if (millis() > time) {
        time = millis()+100;
    	dataCaptureLoop();
    }

    Point p = ts.getPoint();
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    if (setupButton.isPressed(p))
    {
    	doSetup();
    }

    if (gps.newNMEAreceived()) {

       if (gps.parse(gps.lastNMEA())) {
    	   gps.latitude_fixed;
    	   gps.longitude_fixed;
    	   gps.speed;
    	   gps.angle;
    	   gps.altitude;
       }
    }
}

// Interrupt is called once a millisecond, looks for any new GPS data, and stores it
SIGNAL(TIMER0_COMPA_vect) {
 gps.read();
}
