// ********************************************************************
// ** IMPORTANT: CONFIGURATION FILE REQUIRED                         **
// ********************************************************************
// please configure the 'config.h' file accordingly.
//
// The file 'config.h' contains the following settings:
// - USE_I2C: Set to 1 to enable I2C support, or 0 to disable it.
//
// Make sure to adjust these definitions to match your hardware and
// project requirements.
//
// The 'config.h' file should be located in the same directory
// as this sketch or in the library's directory structure.
// ../documents/Arduino/libraries/SmartLadderFirmware/config.h
// ********************************************************************
#include <SmartLadderArduino.h>

SmartLadderArduino smartLadder;

void setup()
{
  smartLadder.setup();
}

void loop()
{
  smartLadder.loop();
}