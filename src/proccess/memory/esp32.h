#include "SmartLadderEsp32.h"
#include <EEPROM.h>
#include <FS.h>
#include "SPIFFS.h"

void SmartLadderEsp32::connectEeprom()
{
  EEPROM.begin(300);

  int rootSize = 8 + 4;
  int sSize = (int)EEPROM.read(9) + rootSize;
  int pSize = (int)EEPROM.read(10) + sSize;
  int cSize = (int)EEPROM.read(11) + pSize;
  SSID_ = "";
  PASW_ = "";
  CODE_ = "";

  for (int i = rootSize; i < sSize; i++)
    SSID_.concat(char(EEPROM.read(i)));

  for (int j = sSize; j < pSize; j++)
    PASW_.concat(char(EEPROM.read(j)));

  for (int x = pSize; x < cSize; x++)
    CODE_.concat(char(EEPROM.read(x)));

  EEPROM.end();

  Serial.println(F("Access:"));
  Serial.println(SSID_);
  Serial.println(PASW_);
  Serial.println(CODE_);

  WiFi.begin(SSID_.c_str(), PASW_.c_str());
}

void SmartLadderEsp32::setEEPROM()
{
  EEPROM.begin(300);
  if (!compareEeprom(SSID_, PASW_, CODE_))
  {
    int ps = SSID_.length() + 12;
    int pp = PASW_.length() + ps;
    int pc = CODE_.length() + pp;

    EEPROM.write(9, SSID_.length());
    EEPROM.write(10, PASW_.length());
    EEPROM.write(11, CODE_.length());
    for (int i = 12; i < ps; i++)
      EEPROM.write(i, SSID_.charAt(i - 12));
    for (int i = ps; i < pp; i++)
      EEPROM.write(i, PASW_.charAt(i - ps));
    for (int i = pp; i < pc; i++)
      EEPROM.write(i, CODE_.charAt(i - pp));
    EEPROM.commit();
  }
  EEPROM.end();
}

boolean SmartLadderEsp32::compareEeprom(String s, String p, String c)
{
  int ps = int(EEPROM.read(9)) + 12;
  int pp = int(EEPROM.read(10)) + ps;
  int pc = int(EEPROM.read(11)) + pc;
  String id = "";
  String pass = "";
  String cd = "";
  for (int i = 12; i < ps; i++)
    id += char(EEPROM.read(i));
  for (int j = ps; j < pp; j++)
    pass += char(EEPROM.read(j));
  for (int x = pp; x < pc; x++)
    cd += char(EEPROM.read(x));
  return (id.equals(s) && pass.equals(p) && cd.equals(c));
}


void SmartLadderEsp32::setEEPROMProgram()
{
  SPIFFS.begin();
  File wFile = SPIFFS.open("/program.txt", "w");
  wFile.println(P_P.c_str());
  wFile.close();
  SPIFFS.end();
}

void SmartLadderEsp32::setEEPROMPort()
{
  SPIFFS.begin();
  File wFile = SPIFFS.open("/port.txt", "w");
  wFile.println(R_R.c_str());
  wFile.close();
  SPIFFS.end();
  typePort();
}

void SmartLadderEsp32::loadProgram()
{
  P_P = "";
  SPIFFS.begin(true);
  File file = SPIFFS.open("/program.txt", "r");
  if (file)
  {
    while (file.available())
      P_P += (char)file.read();
    file.close();
  }
  SPIFFS.end();
  Serial.println(P_P);
  emit(P_P);
}

void SmartLadderEsp32::loadPort()
{
  R_R = "";

  SPIFFS.begin();
  File file = SPIFFS.open("/port.txt", "r");
  if (file)
  {
    while (file.available())
      R_R += (char)file.read();
    file.close();
  }
  SPIFFS.end();
  typePort();
  emit(R_R);
}