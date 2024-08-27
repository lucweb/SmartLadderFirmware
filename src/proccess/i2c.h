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
  int byteCount = 0;
  char *endBits = strtok(bits, DELIMITER);
  while (endBits != NULL)
  {
    buffer[byteCount++] = atoi(endBits);
    endBits = strtok(NULL, DELIMITER);
  }
  return byteCount;
}

void Generic::setConfigI2c(TwoWire &i2c, int address, char *dados, char *bits)
{
  int buffer[4];
  getBytes(bits, buffer);

  i2c.beginTransmission(address);

  int byteCount = 0;
  char *token = strtok(dados, DELIMITER);
  while (token != NULL)
  {
    uint8_t data = (uint8_t)atoi(token);
    B_B[buffer[byteCount++]] = data;
    i2c.write(data);
    token = strtok(NULL, DELIMITER);
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
  char *modifiableProp = strdup(prop);
  if (modifiableProp == nullptr)
    return;

  char *bytes;
  char *data;
  int numI2c;
  int address;
  int sda;
  int scl;

  const char delimiter[] = "/";

  int x = 0;
  char *endBits = strtok(modifiableProp, delimiter);

  while (endBits != NULL)
  {
    if (x == 0)
      numI2c = atoi(endBits);
    else if (x == 1)
      address = atoi(endBits);
    else if (x == 2)
      sda = atoi(endBits);
    else if (x == 3)
      scl = atoi(endBits);
    else if (x == 4)
      data = endBits;
    else if (x == 5)
      bytes = endBits;
    x++;
    endBits = strtok(NULL, delimiter);
  }
  startWire((numI2c == 0 ? I2C_1 : I2C_2), sda, scl);
  setConfigI2c((numI2c == 0 ? I2C_1 : I2C_2), address, data, bytes);

  free(modifiableProp);
}
