#ifndef SmartLadderArduino_h
#define SmartLadderArduino_h

#include <Arduino.h>

#include "Generic.h" 

#define VERSION F("v1.0.0")

class SmartLadderArduino : public Generic
{

public:
  void setup();
  void loop();
    
private:
  unsigned long measureCycleTime(void (*func)());
  void declare(int port, char tipePort);
  void typePort();
  int pEEPROM();
  void loadProgram();
  void loadPort();
  void setEEPROMProgram();
  void setEEPROMPort();
  void startTypeRequest(char e);
  void eR();
  void receiver(char e);
  void emitDigitalRead();
  void setStatusResource();
  void emit(String value);
};

#endif