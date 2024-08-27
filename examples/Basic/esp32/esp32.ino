// ********************************************************************
// ** IMPORTANT: CONFIGURATION FILE REQUIRED                         **
// ********************************************************************
// By default, Wi-Fi are enabled. If you need to change 
// these settings, or if you want to enable/disable Ethernet support,
// please configure the 'config.h' file accordingly.
//
// The file 'config.h' contains the following settings:
// - USE_WIFI: Set to 1 to enable Wi-Fi support, or 0 to disable it.
// - USE_I2C: Set to 1 to enable I2C support, or 0 to disable it.
// - USE_ETH8720: Set to 1 to enable Ethernet support (LAN8720) and i2c, or 0 to disable it.
//
// Make sure to adjust these definitions to match your hardware and 
// project requirements.
//
// The 'config.h' file should be located in the same directory 
// as this sketch or in the library's directory structure.
// ../documents/Arduino/libraries/SmartLadderFirmware/config.h
// ********************************************************************

#include <SmartLadderEsp32.h>

SmartLadderEsp32 smartLadder;

void setup()
{
  smartLadder.setup();
}

void loop() {}