#ifndef SmartLadderArduino_h
#define SmartLadderArduino_h

#include <Arduino.h>

#define SMARTLADDER_ARDUINO_VERSION F("v1.0.0")

class SmartLadderArduino
{

public:
  smartLadder();

  void setup();
  void loop();
  
private:
  String P_P, _F, R_R, _TD;
  int CT_CT[10], _ST;
  long int TL_TL[10];
  byte B_B[20], _TPR, C_CCCTU[10], C_CCCTD[10];

  void declare(int port, char tipePort);
  void typePort();
  int pEEPROM();
  void loadProgram();
  void loadPort();
  void setEEPROMProgram();
  void setEEPROMPort();
  bool isForce(int i, char tipo = 0);
  void setForce(String m);
  void upForce();
  int getCount(int i);
  bool setCount(int i, int t);
  void resetCount(int i);
  int setCountDown(int i, int t);
  int getTemp(int x);
  void loopTemp(int t, int qt);
  void resetTemp(int t);
  bool validaTemp(int t, int qt);
  int atvIADCPort(int resource);
  int atvIDACPort(int resource, String value);
  bool atvIOPort(int resource, int tCond, int s);
  bool atvIOBit(String port, int tCond, int s);
  bool atvTime(String port, int tCond, String prop, int s);
  bool atvCount(String port, int tCond, String prop, int s);
  bool atvCountDown(String port, int tCond, String prop, int s);
  bool atv(String tipoCond, String port, String prop, int s);
  int checkStatus(String port);
  bool setEqual(String tipoCond, String sA, String sB);
  void start();
  void startTypeRequest(char e);
  void eR();
  void receiver(char e);
  void emitDigitalRead();
  void setStatusResource();
  bool stepper(int resource, String values);
  bool basicStepper(int pin, int v, int totalSteps, int count);
  bool asyncStepper(int pin, int v, int totalSteps, int count);
};

#endif