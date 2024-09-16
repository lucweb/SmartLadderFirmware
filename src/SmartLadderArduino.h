#ifndef SmartLadderArduino_h
#define SmartLadderArduino_h

#include <Arduino.h>

#include "Generic.h"

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
  void setConfigProgram();
  void setConfigPort();
  void startTypeRequest(char e);
  void eR();
  void receiver(char e);
  void emitDigitalRead();
  void setStatusResource();
  void emit(String value);
};

#endif