#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

void setup() {
  Serial.begin(115200);

  BLEDevice::init("Kabir-ESP32");

  BLEServer *pServer = BLEDevice::createServer();

  Serial.println("BLE is running!");
}

void loop() {
  delay(1000);
}
