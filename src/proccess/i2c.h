#include "Generic.h"

#if defined(ESP32)
void Generic::startWire(TwoWire &i2c, int sda, int scl)
{
  i2c.begin(sda, scl);
}
#endif

int Generic::getBytes(char *bits, int *buffer)
{
  size_t length = strlen(bits);
  int x = 0;
  String v = "";
  for (size_t i = 0; i < length; i++)
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
  size_t length = strlen(bits);
  int buffer[length];
  getBytes(bits, buffer);

  int bufferD[length];
  getBytes(dados, bufferD);

  i2c.beginTransmission(address);

  int x = 0;
  for (size_t i = 0; i < length; i++)
  {
    if (bits[i] == '-' || (i + 1) == length)
    {
      B_B[buffer[x]] = (uint8_t)bufferD[x];
      i2c.write(bufferD[x]);
      x++;
    }
  }
  i2c.endTransmission();
}

uint32_t Generic::getPinStateReal(TwoWire &i2c, int address, char *bits, int anl)
{
  size_t length = strlen(bits);
  int buffer[length];
  int bytes = getBytes(bits, buffer);
  i2c.requestFrom(address, bytes);

  if (i2c.available() == bytes)
  {
    uint32_t combinedState = 0;
    for (int i = 0; i < bytes; i++)
    {
      uint8_t data = i2c.read();
      B_B[buffer[i]] = data;

      if (i > anl)
        combinedState |= (uint32_t)data << (8 * i);
    }
    return combinedState;
  }
  uint32_t historicalState = 0;
  for (int i = 0; i < bytes; i++)
  {
    if (i > anl)
      historicalState |= (uint32_t)B_B[buffer[i]] << (8 * i);
  }
  return historicalState;
}

bool Generic::checkPinStateI2c(TwoWire &i2c, int address, int pinNumber, char *bits)
{
  uint32_t combinedState = getPinStateReal(i2c, address, bits, -1);
  return (combinedState & (1 << pinNumber)) != 0;
}

uint16_t Generic::getAnalogicValueI2c(TwoWire &i2c, int address, char *bits, int b1, int b2, int anl)
{
  getPinStateReal(i2c, address, bits, anl);
  return (B_B[b1] << 8) | B_B[b2];
}

void Generic::updatePinI2c(TwoWire &i2c, int address, int pin, bool state, char *bits)
{
  updatePinI2cAll(i2c, address, pin, state, bits, -1, -1);
}

void Generic::updatePinI2cAll(TwoWire &i2c, int address, int pin, bool state, char *bits, int pin2, int v)
{
  size_t length = strlen(bits);
  int buffer[length];
  int bytes = getBytes(bits, buffer);

  if (pin < 0 || pin >= (8 * bytes))
  {
    Serial.println(F("Out of ranger"));
    return;
  }

  if (pin2 == -1)
  {
    int byteIndex = pin / 8;
    int bitIndex = pin % 8;

    if (state)
      B_B[buffer[byteIndex]] |= (1 << bitIndex);
    else
      B_B[buffer[byteIndex]] &= ~(1 << bitIndex);
  }
  else
  {
    B_B[pin] = (v >> 8) & 0xFF;
    B_B[pin2] = v & 0xFF;
  }

  i2c.beginTransmission(address);

  for (int i = 0; i < bytes; i++)
  {
    i2c.write(B_B[buffer[i]]);
  }

  i2c.endTransmission();
}

#if defined(ESP32)
void Generic::declareWIRE(const char *prop)
{
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
}
#endif

void Generic::scanI2C(TwoWire &i2c)
{
  byte error, address;
  int nDevices = 0;
  for (address = 0x01; address < 0x7f; address++)
  {
    i2c.beginTransmission(address);
    error = i2c.endTransmission();
    if (error == 0)
    {
      Serial.print(address);
      Serial.print('-');
      nDevices++;
    }
    else if (error != 2)
    {
      Serial.print('e');
      Serial.print(address);
    }
  }
  if (nDevices == 0)
    Serial.print('n');
}