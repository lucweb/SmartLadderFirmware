#ifndef SmartLadderEsp32_h
#define SmartLadderEsp32_h

#include <Arduino.h>

#include "config.h" 

#include "Generic.h"

#define VERSION F("v1.0.0")

class SmartLadderEsp32 : public Generic
{

public:
  void setup();
  void loop();

private:
  void Task1code(void *pvParameters);
  void Task2code(void *pvParameters);
  void declare(int port, char tipePort);
  void declareDAC(String d);
  void typePort();

  void startTypeRequest(char e);
  void eR();
  void receiver(char e);
  void emitDigitalRead();
  void setStatusResource();
  void emit(String value);
  void ctrlDtSv();
  String getVDS(String port);

#if USE_ETH8720
  void connectEth();
#endif

#if USE_WIFI
  void getScanNetworks();
  void checkStatusWifi();
#endif

#if USE_ETH8720 || USE_WIFI

  void emitDtSv(String v);
  void mqttCallback(char *topic, byte *payload, unsigned int length);
  void rMQTT();
  void setupMqttConnect();
  void dataConfig(bool save);

  void TempStatusConnect();
  void vStatusAll();
  void connectLoop();
#endif

  void setConfig();
  void setConfigProgram();
  void setConfigPort();
  void loadProgram();
  void loadPort();
  void loadConfig();
};
#endif