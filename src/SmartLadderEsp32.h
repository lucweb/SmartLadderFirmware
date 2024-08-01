#ifndef SmartLadderEsp32_h
#define SmartLadderEsp32_h

#include <Arduino.h>
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

  String getScanNetworks();
  String listSSID();
  String getDNS();
  void setDNS();
  void wifiInit();
  void emitDtSv(String v);
  void mqttCallback(char *topic, byte *payload, unsigned int length);
  void rMQTT();
  void connectClientWifi();
  void reloadWifi();
  void setupConnect();
  void hRoot();
  void getInfo();
  void hForm();
  void TempStatusWifi();
  void vStatusAll();
  void conectLoop();
  void checkStatusWifi();

  void connectEeprom();
  void setEEPROM();
  boolean compareEeprom(String s, String p, String c);
  void setEEPROMProgram();
  void setEEPROMPort();
  void loadProgram();
  void loadPort();
};

#endif
