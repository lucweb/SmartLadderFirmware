# SmartLadder

## Description

`SmartLadder` is a firmware for Arduino microcontrollers that facilitates the programming of programmable logic controllers (PLCs) using ladder diagrams. It allows users to create ladder programs through a web interface, which generates a string of instructions stored in the microcontroller's memory and continuously read to execute the programmed tasks.

## Version

Current: v1.0.0

## Installation

1. Clone this repository or download the ZIP file and extract it to your Arduino libraries folder:
    ```sh
    git clone https://github.com/lucweb/SmartLadderFirmware.git
    ```

2. Open the Arduino IDE and go to `Sketch` -> `Include Library` -> `Add .ZIP Library...` and select the `SmartLadderFirmware` folder.

3. Include the library in your sketch:
    ```cpp
    #include <SmartLadderArduino.h>
    ```

## Usage with Arduino NANO, UNO, and MEGA

Here is a basic example of how to use the `SmartLadderFirmware` library in an Arduino sketch:

```cpp
#include <SmartLadderArduino.h>

SmartLadderArduino smartLadder;

void setup() {
    smartLadder.setup();
}

void loop() {
    smartLadder.loop();
}
```

## Usage with Esp32

Here is a basic example of how to use the `SmartLadderFirmware` library in an Arduino sketch:

```cpp
#include <SmartLadderEsp32.h>

SmartLadderEsp32 smartLadder;

void setup() {
  smartLadder.setup();
}

void loop() {}
````


# SmartLadder Firmware Configuration

## IMPORTANT: Configuration File Required

By default, Wi-Fi are enabled in the firmware esp32. If you need to modify these settings, or if you want to enable/disable Ethernet (LAN8720) support and I2C, you must configure the `config.h` file accordingly.

### Default Settings

- **USE_WIFI**: Set to `1` to enable Wi-Fi support, or `0` to disable it.
- **USE_I2C**: Set to `1` to enable I2C support, or `0` to disable it.
- **USE_ETH8720**: Set to `1` to enable Ethernet support (LAN8720), or `0` to disable it.

### File Location

The configuration file `config.h` should be located in the same directory as your sketch or within the library's directory structure.

**Example path**:  
`../documents/Arduino/libraries/SmartLadderFirmware/config.h`

Make sure to adjust these definitions to match your hardware setup and project requirements.