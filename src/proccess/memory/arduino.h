#include "SmartLadderArduino.h"
#include <EEPROM.h>

void SmartLadderArduino::loadPort()
{
  R_R = "";
  for (int i = 0; i < 1024; i++)
  {
    char d = (char)EEPROM.read(i);
    if (d == '#' || d == '\0' || (d != '0' && !d))
      return;
    R_R += d;
  }
  delay(5);
  typePort();
}

int SmartLadderArduino::pEEPROM()
{
  return R_R.length() + 2;
}

void SmartLadderArduino::loadProgram()
{
  P_P = "";
  int q = pEEPROM();
  for (int i = q; i < 1024; i++)
  {
    char d = (char)EEPROM.read(i);
    if (d == '#' || d == '\0')
      return;
    P_P += d;
  }
  delay(5);
}

void SmartLadderArduino::setEEPROMProgram()
{
    int q = pEEPROM();
    int c = P_P.length() + q;
    for (int i = q; i < c; i++)
        EEPROM.write(i, P_P[i - q]);
    EEPROM.write(c, '#');
}

void SmartLadderArduino::setEEPROMPort()
{
    int c = R_R.length();
    for (int i = 0; i < c; i++)
        EEPROM.write(i, R_R[i]);
    EEPROM.write(c, '#');
    typePort();
}
