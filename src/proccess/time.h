#include "Generic.h"

int Generic::getTemp(int x)
{
  return TL_TL[x] > 0 ? (millis() - TL_TL[x]) : (TL_TL[x] < 0 ? abs(TL_TL[x]) : 0);
}

void Generic::loopTemp(int t, int qt)
{
  if (millis() - TL_TL[t] >= qt)
    TL_TL[t] = -qt;
}

void Generic::resetTemp(int t)
{
  TL_TL[t] = 0;
}

bool Generic::validaTemp(int t, int qt)
{
  if (TL_TL[t] >= 0)
  {
    if (TL_TL[t] == 0)
      TL_TL[t] = millis();
    loopTemp(t, qt);
  }
  return (TL_TL[t] < 0);
}

bool Generic::atvTime(const char* port, int tCond, const char* prop, int s)
{
    int t = atoi(port + 1); 
    int value = atoi(prop); 

    switch (tCond)
    {
    case 1:
        return TL_TL[t] < 0;
    case 2:
        return TL_TL[t] >= 0;
    case 6:
        if (s)
            return validaTemp(t, value);
        else if (TL_TL[t] > 0)
        {
            loopTemp(t, value);
        }
        return s;
    case 9:
        if (s)
            resetTemp(t);
        break;
    }
    return s;
}
