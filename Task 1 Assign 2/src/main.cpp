/*
 * Homework 2 - Task 1: Dual-LED fading with different PWM frequencies
 * LED1: GPIO25 @ 1 kHz    LED2: GPIO26 @ 5 kHz    10-bit resolution
 * Works with Arduino-ESP32 core 2.x (your 2.0.17) and 3.x.
 */
#include <Arduino.h>

const int LED1_PIN = 25;
const int LED2_PIN = 26;
const uint32_t LED1_FREQ = 1000;   // Hz
const uint32_t LED2_FREQ = 5000;   // Hz
const uint8_t  PWM_RES   = 10;     // bits -> duty 0..1023
const uint32_t DUTY_MAX  = (1UL << PWM_RES) - 1;

// Channels 0/1 share timer 0 and 2/3 share timer 1, so use 0 and 2
const int LED1_CH = 0;
const int LED2_CH = 2;

// Kept global so Task 2 can display them
uint32_t duty1 = 0, duty2 = 0;
int dir1 = 1, dir2 = 1;
const int STEP1 = 4;   // LED1 fades slower
const int STEP2 = 7;   // LED2 fades faster

unsigned long previousMillis = 0;
const unsigned long FADE_INTERVAL = 10;   // ms between fade steps

void pwmInit() {
#if ESP_ARDUINO_VERSION_MAJOR >= 3
  ledcAttachChannel(LED1_PIN, LED1_FREQ, PWM_RES, LED1_CH);
  ledcAttachChannel(LED2_PIN, LED2_FREQ, PWM_RES, LED2_CH);
#else
  ledcSetup(LED1_CH, LED1_FREQ, PWM_RES);
  ledcSetup(LED2_CH, LED2_FREQ, PWM_RES);
  ledcAttachPin(LED1_PIN, LED1_CH);
  ledcAttachPin(LED2_PIN, LED2_CH);
#endif
}

void pwmWriteDuty(int pin, int ch, uint32_t duty) {
#if ESP_ARDUINO_VERSION_MAJOR >= 3
  ledcWrite(pin, duty);
#else
  ledcWrite(ch, duty);
#endif
}

void stepFade(uint32_t &duty, int &dir, int step) {
  long next = (long)duty + dir * step;
  if (next >= (long)DUTY_MAX) { next = DUTY_MAX; dir = -1; }
  if (next <= 0)              { next = 0;        dir = 1;  }
  duty = (uint32_t)next;
}

void setup() {
  Serial.begin(115200);
  pwmInit();
}

void loop() {
  unsigned long now = millis();
  if (now - previousMillis >= FADE_INTERVAL) {
    previousMillis = now;

    stepFade(duty1, dir1, STEP1);
    stepFade(duty2, dir2, STEP2);
    pwmWriteDuty(LED1_PIN, LED1_CH, duty1);
    pwmWriteDuty(LED2_PIN, LED2_CH, duty2);

    Serial.printf("LED1 %3lu%% @%luHz | LED2 %3lu%% @%luHz\n",
                  duty1 * 100UL / DUTY_MAX, LED1_FREQ,
                  duty2 * 100UL / DUTY_MAX, LED2_FREQ);
  }
}