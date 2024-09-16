#include "SmartLadderArduino.h"

void SmartLadderArduino::declare(int port, char tipePort)
{
  if (tipePort == 'i')
    pinMode(port, INPUT);
  else if (tipePort == 'o' || tipePort == 'd')
    pinMode(port, OUTPUT);
  delay(0.5);
}

void SmartLadderArduino::typePort()
{
  String p = "";
  for (int x = 0; x < R_R.length(); x++)
  {
    if (R_R[x] != 'i' && R_R[x] != 'o' && R_R[x] != 'd' && R_R[x] != 'a' && R_R[x] != 'w')
      p += R_R[x];
    else if (R_R[x] == 'w')
    {
#if USE_I2C
      declareWIRE(p.c_str());
#endif
      p = "";
    }
    else
    {
      if (R_R[x] != 'a')
        declare(p.toInt(), R_R[x]);
      p = "";
    }
  }

  upForce();
}