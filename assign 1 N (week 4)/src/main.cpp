#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define BUTTON_PIN 18

Adafruit_SSD1306 display(
  SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET
);

int buttonCount = 0;
int lastButtonState = HIGH;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found!");
    while (true) {
      delay(100);
    }
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(10, 10);
  display.println("Button Counter");
  display.setCursor(10, 30);
  display.println("Presses: 0");
  display.display();
}

void loop() {
  int buttonState = digitalRead(BUTTON_PIN);

  // Count only when the button is pressed
  if (lastButtonState == HIGH && buttonState == LOW) {
    buttonCount++;

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);

    display.setTextSize(1);
    display.setCursor(10, 5);
    display.println("Button Counter");

    display.setTextSize(2);
    display.setCursor(10, 25);
    display.print("Count: ");
    display.println(buttonCount);

    display.display();

    Serial.print("Button presses: ");
    Serial.println(buttonCount);

    delay(200); // Simple debounce
  }

  lastButtonState = buttonState;
}
