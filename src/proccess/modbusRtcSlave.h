#include <SoftwareSerial.h>

#define PCF8574_ADDRESS 0x21
byte slave[3];
// 26ib5b13oy3:1:21:20y;
/// @brief
/// @param i2c
/// @param sda
/// @param scl
/// @param address
void Generic::i2cSetupSlave(TwoWire &i2c, int sda, int scl, int address)
{
  i2c.begin(sda, scl, address);
  i2c.onReceive(receiveEvent);
  i2c.onRequest(requestEvent);
}

void Generic::i2cSetupSlaveArduino(int address)
{
  i2c.begin(address);
  i2c.onReceive(receiveEvent);
  i2c.onRequest(requestEvent);
}

void Generic::receiveEvent(int howMany)
{
  if (howMany == 1)
  {
    pins_state = Wire.read();
    for (size_t i = 0; i < 8; i++)
    {
      digitalWrite(2 + i, (pins_state & (1 << i)) ? HIGH : LOW);
    }
  }
}

void Generic::requestEvent()
{
  byte pSte = 0;
  int x = 0;
  String p = "";
  for (size_t x = 0; x < R_R.length(); x++)
  {
    if (R_R[x] == 'y')
      break;
    else if (
        R_R[x] != 'b' &&
        R_R[x] != 'i' &&
        R_R[x] != 'o' &&
        R_R[x] != 'a')
      p += R_R[x];
    else if (R_R[x] == 'b')
    {
      bool value = B_B(p.toInt());
      bitWrite(pSte, x, value);
      p = "";
      x++;
    }
    else if (R_R[x] == 'i' || R_R[x] == 'o')
    {
      bitWrite(pSte, x, digitalRead(p.toInt()));
      p = "";
      x++;
    }
    else if (R_R[x] == 'a')
    {
      int value = atvIADCPort(p.toInt());
      uint8_t highByte = (value >> 8) & 0xFF;
      uint8_t lowByte = value & 0xFF;   
      Wire.write(highByte);
      Wire.write(lowByte)
      p = "";
    }

    if (x == 8)
    {
      Wire.write(pSte);
      x = 0;
    }
  };

  if (x != 0)
    Wire.write(pSte);
}