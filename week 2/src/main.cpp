#include <Arduino.h>

const int buttonPin = 13;  // Button between GPIO 13 and GND
const int ledPin    = 5;   // LED + resistor on GPIO 5

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // With INPUT_PULLUP: pressed = LOW, released = HIGH
  if (digitalRead(buttonPin) == LOW) {
    digitalWrite(ledPin, HIGH);   // pressed -> LED on
  } else {
    digitalWrite(ledPin, LOW);    // released -> LED off
  }
}