// Include libraries
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>

// Configure the LCD screen
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

// Define the backlight color
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

int SolarPanelCurrent_IN = A0;
int SolarPanelVoltage_IN = A1;
int BatteryVoltage_IN = A2;

int outputToRelayPin = 8;

int control = 0;

void setup() {

  // Debugging output
  Serial.begin(9600);
  
  // Set up the columns and rows in the LCD screen
  lcd.begin(16, 2);
  
  // Setting the pin to input mode
  pinMode(SolarPanelCurrent_IN, INPUT);
  pinMode(SolarPanelVoltage_IN, INPUT);
  pinMode(BatteryVoltage_IN, INPUT);
  
  // Setting the pin to output mode
  pinMode(outputToRelayPin, OUTPUT);

  // Set LCD screen's backlight
  lcd.setBacklight(BLUE);
}

void loop() {

  // Setup variables
  uint8_t button = lcd.readButtons();
  
  int getVal_SolarPanelCurrent;
  int getVal_SolarPanelVoltage;
  int getVal_BatteryVoltage;

  float SolarPanelCurrent;
  float SolarPanelVoltage;
  float BatteryVoltage;

  int analogReadsPerAverageCalculation = 10;
  float voltageDividerRatio = 4.44;

  for (int x=0; x<analogReadsPerAverageCalculation; x++){
    
    // Read the all the analog inputs 
    getVal_SolarPanelCurrent = analogRead(SolarPanelCurrent_IN);
    getVal_SolarPanelVoltage = analogRead(SolarPanelVoltage_IN);
    getVal_BatteryVoltage = analogRead(BatteryVoltage_IN);

    SolarPanelCurrent = SolarPanelCurrent + getVal_SolarPanelCurrent;
    SolarPanelVoltage = SolarPanelVoltage + getVal_SolarPanelVoltage;
    BatteryVoltage = BatteryVoltage + getVal_BatteryVoltage;
  }

  SolarPanelCurrent = SolarPanelCurrent/analogReadsPerAverageCalculation;
  SolarPanelVoltage = SolarPanelVoltage/analogReadsPerAverageCalculation;
  BatteryVoltage = BatteryVoltage/analogReadsPerAverageCalculation;

  // Calculate the voltage and current
  SolarPanelCurrent = (float)SolarPanelCurrent*voltageDividerRatio*(5.0/1023.0);  // This line needs Voltage per Ampere correction, change the 10 to correct value.
  SolarPanelVoltage = (float)SolarPanelVoltage*voltageDividerRatio*(5.0/1023.0);
  BatteryVoltage = (float)BatteryVoltage*voltageDividerRatio*(5.00/1023.0);
  
  // Check battery voltage to control the relay
  if(BatteryVoltage > 11.8){
    digitalWrite(outputToRelayPin, HIGH);
    lcd.setBacklight(RED);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("BATTERY VOLTAGE");
    lcd.setCursor(0,1);
    lcd.print("IN DANGER ZONE");
    delay(5000);
    lcd.clear();
    display_battery_voltage(BatteryVoltage);
    delay(5000);
  }
  else{
    digitalWrite(outputToRelayPin, LOW);
    lcd.setBacklight(BLUE);
    delay(10);

  if(button){
      lcd.clear();
      lcd.setCursor(0,0);
      if(button & BUTTON_UP){
        control--;
      }
      if(button & BUTTON_DOWN){
        control++;
      }
      if(button & BUTTON_LEFT){
        control--;
      }
      if(button & BUTTON_RIGHT){
        control++;
      }
      if(button & BUTTON_SELECT){
        control = 0;
      }
    }
  
    if(abs(control%3) == 0){
      delay(500);
      display_solar_panel_current(SolarPanelCurrent);
    }
    else if(abs(control%3) == 1){
      delay(500);
      display_solar_panel_voltage(SolarPanelVoltage);
    }
    else if(abs(control%3) == 2){
      delay(500);
      display_battery_voltage(BatteryVoltage);
    }
    else{
      lcd.clear();
      lcd.setCursor(0,0);
    }
    
  }



}

void display_solar_panel_current(float SolarPanelCurrent){
  lcd.setCursor(0, 0);
  lcd.print("SOLAR PANEL");
  lcd.setCursor(0, 1);
  lcd.print("CURRENT = ");
  lcd.setCursor(10,1);
  lcd.print(SolarPanelCurrent);
  lcd.setCursor(15,1);
  lcd.print("A");
}

void display_solar_panel_voltage(float SolarPanelVoltage){
  lcd.setCursor(0, 0);
  lcd.print("SOLAR PANEL");
  lcd.setCursor(0, 1);
  lcd.print("VOLTAGE = ");
  lcd.setCursor(10,1);
  lcd.print(SolarPanelVoltage);
  lcd.setCursor(15,1);
  lcd.print("V");
}

void display_battery_voltage(float BatteryVoltage){
  lcd.setCursor(0, 0);
  lcd.print("BATTERY");
  lcd.setCursor(0, 1);
  lcd.print("VOLTAGE = ");
  lcd.setCursor(10,1);
  lcd.print(BatteryVoltage);
  lcd.setCursor(15,1);
  lcd.print("V");
}
