#include <Wire.h>

#define PCF8575_ADDRESS 0x20 
byte pins_state[2];

void setup() {
  Wire.begin(PCF8575_ADDRESS); 
  Wire.onReceive(receiveEvent); 
  Wire.onRequest(requestEvent); 

  for (int i = 2; i <= 17; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW); 
  }
}

void loop() {}

void receiveEvent(int howMany) {
  if (howMany == 2) { 
    pins_state[0] = Wire.read(); 
    pins_state[1] = Wire.read();

    for (int i = 0; i < 8; i++) {
      digitalWrite(2 + i, (pins_state[0] & (1 << i)) ? HIGH : LOW); 
      digitalWrite(10 + i, (pins_state[1] & (1 << i)) ? HIGH : LOW); 
    }
  }
}

void requestEvent() {
  for (int i = 0; i < 8; i++) {
    bitWrite(pins_state[0], i, digitalRead(2 + i));  
    bitWrite(pins_state[1], i, digitalRead(10 + i)); 
  }

  Wire.write(pins_state[0]);
  Wire.write(pins_state[1]); 
}
