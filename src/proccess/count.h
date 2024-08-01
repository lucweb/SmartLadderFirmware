#include "Generic.h"

int Generic::getCount(int i)
{
  return abs(CT_CT[i]);
}

bool Generic::setCount(int i, int t)
{
  if (CT_CT[i] >= 0 && CT_CT[i] < t && !C_CCCTU[i])
  {
    C_CCCTU[i] = 1;
    CT_CT[i]++;
    if (CT_CT[i] == t)
      CT_CT[i] = -CT_CT[i];
  }
  return getCount(i) == t;
}

void Generic::resetCount(int i)
{
  CT_CT[i] = 0;
}

int Generic::setCountDown(int i, int t)
{
  if (getCount(i) != 0 && !C_CCCTD[i])
  {
    C_CCCTD[i] = 1;
    CT_CT[i] = getCount(i) - t;
    if (CT_CT[i] < 1)
      CT_CT[i] = 0;
  }
  return getCount(i) == 0;
}

bool Generic::atvCountDown(String port, int tCond, String prop, int s)
{
  port.remove(0, 1);
  int t = port.toInt();

  bool r = (s) ? setCountDown(t, prop.toInt()) : 0;
  C_CCCTD[t] = (!s) ? 0 : 1;
  return r;
}

bool Generic::atvCount(String port, int tCond, String prop, int s)
{
  port.remove(0, 1);
  int t = port.toInt();
  if (tCond == 9)
  {
    if (s)
      resetCount(t);
  }
  switch (tCond)
  {
  case 1:
    return CT_CT[t] < 0;
    break;
  case 2:
    return CT_CT[t] >= 0;
    break;
  case 7:
    bool r = (s) ? setCount(t, prop.toInt()) : 0;
    C_CCCTU[t] = (!s) ? 0 : 1;
    return r;
    break;
  }
  return s;
}