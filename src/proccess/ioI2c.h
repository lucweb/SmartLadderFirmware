#include "Generic.h"

bool Generic::atvIOPortI2c(int resource, int tCond, const char *prop, int s)
{
  char *modifiableProp = strdup(prop);
  if (modifiableProp == nullptr)
    return false;

  char *bytes;
  int numI2c;
  int adreess;
  int inverter = 0;

  const char delimiter[] = "/";

  int x = 0;
  char *endBits = strtok(modifiableProp, delimiter);

  while (endBits != NULL)
  {
    if (x == 0)
      numI2c = atoi(endBits);
    else if (x == 1)
      adreess = atoi(endBits);
    else if (x == 2)
      bytes = endBits;
    else if (x == 3)
      inverter = atoi(endBits);
    x++;
    endBits = strtok(NULL, delimiter);
  }

  switch (tCond)
  {
  case 1:
    s = checkPinStateI2c((numI2c == 0 ? I2C_1 : I2C_2), adreess, resource, bytes);
    break;
  case 2:
    s = !checkPinStateI2c((numI2c == 0 ? I2C_1 : I2C_2), adreess, resource, bytes);
    break;
  case 3:
    updatePinI2c((numI2c == 0 ? I2C_1 : I2C_2), adreess, resource, s, bytes);
    break;
  case 4:
    if (s)
      updatePinI2c((numI2c == 0 ? I2C_1 : I2C_2), adreess, resource, 1, bytes);
    break;
  case 5:
    if (s)
      updatePinI2c((numI2c == 0 ? I2C_1 : I2C_2), adreess, resource, 0, bytes);
    break;
  }

  free(modifiableProp);
  return s;
}