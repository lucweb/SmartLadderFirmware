#include "Generic.h"

#if defined(ESP32)
int Generic::atvIDACPort(int resource, String value)
{
  int p = 0;
  String v1 = "";
  String v2 = "";
  String v3 = "";
  String v4 = "";
  String v5 = "";
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
      if (p == 3)
        v4 += value[i];
      if (p == 4)
        v5 += value[i];
    }
  }

  int tempValue = 0;
  if (v1[0] == 'A')
  {
    v1.remove(0, 1);
    tempValue = atvIADCPort(v1);
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

  int result = map(tempValue, v2.toInt(), v3.toInt(), 0, v4.toInt());
  ledcWrite(v5.toInt(), result);

  return result;
}
#else
int Generic::atvIDACPort(int resource, const char* value)
{
    int p = 0;
    char v1[6] = {0};
    char v2[6] = {0};
    char v3[6] = {0};
    int index = 0;
    for (int i = 0; i < strlen(value); i++)
    {
        if (value[i] == '/')
        {
            p++;
            index = 0; 
        }
        else
        {
            if (p == 0)
                v1[index++] = value[i];
            else if (p == 1)
                v2[index++] = value[i];
            else if (p == 2)
                v3[index++] = value[i];
        }
    }
    v1[index] = '\0'; 
    v2[index] = '\0';
    v3[index] = '\0';

    int tempValue = 0;
    if (v1[0] == 'A')
    {
        tempValue = atvIADCPort(atoi(v1 + 1));
    }
    else if (v1[0] == 'c')
    {
        tempValue = getCount(atoi(v1 + 1));
    }
    else
    {
        tempValue = atoi(v1);
    }

    int mapMin = atoi(v2);
    int mapMax = atoi(v3);
    int result = map(tempValue, mapMin, mapMax, 0, 255);
    if (result < 0)
        result = 0;

    analogWrite(resource, result);
    return result;
}
#endif