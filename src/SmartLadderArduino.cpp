#include <SmartLadderArduino.h>
#include "proccess/arduino/adc.h"
#include "communication/communication.h"
#include "proccess/bit.h"
#include "proccess/count.h"
#include "proccess/equal.h"
#include "proccess/force.h"
#include "proccess/io.h"
#include "proccess/proccess.h"
#include "proccess/ResourceStatus.h"
#include "proccess/time.h"
#include "proccess/arduino/dac.h"
#include "proccess/memory/arduino.h"
#include "proccess/settings/arduino.h"
#include "proccess/stepper.h"

SmartLadderArduino::smartLadder()
{
}

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