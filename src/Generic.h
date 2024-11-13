#ifndef GENERIC_H
#define GENERIC_H

#include <Arduino.h>
#include "config.h"

#if USE_I2C
#include <Wire.h>
#endif

class Generic
{
public:
#if defined(ESP32)
  byte ORIGIN_ = 0;
  String SSID_, PASW_, CODE_, TEMP_;
  byte isForm = false;

  String R_R, P_P, F_F, TD_TD, DT_SV;

  uint16_t A_D[50];
  float CT_CT[50];
  long int TL_TL[50], A_M_S[50];
  byte B_B[50], _ST, _TPR, C_CCCTU[50], C_CCCTD[50], DT_SV_S[50];

  int atvIADCPort(String resource);
#else
  String P_P, F_F, R_R, TD_TD;
  long int TL_TL[20];
  float CT_CT[20];
  byte B_B[20], _ST, _TPR, C_CCCTU[20], C_CCCTD[20];
  uint16_t A_D[30];
  int atvIADCPort(int resource);
#endif

  bool stepper(int resource, String values);
  bool basicStepper(int pin, int v, unsigned long totalSteps, int count);
  bool asyncStepper(int pin, int v, unsigned long totalSteps, int count);
  bool atvIOBit(const char *port, int tCond, int s);
  bool isForce(int i, char tipo = '0');
  void setForce(String m);
  void upForce();
  void start();
  bool atv(String tipoCond, String port, String prop, int s);
  float checkStatus(const char *port);
  bool setEqual(const char *tipoCond, const char *sA, const char *sB);
  void atvSet(const char *port, const char *value);
  float getCount(int i);
  bool setCount(int i, int t);
  void resetCount(int i);
  int setCountDown(int i, int t);
  int getTemp(int x);
  void loopTemp(int t, unsigned long qt);
  void resetTemp(int t);
  bool validaTemp(int t, int qt);
  bool atvTime(const char *port, int tCond, const char *prop, int s);
  bool atvCount(const char *port, int tCond, const char *prop, int s);
  bool atvCountDown(const char *port, const char *prop, int s);
  bool atvIOPort(int resource, int tCond, int s);
  int atvIDACPort(int resource, const char *value);
  bool atvPUpDown(int tCond, const char *port, const char *prop);
  bool toggleUp(int i, bool st);
  bool toggleSetUp(int i, bool st);
  bool toggleDown(int i, bool st);
  bool toggleSetDown(int i, bool st);
  void setCalc(const char *o, const char *prop);
  void timerConfigCallback(int pin, int v, int c, unsigned long totalSteps);
#ifndef ESP32
  void rstTimer();
  void stpTimer();
#endif
#if USE_I2C

  bool atvIOPortI2c(int resource, int tCond, const char *prop, int s);
  void scanI2C(TwoWire &i2c);
  int getBytes(char *bits, int *buffer);
  void setConfigI2c(TwoWire &i2c, int address, char *dados, char *bits);
  uint32_t getPinStateReal(TwoWire &i2c, int address, char *bits, int anl);
  uint16_t getAnalogicValueI2c(TwoWire &i2c, int address, char *bits, int b1, int b2, int anl);
  bool checkPinStateI2c(TwoWire &i2c, int address, int pinNumber, char *bits);
  void updatePinI2c(TwoWire &i2c, int address, int pin, bool state, char *bits);
  void updatePinI2cAll(TwoWire &i2c, int address, int pin, bool state, char *bits, int pin2, int v);

#if defined(ESP32)
  void startWire(TwoWire &i2c, int sda, int scl);
  void declareWIRE(const char *prop);
  TwoWire I2C_1 = TwoWire(0);
  TwoWire I2C_2 = TwoWire(1);
#else
  TwoWire &I2C_1 = Wire;
  TwoWire I2C_2;
#endif

#endif

private:
};
#endif
