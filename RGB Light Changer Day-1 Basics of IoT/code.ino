#include <Adafruit_NeoPixel.h> //This is the module that will be loaded by the include which is Adafruit_neopixel

#define LED_PIN 48      // This may be different on your board no of pins on the board
#define NUMPIXELS 1

Adafruit_NeoPixel pixels(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pixels.begin();
}

void loop() {

  // White It can be green but in my eyes these looks like white so
  pixels.setPixelColor(0, pixels.Color(255, 255, 255)); // This means that show this colours for now
  pixels.show();// it will show the basically the pixels
  delay(500);// Nothing time delay between the flash of different lights.

  // Red
  pixels.setPixelColor(0, pixels.Color(255, 0, 0));
  pixels.show();
  delay(300);

  // Blue
  pixels.setPixelColor(0, pixels.Color(0, 0, 255));
  pixels.show();
  delay(200);

}



