#if defined(ESP32)
    #include "SmartLadderEsp32.h"
#elif defined(ESP8266)
    #include "SmartLadderEsp8266.h"
#else
    #include "SmartLadderArduino.h"
#endif

bool SmartLadderArduino::atvIOPort(int resource, int tCond, int s)
{
  switch (tCond)
  {
  case 1:
    return digitalRead(resource);
    break;
  case 2:
    return !digitalRead(resource);
    break;
  case 3:
    if (!isForce(resource))
      digitalWrite(resource, s);
    break;
  case 4:
    if (!isForce(resource))
    {
      if (s)
        digitalWrite(resource, 1);
    }
    break;
  case 5:
    if (!isForce(resource))
    {
      if (s)
        digitalWrite(resource, 0);
    }
    break;
  }
  return s;
}