#include "Generic.h"

bool Generic::atvIOPort(int resource, int tCond, int s)
{
  switch (tCond)
  {
  case 1:
    if(!s) return s;
    return digitalRead(resource);
    break;
  case 2:
    if(!s) return s;
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