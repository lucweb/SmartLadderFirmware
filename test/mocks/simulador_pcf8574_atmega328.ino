#include <Wire.h>

#define PCF8574_ADDRESS 0x21 
byte pins_state;

void setup() {
  Wire.begin(PCF8574_ADDRESS); 
  Wire.onReceive(receiveEvent); 
  Wire.onRequest(requestEvent); 

  for (int i = 2; i <= 9; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW); 
  }
}

void loop() {}

void receiveEvent(int howMany) {
  if (howMany == 1) { 
    pins_state = Wire.read(); 

    for (int i = 0; i < 8; i++) {
      digitalWrite(2 + i, (pins_state & (1 << i)) ? HIGH : LOW); 
    }
  }
}

void requestEvent() {
  for (int i = 0; i < 8; i++) {
    bitWrite(pins_state, i, digitalRead(2 + i));  
  }

  Wire.write(pins_state);
}
