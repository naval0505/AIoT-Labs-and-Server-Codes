#include <Adafruit_NeoPixel.h>

#define LED_PIN 48
#define NUMPIXELS 1

#define BUTTON_PIN 0

Adafruit_NeoPixel pixels(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
    Serial.begin(115200);

    pixels.begin();
    pixels.clear();
    pixels.show();

    pinMode(BUTTON_PIN, INPUT_PULLUP);

    Serial.println("GPIO Input Demo Started");
}

void loop()
{
    int buttonState = digitalRead(BUTTON_PIN);

    if (buttonState == LOW)
{
    pixels.setPixelColor(0, pixels.Color(0, 0, 255)); // Blue
    Serial.println("Button Pressed");
}
   else
{
    pixels.setPixelColor(0, pixels.Color(255, 0, 0)); // Red
    Serial.println("Button Released");
}

    pixels.show();

    delay(100);
}
