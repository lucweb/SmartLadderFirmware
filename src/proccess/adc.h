#include "Generic.h"

#if defined(ESP32)
int Generic::atvIADCPort(String resource)
{
  String n = "";
  String p = "";
  bool init = false;
  for (int x = 0; x < resource.length(); x++)
  {
    if (resource[x] == '-')
      init = true;
    else if (init)
      n += resource[x];
    else
      p += resource[x];
  }
  
  int i = n.toInt();
  if ((millis() - A_M_S[i]) >= 50)
  {
    T_AD_V[i] = analogRead(p.toInt());
    A_M_S[i] = 0;
  }
  if (A_M_S[i] == 0)
    A_M_S[i] = millis();
  return T_AD_V[i];
}
#elif defined(ESP8266)
int Generic::atvIADCPort(int resource)
{
  return analogRead(resource);
}
#else
int Generic::atvIADCPort(int resource)
{
  return analogRead(resource);
}
#endif