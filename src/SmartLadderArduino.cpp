#if !defined(ESP32)
#include <SmartLadderArduino.h>
#include "communication/arduino.h"
#include "proccess/memory/arduino.h"
#include "proccess/settings/arduino.h"
#include "Generic.h"

void SmartLadderArduino::setup()
{
  Serial.begin(115200);
  delay(10);
  loadPort();
  typePort();
  loadProgram();
}

void SmartLadderArduino::loop()
{
  while (Serial.available())
    receiver(Serial.read());

  if (_TPR == 0)
    start();

  if (_TPR == 0 && _ST > 0)
    setStatusResource();
}
#endif