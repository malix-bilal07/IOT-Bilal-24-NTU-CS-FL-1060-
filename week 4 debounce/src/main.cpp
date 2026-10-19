#include <Arduino.h>

#define LED 4
#define BUTTON_PIN 32

hw_timer_t *My_timer = NULL;
hw_timer_t *debounceTimer = NULL;

volatile bool debounceActive = false;
volatile bool ledState = false;

void IRAM_ATTR onTimer() {
  ledState = !ledState;
  digitalWrite(LED, ledState);
}

void IRAM_ATTR onDebounceTimer() {
  debounceActive = false;
}

void IRAM_ATTR onButtonISR() {
  if (!debounceActive) {
    debounceActive = true;
    ledState = !ledState;
    digitalWrite(LED, ledState);
    timerWrite(debounceTimer, 0);        // restart the 50 ms window
    timerAlarmEnable(debounceTimer);     // re-arm the one-shot alarm
  }
}

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Blink timer: toggles every 1 s
  My_timer = timerBegin(0, 80, true);
  timerAttachInterrupt(My_timer, &onTimer, true);
  timerAlarmWrite(My_timer, 1000000, true);
  timerAlarmEnable(My_timer);

  // Debounce timer: one-shot 50 ms, armed by the button ISR
  debounceTimer = timerBegin(1, 80, true);
  timerAttachInterrupt(debounceTimer, &onDebounceTimer, true);
  timerAlarmWrite(debounceTimer, 50000, false);

  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), onButtonISR, FALLING);
}

void loop() {
}