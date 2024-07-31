#if defined(ESP32)
#include "SmartLadderEsp32.h"
#elif defined(ESP8266)
#include "SmartLadderEsp8266.h"
#else
#include "SmartLadderArduino.h"
#endif

bool SmartLadderArduino::stepper(int resource, String values)
{
  int p = 0;
  String v1 = "";
  String v2 = "";
  String v3 = "";
  String v4 = "";
  for (int i = 0; i < values.length(); i++)
  {
    if (values[i] == '/')
      p++;
    else
    {
      if (p == 0)
        v1 += values[i];
      else if (p == 1)
        v2 += values[i];
      else if (p == 2)
        v3 += values[i];
      else if (p == 3)
        v4 += values[i];
    }
  }

  int speed = v1.toInt();
  int totalSteps = v2.toInt();
  int c = v3.toInt();

  if(v4 == '1')
    return asyncStepper(resource, speed, totalSteps, c);

  return basicStepper(resource, speed, totalSteps, c);
}

bool SmartLadderArduino::basicStepper(int pin, int v, int totalSteps, int c)
{
  if (CT_CT[c] >= totalSteps)
    return true;

  digitalWrite(pin, 1);
  delayMicroseconds(v);
  digitalWrite(pin, 0);
  CT_CT[c]++;

  return false;
}

bool SmartLadderArduino::asyncStepper(int pin, int v, int totalSteps, int c)
{
  if (CT_CT[c] >= totalSteps)
    return true;
  for (int x = 0; x < totalSteps; x++)
  {
    digitalWrite(pin, 1);
    delayMicroseconds(v);
    digitalWrite(pin, 0);
    CT_CT[c]++;
  }
  return false;
}
