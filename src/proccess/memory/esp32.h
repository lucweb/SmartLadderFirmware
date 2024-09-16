#include "SmartLadderEsp32.h"
#include <FS.h>
#include "SPIFFS.h"

void SmartLadderEsp32::loadConfig()
{
  TEMP_ = "";
  SPIFFS.begin(true);
  File file = SPIFFS.open("/conf.txt", "r");
  if (file)
  {
    while (file.available())
      TEMP_ += (char)file.read();
    file.close();
  }
  SPIFFS.end();
#if USE_ETH8720 || USE_WIFI
  if (TEMP_)
    dataConfig();

  setupMqttConnect();
#endif
}

void SmartLadderEsp32::setConfig()
{
  SPIFFS.begin();
  File wFile = SPIFFS.open("/conf.txt", "w");
  wFile.println(TEMP_.c_str());
  wFile.close();
  SPIFFS.end();
  SPIFFS.end();
}

void SmartLadderEsp32::setConfigProgram()
{
  SPIFFS.begin();
  File wFile = SPIFFS.open("/program.txt", "w");
  wFile.println(P_P.c_str());
  wFile.close();
  SPIFFS.end();
}

void SmartLadderEsp32::setConfigPort()
{
  SPIFFS.begin();
  File wFile = SPIFFS.open("/port.txt", "w");
  wFile.println(R_R.c_str());
  wFile.close();
  SPIFFS.end();
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
}