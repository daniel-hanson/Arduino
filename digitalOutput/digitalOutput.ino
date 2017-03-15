// Libraries
// Changed with a new branch
#include <Wire.h>

int outputToRelayPin = 8;

void setup() {
  // Setting the pin to output mode
  pinMode(outputToRelayPin, OUTPUT);
}

uint8_t i=0;
void loop() {
  // Oscillating the output pin
  digitalWrite(outputToRelayPin, HIGH);
  delay(10000);
  digitalWrite(outputToRelayPin, LOW);
  delay(10000);
}
