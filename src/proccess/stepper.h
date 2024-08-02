#include "Generic.h"

bool Generic::stepper(int resource, String values)
{ 
  int p = 0;
  char v1[4] = "";
  char v2[8] = "";
  char v3[3] = "";
  char v4 = '0';
  int vIndex = 0;

  for (int i = 0; i < values.length(); i++)
  {
    if (values[i] == '/')
    {
      p++;
      vIndex = 0;
    }
    else
    {
      if (p == 0)
        v1[vIndex++] = values[i];
      else if (p == 1)
        v2[vIndex++] = values[i];
      else if (p == 2)
        v3[vIndex++] = values[i];
      else if (p == 3){
        v4 = values[i];
      }
    }
  }

  int speed = atoi(v1);
  int totalSteps = atoi(v2);
  int c = atoi(v3);

  if (v4 == '1')
    return asyncStepper(resource, speed, totalSteps, c);

  return basicStepper(resource, speed, totalSteps, c);
}

bool Generic::basicStepper(int pin, int v, int totalSteps, int c)
{
  if (CT_CT[c] >= totalSteps)
    return true;

  digitalWrite(pin, 1);
  delayMicroseconds(v);
  digitalWrite(pin, 0);
  CT_CT[c]++;

  return false;
}

bool Generic::asyncStepper(int pin, int v, int totalSteps, int c)
{
  if (CT_CT[c] >= totalSteps)
    return true;
  for (int x = 0; x < totalSteps; x++)
  {
    digitalWrite(pin, 1);
    delayMicroseconds(v);
    digitalWrite(pin, 0);
    CT_CT[c]++;
  }
  return true;
}
