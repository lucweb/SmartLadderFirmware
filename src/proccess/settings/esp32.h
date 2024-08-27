#include "SmartLadderEsp32.h"

void SmartLadderEsp32::declare(int port, char tipePort)
{
  if (tipePort == 'i')
    pinMode(port, INPUT);
  else if (tipePort == 'o')
    pinMode(port, OUTPUT);
  delay(0.5);
}

void SmartLadderEsp32::declareDAC(String d)
{
  String p = "";
  String c = "";
  String f = "";
  String r = "";
  int i = 0;
  for (int x = 0; x < d.length(); x++)
  {
    if (d[x] == ':')
      i++;
    else if (i == 0)
      p += d[x];
    else if (i == 1)
      c += d[x];
    else if (i == 2)
      f += d[x];
    else if (i == 3)
      r += d[x];
  }

  ledcAttach(p.toInt(), f.toInt(), r.toInt());
}

void SmartLadderEsp32::typePort()
{
  String p = "";
  for (int x = 0; x < R_R.length(); x++)
  {
    if (
        R_R[x] != 'i' &&
        R_R[x] != 'o' &&
        R_R[x] != 'a' &&
        R_R[x] != 'd' &&
        R_R[x] != 'w')
      p += R_R[x];
    else if (R_R[x] == 'a')
    {
      p = "";
    }
    else if (R_R[x] == 'd')
    {
      declareDAC(p);
      p = "";
    }
    else if (R_R[x] == 'w')
    {

#if USE_I2C
      declareWIRE(p.c_str());
#endif
      p = "";
    }
    else
    {
      declare(p.toInt(), R_R[x]);
      p = "";
    }
  }
  upForce();
}