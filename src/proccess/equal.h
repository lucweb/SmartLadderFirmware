#include "Generic.h"

int Generic::checkStatus(String port)
{
  int resource = port.toInt();
  if (resource)
    digitalRead(resource);
  else if (port[0] == 'b')
  {
    port.remove(0, 1);
    int t = port.toInt();
    return B_B[t];
  }
  else if (port[0] == 'T')
  {
    port.remove(0, 1);
    int t = port.toInt();
    return getTemp(t);
  }
  else if (port[0] == 'c')
  {
    port.remove(0, 1);
    int t = port.toInt();
    return getCount(t);
  }
  return 0;
}

bool Generic::setEqual(String tipoCond, String sA, String sB)
{
    int iA = sA.indexOf('/');
    int iB = sB.indexOf('/');
    int a = 0;
    int b = 0;
    if (iA >= 0)
    {
        sA.remove(0, 1);
        a = checkStatus(sA);
    }
    else
        a = sA.toInt();

    if (iB >= 0)
    {
        sB.remove(0, 1);
        b = checkStatus(sB);
    }
    else
        b = sB.toInt();

    switch (tipoCond[0])
    {
    case '=':
        return (a == b);
        break;
    case '!':
        return (a != b);
        break;
    case '-':
        return (a < b);
        break;
    case '+':
        return (a > b);
        break;
    }
    return false;
}