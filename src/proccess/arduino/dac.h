#include "SmartLadderArduino.h"

int SmartLadderArduino::atvIDACPort(int resource, String value)
{
  int p = 0;
  String v1 = "";
  String v2 = "";
  String v3 = "";
  for (int i = 0; i < value.length(); i++)
  {
    if (value[i] == '/')
      p++;
    else
    {
      if (p == 0)
        v1 += value[i];
      if (p == 1)
        v2 += value[i];
      if (p == 2)
        v3 += value[i];
    }
  }

  int tempValue = 0;
  if (v1[0] == 'A')
  {
    v1.remove(0, 1);
    tempValue = atvIADCPort(v1.toInt());
  }
  else if (v1[0] == 'c')
  {
    v1.remove(0, 1);
    tempValue = getCount(v1.toInt());
  }
  else
  {
    tempValue = v1.toInt();
  }

  int result = map(tempValue, v2.toInt(), v3.toInt(), 0, 255);
  if (result < 0)
    result = 0;

  analogWrite(resource, result);
  return result;
}