#ifndef tempsensor_h
#define tempsensor_h

#include "Arduino.h"

#define REF_PIN 12


class TempSensor {

 public:
 
  TempSensor(void);
  
    float getTempFloat();
    int getTempInt100();
    int getTempInt();
  
 private:

  void OneWireReset (int Pin);

  void OneWireOutByte (int Pin, byte d);

  byte OneWireInByte (int Pin);
};

#endif
