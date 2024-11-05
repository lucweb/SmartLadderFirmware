#include "Generic.h"

#if defined(ESP32)
int Generic::atvIADCPort(String resource)
{
  String n = "";
  String p = "";
  bool init = false;
  for (size_t x = 0; x < resource.length(); x++)
  {
    if (resource[x] == '-')
      init = true;
    else if (init)
      n += resource[x];
    else
      p += resource[x];
  }

  int i = n.toInt();
  if ((millis() - A_M_S[i]) >= 30)
  {
    if (!A_D[i])
      A_D[i] = 0;

    int value = analogRead(p.toInt());
    const float fs = 0.05;
    A_D[i] = value * fs + A_D[i] * (1 - fs);
    A_M_S[i] = 0;
  }
  if (A_M_S[i] == 0)
    A_M_S[i] = millis();
  return A_D[i];
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