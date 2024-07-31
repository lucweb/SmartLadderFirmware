#include "SmartLadderArduino.h"

int SmartLadderArduino::atvIADCPort(int resource)
{
  return analogRead(resource);
}