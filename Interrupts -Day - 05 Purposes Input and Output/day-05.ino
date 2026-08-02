#include <Adafruit_NeoPixel.h>

#define LED_PIN 48
#define NUMPIXELS 1

#define BUTTON_PIN 0

Adafruit_NeoPixel pixels(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

// This variable can be changed by the interrupt
volatile bool buttonPressed = false;

// Interrupt Service Routine (ISR)
void IRAM_ATTR buttonISR()
{
    buttonPressed = !buttonPressed;
}

void setup()
{
    Serial.begin(115200);

    pixels.begin();
    pixels.clear();

    pinMode(BUTTON_PIN, INPUT_PULLUP);

    // Attach interrupt to GPIO0
    attachInterrupt(
        digitalPinToInterrupt(BUTTON_PIN),
        buttonISR,
        FALLING
    );

    pixels.setPixelColor(0, pixels.Color(255, 0, 0));
    pixels.show();

    Serial.println("Interrupt Demo Started");
}

void loop()
{
    if(buttonPressed)
    {
        pixels.setPixelColor(0, pixels.Color(0, 0, 255));
    }
    else
    {
        pixels.setPixelColor(0, pixels.Color(255, 0, 0));
    }

    pixels.show();

    delay(20);
}
