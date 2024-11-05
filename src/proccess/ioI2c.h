#include "Generic.h"

bool Generic::atvIOPortI2c(int resource, int tCond, const char *prop, int s)
{
  char numI2c = '0';
  bool inver = false;
  String v2 = "";
  String v3 = "";

  size_t length = strlen(prop);
  int x = 0;
  for (size_t i = 0; i < length; i++)
  {

    if (prop[i] == '/')
      x++;
    else if (x == 0)
      numI2c = prop[i];
    else if (x == 1)
      v2 += prop[i];
    else if (x == 2)
      v3 += prop[i];
    else if (x == 3)
      inver = prop[i] == '1';
  }
  bool r = false;
  int adreess = v2.toInt();
  const size_t MAX_LENGTH = v3.length() + 1;
  char bytes[MAX_LENGTH];
  strncpy(bytes, v3.c_str(), MAX_LENGTH - 1);
  bytes[MAX_LENGTH - 1] = '\0';

  switch (tCond)
  {
  case 1:
    s = checkPinStateI2c((numI2c == '0' ? I2C_1 : I2C_2), adreess, resource, bytes);
    if(inver) s = !s;
    break;
  case 2:
    s = !checkPinStateI2c((numI2c == '0' ? I2C_1 : I2C_2), adreess, resource, bytes);
    if(inver) s = !s;
    break;
  case 3: 
    if(inver) 
      r = !s;
    updatePinI2c((numI2c == '0' ? I2C_1 : I2C_2), adreess, resource, r, bytes);
    break;
  case 4:
    if (s){
      r = inver ? 1 : 0;
      updatePinI2c((numI2c == '0' ? I2C_1 : I2C_2), adreess, resource, r, bytes);
    }
    break;
  case 5:
    if (s){
      r = inver ? 0 : 1;
      updatePinI2c((numI2c == '0' ? I2C_1 : I2C_2), adreess, resource, r, bytes);
    }
    break;
  }

  return s;
}