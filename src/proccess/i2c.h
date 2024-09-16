#include "Generic.h"

void Generic::startWire(TwoWire &i2c, int sda, int scl)
{
#if defined(ESP32)
  i2c.begin(sda, scl);
#else
  i2c.begin();
#endif
}

int Generic::getBytes(char *bits, int *buffer)
{
  size_t length = strlen(bits);
  int x = 0;
  String v = "";
  for (int i = 0; i < length; i++)
  {

    if (bits[i] == '-' || (i + 1) == length)
    {
      if (bits[i] != '-')
        v += bits[i];
      buffer[x] = v.toInt();
      v = "";
      x++;
    }
    else
      v += bits[i];
  }
  return x;
}

void Generic::setConfigI2c(TwoWire &i2c, int address, char *dados, char *bits)
{
  int buffer[4];
  getBytes(bits, buffer);

  i2c.beginTransmission(address);

  size_t length = strlen(bits);
  int x = 0;
  String v = "";
  for (int i = 0; i < length; i++)
  {
    if (bits[i] == '-' || (i + 1) == length)
    {
      uint8_t data = (uint8_t)v.toInt();
      B_B[buffer[x++]] = data;
      i2c.write(data);
      v = "";
      x++;
    }
    else
      v += bits[i];
  }

  i2c.endTransmission();
}

uint32_t Generic::getPinStateReal(TwoWire &i2c, int address, char *bits)
{
  int buffer[4];
  int bytes = getBytes(bits, buffer);
  i2c.requestFrom(address, bytes);
  if (i2c.available() == bytes)
  {
    uint32_t combinedState = 0;
    for (int i = 0; i < bytes; i++)
    {
      uint8_t data = i2c.read();
      B_B[buffer[i]] = data;
      combinedState |= (uint32_t)data << (8 * i);
    }
    return combinedState;
  }
  uint32_t historicalState = 0;
  for (int i = 0; i < bytes; i++)
  {
    historicalState |= (uint32_t)B_B[buffer[i]] << (8 * i);
  }
  return historicalState;
}

bool Generic::checkPinStateI2c(TwoWire &i2c, int address, int pinNumber, char *bits)
{
  uint32_t combinedState = getPinStateReal(i2c, address, bits);
  return (combinedState & (1 << pinNumber)) != 0;
}

void Generic::updatePinI2c(TwoWire &i2c, int address, int pin, bool state, char *bits)
{
  int buffer[4];
  int bytes = getBytes(bits, buffer);

  if (pin < 0 || pin >= (8 * bytes))
  {
    Serial.println(F("Out of ranger"));
    return;
  }

  int byteIndex = pin / 8;
  int bitIndex = pin % 8;

  if (state)
    B_B[buffer[byteIndex]] |= (1 << bitIndex);
  else
    B_B[buffer[byteIndex]] &= ~(1 << bitIndex);

  i2c.beginTransmission(address);

  for (int i = 0; i < bytes; i++)
  {
    i2c.write(B_B[buffer[i]]);
  }

  i2c.endTransmission();
}

void Generic::declareWIRE(const char *prop)
{
#if defined(ESP32)
  char numI2c = '0';
  String v2 = "";
  String v3 = "";
  String v4 = "";
  String v5 = "";
  String v6 = "";

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
    else if (x == 3)
      v4 += prop[i];
    else if (x == 4)
      v5 += prop[i];
    else if (x == 5)
      v6 += prop[i];
  }

  int address = v2.toInt();
  int sda = v3.toInt();
  int scl = v4.toInt();
  char *data = strdup(v5.c_str());
  char *bytes = strdup(v6.c_str());

  startWire((numI2c == '0' ? I2C_1 : I2C_2), sda, scl);
  setConfigI2c((numI2c == '0' ? I2C_1 : I2C_2), address, data, bytes);

  free(data);
  free(bytes);
#else
  I2C_1.begin();
#endif
}
