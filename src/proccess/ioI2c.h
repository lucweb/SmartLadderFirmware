#include "Generic.h"

bool Generic::atvIOPortI2c(int resource, int tCond, const char *prop, int s)
{
  char numI2c = '0';
  String v2 = "";
  String v3 = "";

  size_t length = strlen(prop);
  int x = 0;
  for (int i = 0; i < length; i++)
  {

    if (prop[i] == '/')
      x++;
    else if (x == 0)
      numI2c = prop[i];
    else if (x == 1)
      v2 += prop[i];
    else if (x == 2)
      v3 += prop[i];
  }

  int adreess = v2.toInt();
  const size_t MAX_LENGTH = v3.length() + 1;
  char bytes[MAX_LENGTH];
  strncpy(bytes, v3.c_str(), MAX_LENGTH - 1);
  bytes[MAX_LENGTH - 1] = '\0';

  switch (tCond)
  {
  case 1:
    s = checkPinStateI2c((numI2c == '0' ? I2C_1 : I2C_2), adreess, resource, bytes);
    break;
  case 2:
    s = !checkPinStateI2c((numI2c == '0' ? I2C_1 : I2C_2), adreess, resource, bytes);
    break;
  case 3:
    updatePinI2c((numI2c == '0' ? I2C_1 : I2C_2), adreess, resource, s, bytes);
    break;
  case 4:
    if (s)
      updatePinI2c((numI2c == '0' ? I2C_1 : I2C_2), adreess, resource, 1, bytes);
    break;
  case 5:
    if (s)
      updatePinI2c((numI2c == '0' ? I2C_1 : I2C_2), adreess, resource, 0, bytes);
    break;
  }

  return s;
}