#include "TempSensor.h"


// Constructor 
TempSensor::TempSensor(void) {
  digitalWrite(REF_PIN, LOW);
  pinMode(REF_PIN, INPUT);      // sets the digital pin as input (logic 1)
}

void TempSensor::OneWireReset (int Pin) // reset.  Should improve to act as a presence pulse
{
  digitalWrite (Pin, LOW);
  pinMode (Pin, OUTPUT);        // bring low for 500 us
  delayMicroseconds (500);
  pinMode (Pin, INPUT);
  delayMicroseconds (500);
}

void TempSensor::OneWireOutByte (int Pin, byte d) // output byte d (least sig bit first).
{
  byte n;

  for (n=8; n!=0; n--)
  {
    if ((d & 0x01) == 1)  // test least sig bit
    {
      digitalWrite (Pin, LOW);
      pinMode (Pin, OUTPUT);
      delayMicroseconds (5);
      pinMode (Pin, INPUT);
      delayMicroseconds (60);
    }
    else
    {
      digitalWrite (Pin, LOW);
      pinMode (Pin, OUTPUT);
      delayMicroseconds (60);
      pinMode (Pin, INPUT);
    }

    d = d>>1; // now the next bit is in the least sig bit position.
  }
}


byte TempSensor::OneWireInByte (int Pin) // read byte, least sig byte first
{
  byte d =0, n, b;

  for (n=0; n<8; n++)
  {
    digitalWrite (Pin, LOW);
    pinMode (Pin, OUTPUT);
    delayMicroseconds (5);
    pinMode (Pin, INPUT);
    delayMicroseconds (5);
    b = digitalRead (Pin);
    delayMicroseconds (50);
    d = (d >> 1) | (b<<7); // shift d to right and insert b in most sig bit position
  }
  return (d);
}


int TempSensor::getTempInt ()
{
    return getTempInt100() / 100;
    
}

float TempSensor::getTempFloat ()
{
    return getTempInt100() / 100.0;
}

int TempSensor::getTempInt100 ()
{
  int HighByte, LowByte, TReading, Tc_100, sign;

  OneWireReset (REF_PIN);
  OneWireOutByte (REF_PIN, 0xcc);
  OneWireOutByte (REF_PIN, 0x44); // perform temperature conversion, strong pullup for one sec

  OneWireReset (REF_PIN);
  OneWireOutByte (REF_PIN, 0xcc);
  OneWireOutByte (REF_PIN, 0xbe);

  LowByte = OneWireInByte (REF_PIN);
  HighByte = OneWireInByte (REF_PIN);
  TReading = (HighByte << 8) + LowByte;
  sign = TReading & 0x8000;  // test most sig bit
  if (sign) // negative
  {
    TReading = (TReading ^ 0xffff) + 1; // 2's comp
  }
  Tc_100 = (6 * TReading) + TReading / 4;    // multiply by (100 * 0.0625) or 6.25
  if (sign) Tc_100 *=-1;
  
  return Tc_100;

//  int whole = Tc_100 / 100;  // separate off the whole and fractional portions
//  int fract = Tc_100 % 100;
//
//  if (sign) {
//    temp[0] = '-';
//  } else {
//    temp[0] = '+';
//  }
//
//  if (whole/100 == 0) {
//    temp[1] = ' ';
//  } else {
//    temp[1] = whole/100+'0';
//  }
//
//  temp[2] = (whole-(whole/100)*100)/10 +'0' ;
//  temp[3] = whole-(whole/10)*10 +'0';
//  temp[4] = '.';
//  temp[5] = fract/10 +'0';
//  temp[6] = '\0';
//  //  temp[6] = fract-(fract/10)*10 +'0';
//  //  temp[7] = '\0';
}
