#include <Adafruit_NeoPixel.h>

#define LED_PIN 48
#define NUMPIXELS 1

#define BUTTON_PIN 0

Adafruit_NeoPixel pixels(
  NUMPIXELS,
  LED_PIN,
  NEO_GRB + NEO_KHZ800
);

// Button state
volatile bool buttonPressed = false;

// Debouncing variables
volatile unsigned long lastInterruptTime = 0;

const unsigned long debounceTime = 20000;

void IRAM_ATTR buttonISR()
{
  unsigned long currentTime = millis();

  if (currentTime - lastInterruptTime > debounceTime)
  {
    buttonPressed = !buttonPressed;

    lastInterruptTime = currentTime;
  }
}

void setup()
{
  Serial.begin(115200);

  // Start RGB LED
  pixels.begin();
  pixels.clear();
  pixels.show();

  // Configure BOOT button
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Configure interrupt
  attachInterrupt(
    digitalPinToInterrupt(BUTTON_PIN),
    buttonISR,
    FALLING
  );

  // Initial LED state
  pixels.setPixelColor(
    0,
    pixels.Color(255, 0, 0)
  );

  pixels.show();

  Serial.println("Day 06 - Button Debouncing");
  Serial.println("Press BOOT button to toggle LED");
}

void loop()
{
  if (buttonPressed)
  {
    // Blue
    pixels.setPixelColor(
      0,
      pixels.Color(0, 0, 255)
    );
  }
  else
  {
    // Red
    pixels.setPixelColor(
      0,
      pixels.Color(255, 0, 0)
    );
  }

  pixels.show();

  delay(10);
}
