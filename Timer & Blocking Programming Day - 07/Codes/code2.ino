
#include <Adafruit_NeoPixel.h>

#define LED_PIN 48
#define NUMPIXELS 1
#define BUTTON_PIN 0

Adafruit_NeoPixel pixels(
  NUMPIXELS,
  LED_PIN,
  NEO_GRB + NEO_KHZ800
);

// Timer variables
unsigned long previousTime = 0;
const unsigned long interval = 1000;

// LED state
bool ledState = false;

// Button variables
bool lastButtonState = HIGH;
unsigned long lastButtonTime = 0;
const unsigned long debounceTime = 200;

void setup()
{
  Serial.begin(115200);

  // Start the RGB LED
  pixels.begin();
  pixels.clear();
  pixels.show();

  // Configure BOOT button
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.println("Day 07 - Timers and Non-Blocking Programming");
}

void loop()
{
  // Get the current time
  unsigned long currentTime = millis();

  // -------------------------
  // BUTTON TASK
  // -------------------------

  bool currentButtonState = digitalRead(BUTTON_PIN);

  if (currentButtonState == LOW &&
      lastButtonState == HIGH &&
      currentTime - lastButtonTime > debounceTime)
  {
    ledState = !ledState;

    lastButtonTime = currentTime;

    Serial.println("Button Pressed");
  }

  lastButtonState = currentButtonState;

  // -------------------------
  // TIMER TASK
  // -------------------------

  if (currentTime - previousTime >= interval)
  {
    previousTime = currentTime;

    if (ledState)
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

    ledState = !ledState;
  }
}


