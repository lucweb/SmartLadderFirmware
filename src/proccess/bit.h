#if defined(ESP32)
    #include "SmartLadderEsp32.h"
#elif defined(ESP8266)
    #include "SmartLadderEsp8266.h"
#else
    #include "SmartLadderArduino.h"
#endif

bool SmartLadderArduino::atvIOBit(String port, int tCond, int s)
{
  port.remove(0, 1);
  int b = port.toInt();
  switch (tCond)
  {
  case 1:
    return B_B[b];
    break;
  case 2:
    return !B_B[b];
    break;
  case 3:
    if (!isForce(b, 'b'))
    {
      B_B[b] = s;
    }
    break;
  case 4:
    if (!isForce(b, 'b'))
    {
      if (s)
        B_B[b] = 1;
    }
    break;
  case 5:
    if (!isForce(b, 'b'))
    {
      if (s)
      {
        B_B[b] = 0;
      }
    }
    break;
  }
  return s;
}
