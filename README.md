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