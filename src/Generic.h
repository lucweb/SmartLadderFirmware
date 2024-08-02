#ifndef GENERIC_H
#define GENERIC_H

#include <Arduino.h>

class Generic
{
public:
#if defined(ESP32)
  String SSID_, PASW_, CODE_, TEMP_;
  bool AP_DISCONNECT = false;
  byte isForm = false;

  String R_R, P_P, F_F, TD_TD, DT_SV;

  int T_AD_V[18], CT_CT[30];
  long int TL_TL[30], A_M_S[18];
  byte B_B[30], _ST, _TPR, C_CCCTU[30], C_CCCTD[30], DT_SV_S[20];

  int atvIADCPort(String resource);
#else
  String P_P, F_F, R_R, TD_TD;
  long int TL_TL[10], CT_CT[10];
  byte B_B[20], _ST, _TPR, C_CCCTU[10], C_CCCTD[10];
  int atvIADCPort(int resource);
#endif

  bool stepper(int resource, String values);
  bool basicStepper(int pin, int v, int totalSteps, int count);
  bool asyncStepper(int pin, int v, int totalSteps, int count);
  bool atvIOBit(const char* port, int tCond, int s);
  bool isForce(int i, char tipo = '0');
  void setForce(String m);
  void upForce();
  void start();
  bool atv(String tipoCond, String port, String prop, int s);
  int checkStatus(const char* port);
  bool setEqual(const char* tipoCond, const char* sA, const char* sB);
  int getCount(int i);
  bool setCount(int i, int t);
  void resetCount(int i);
  int setCountDown(int i, int t);
  int getTemp(int x);
  void loopTemp(int t, int qt);
  void resetTemp(int t);
  bool validaTemp(int t, int qt);
  bool atvTime(const char* port, int tCond, const char* prop, int s);
  bool atvCount(const char* port, int tCond, const char* prop, int s);
  bool atvCountDown(const char* port, int tCond, const char* prop, int s);
  bool atvIOPort(int resource, int tCond, int s);
  int atvIDACPort(int resource, const char* value);

private:
};

#endif
