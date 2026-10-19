#include <Arduino.h>

#define LED 2

hw_timer_t *My_timer = NULL;

void IRAM_ATTR onTimer() {
  digitalWrite(LED, !digitalRead(LED));
}

void setup() {
  pinMode(LED, OUTPUT);

  My_timer = timerBegin(0, 80, true);              // timer 0, prescaler 80 -> 1 MHz
  timerAttachInterrupt(My_timer, &onTimer, true);
  timerAlarmWrite(My_timer, 1000000, true);        // 1,000,000 ticks = 1 s, auto-reload
  timerAlarmEnable(My_timer);
}

void loop() {
}