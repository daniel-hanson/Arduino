// Libraries
// Changed with a new branch
#include <Wire.h>

int batVoltageAnalogPin = A1;

void setup() {
  // Setting the pin to output mode
  Serial.begin(9600);
}

uint8_t i=0;
void loop() {
  // Basic Battery Voltage Monitor
  int analogReadsPerAverageCalculation = 10;
  long tempBatteryVoltage = 0;
  long batteryVoltageSum = 0;
  
  for(int x = 0; x < analogReadsPerAverageCalculation; x++) {
    long tempBatteryVoltage = 0;
    tempBatteryVoltage = analogRead(batVoltageAnalogPin);
    batteryVoltageSum += tempBatteryVoltage;
    delay(10);
  }

  int averageBatteryVoltage = (int) (batteryVoltageSum / (long) analogReadsPerAverageCalculation);
  Serial.print(averageBatteryVoltage);
  Serial.println();
}
