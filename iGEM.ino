#include <Wire.h>
#include <Adafruit_MAX31865.h>

// ----- MAX31865 (RTD Temp Sensor) -----
#define MAX_CS   10   // Chip Select pin
#define MAX_DI   11   // MOSI
#define MAX_DO   12   // MISO
#define MAX_CLK  13   // SCK

// Use hardware SPI (CS pin only)
Adafruit_MAX31865 max = Adafruit_MAX31865(MAX_CS);

// Set to 3-wire RTD
#define RTD_WIRES 3

// ----- EZO-pH Sensor -----
#define EZO_PH_ADDRESS 0x63  // Default I2C address for Atlas Scientific EZO-pH


// -----------------------------
//  Function: readPH()
//  Purpose:  Reads pH value from EZO-pH circuit over I2C
// -----------------------------
String readPH() {
  Wire.beginTransmission(EZO_PH_ADDRESS);
  Wire.write("R");          // send read command
  Wire.endTransmission();
  delay(900);               // wait for measurement

  Wire.requestFrom(EZO_PH_ADDRESS, 20, 1);
  String response = "";
  while (Wire.available()) {
    char c = Wire.read();
    if (c == 0) break;
    response += c;
  }
  return response;
}


// -----------------------------
//  Function: readTemperature()
//  Purpose:  Reads temperature (°C) from MAX31865 with PT100 RTD
// -----------------------------
float readTemperature() {
  uint16_t rtd = max.readRTD();
  float ratio = rtd;
  ratio /= 32768;
  float resistance = ratio * 430.0;  // reference resistor = 430Ω
  float temperature = max.temperature(100.0, 430.0); // for PT100
  return temperature;
}


// -----------------------------
//  Arduino Setup
// -----------------------------
void setup() {
  Serial.begin(9600);
  Wire.begin();
  max.begin(MAX31865_3WIRE);
  Serial.println("EZO-pH + PT100 RTD (MAX31865) Reader");
}


// -----------------------------
//  Arduino Loop
// -----------------------------
void loop() {
  float temperature = readTemperature();
  String ph_value = readPH();

  Serial.print("Temperature (°C): ");
  Serial.println(temperature, 2);

  Serial.print("pH: ");
  Serial.println(ph_value);

  Serial.println("----------------------");
  delay(2000);
}
