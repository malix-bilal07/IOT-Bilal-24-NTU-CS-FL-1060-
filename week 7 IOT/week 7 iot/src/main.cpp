#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define LDR_PIN 34
#define SDA_PIN 21
#define SCL_PIN 22
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Declaration for an SSD1306 display connected to I2C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);
  
  // Initialize I2C with custom pins for ESP32
  Wire.begin(SDA_PIN, SCL_PIN);

  // Initialize the OLED display with error handling
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed. Check wiring!"));
    for(;;); // Halt execution if display fails
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  // Read analog value from LDR (ESP32 ADC is 12-bit: 0 to 4095)
  int adcValue = analogRead(LDR_PIN);
  
  // Convert ADC count to voltage based on 3.3V reference
  float voltage = (adcValue / 4095.0) * 3.3;

  // Update OLED Display
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 10);
  display.print("LDR ADC: "); 
  display.println(adcValue);
  display.print("Voltage: "); 
  display.print(voltage, 2); 
  display.println(" V");
  display.display();

  // Print debug info to Serial Monitor
  Serial.printf("ADC: %d  |  Voltage: %.2f V\n", adcValue, voltage);
  
  delay(1000);
}