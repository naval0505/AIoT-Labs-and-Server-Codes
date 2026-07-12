# ESP32-S3 Embedded Systems Journey – Project 01: RGB LED & Bluetooth Low Energy (BLE)

![Platform](https://img.shields.io/badge/Platform-ESP32--S3-blue)
![Framework](https://img.shields.io/badge/Framework-Arduino_IDE-green)
![Language](https://img.shields.io/badge/Language-C++-orange)
![Status](https://img.shields.io/badge/Status-Completed-success)

---

# Introduction

This repository marks the beginning of my Embedded Systems and Internet of Things (IoT) learning journey. Coming from a cybersecurity background, I wanted to understand how embedded devices actually work before diving into IoT security.

Instead of jumping directly into advanced projects, I started with the fundamentals:

- Understanding what an ESP32 is.
- Learning how Arduino IDE programs a microcontroller.
- Writing my first C++ program.
- Controlling the onboard RGB LED.
- Learning how Bluetooth Low Energy (BLE) works.
- Successfully advertising the ESP32 as a BLE device and detecting it from an Android phone.

This project is intentionally simple because every advanced IoT project is built upon these same concepts.

---

# Hardware Used

- OceanLabz ESP32-S3 DevKit N16R8
- ESP32-S3-WROOM-1 Module
- USB Type-C Cable
- Windows 11
- Android Smartphone

---

# Software Used

- Arduino IDE 2.x
- ESP32 Arduino Board Package
- Adafruit NeoPixel Library
- nRF Connect for Mobile (Android)

---

# Understanding the ESP32

The ESP32 is a powerful microcontroller developed by Espressif Systems.

Unlike a normal computer, it does not run Windows or Linux.

Instead, it executes a single program that is stored inside its Flash memory.

Whenever power is supplied, the ESP32 immediately starts executing the uploaded program from beginning to end.

Some of its major features include:

- Dual-core processor
- Built-in Wi-Fi
- Bluetooth Low Energy (BLE)
- GPIO Pins
- PWM
- ADC
- USB Support
- Multiple Communication Protocols (UART, SPI, I2C)

---

# What is Arduino IDE?

Many beginners think Arduino is only a hardware board.

Actually, Arduino consists of three things:

1. Arduino Company
2. Arduino Boards
3. Arduino IDE

For this project, only the Arduino IDE is used.

Arduino IDE allows us to:

- Write C++ code
- Compile the code
- Convert it into machine language
- Upload it to the ESP32

Programming Flow:

```
Developer
      │
      ▼
Arduino IDE
      │
      ▼
Compiler
      │
      ▼
Machine Code
      │
      ▼
USB Cable
      │
      ▼
ESP32
```

---

# Programming Language

The ESP32 is programmed using C++.

Arduino simplifies many complex C++ concepts so beginners can start easily.

Every Arduino program consists of two main functions:

```cpp
void setup()
{

}

void loop()
{

}
```

---

# Understanding setup()

The setup() function executes only one time.

It is used for:

- Initializing hardware
- Configuring GPIO pins
- Starting Serial communication
- Initializing sensors
- Starting Bluetooth
- Connecting Wi-Fi

Example:

```cpp
void setup()
{
    Serial.begin(115200);
}
```

---

# Understanding loop()

After setup() finishes, loop() starts.

It executes forever.

```
Power ON
    │
    ▼
setup()
    │
    ▼
loop()
    │
    ▼
loop()
    │
    ▼
loop()
    │
    ▼
Forever
```

This is the main execution cycle of every Arduino program.

---

# Project 1 — RGB LED Control

The first embedded program written during this journey was controlling the onboard RGB LED.

Initially, the objective was:

- White for 5 seconds
- Red for 3 seconds
- Blue for 2 seconds
- Repeat forever

---

# Installing Required Library

Compilation initially failed with:

```
fatal error:
Adafruit_NeoPixel.h:
No such file or directory
```

Reason:

The required library was not installed.

Solution:

Arduino IDE

Sketch

↓

Include Library

↓

Manage Libraries

↓

Search:

```
Adafruit NeoPixel
```

Install:

```
Adafruit NeoPixel
by Adafruit
```

After installation, the project compiled successfully.

---

# Understanding RGB Values

RGB stands for:

- Red
- Green
- Blue

Every color is represented using three numbers.

Example:

| Color | RGB Value |
|---------|-----------|
| Red | (255,0,0) |
| Green | (0,255,0) |
| Blue | (0,0,255) |
| White | (255,255,255) |
| Yellow | (255,255,0) |
| Cyan | (0,255,255) |
| Magenta | (255,0,255) |
| Off | (0,0,0) |

Each value ranges from:

```
0 → No brightness

255 → Maximum brightness
```

---

# Important NeoPixel Functions

### Set Color

```cpp
pixels.setPixelColor(0, pixels.Color(255,0,0));
```

This tells the LED to become Red.

---

### Show Changes

```cpp
pixels.show();
```

Without this function, the LED never updates.

Think of it as pressing the "Send" button.

---

### Delay

```cpp
delay(3000);
```

Milliseconds:

```
1000 ms = 1 second

2000 ms = 2 seconds

3000 ms = 3 seconds

5000 ms = 5 seconds
```

---

# Testing the RGB LED

The first successful LED program was:

- Red for 2 seconds
- LED Off for 2 seconds

This confirmed:

- Correct GPIO
- Working NeoPixel library
- Successful firmware upload
- Functional onboard RGB LED

---

# Project 2 — Bluetooth Low Energy (BLE)

The next objective was enabling Bluetooth.

Initially, the ESP32 did not appear inside Android Bluetooth settings.

This led to an important learning point.

---

# Classic Bluetooth vs BLE

ESP32-S3 only supports:

✅ Bluetooth Low Energy (BLE)

It does **not** support Classic Bluetooth.

Therefore it does not automatically appear as:

- Headphones
- Speakers
- Keyboard
- Mouse

inside Android Bluetooth settings.

Instead, BLE devices are discovered using specialized BLE scanner applications.

---

# BLE Architecture

```
Android Phone

        ▲

        │

BLE Advertising

        │

ESP32
```

The ESP32 continuously broadcasts advertisement packets.

A BLE Scanner listens for these advertisements.

---

# BLE Terminology Learned

### Peripheral

The ESP32.

It advertises Bluetooth services.

---

### Central

The Android phone.

It scans and connects.

---

### Advertisement

Small packets continuously transmitted by the ESP32 announcing its presence.

---

### GATT Server

The ESP32 acts as a server exposing Bluetooth services and characteristics.

---

# BLE Example Used

Arduino IDE

```
File

↓

Examples

↓

BLE

↓

Server
```

This example was uploaded without modification.

---

# Testing BLE

Instead of Android Bluetooth settings, the following application was used:

```
nRF Connect for Mobile
```

Procedure:

1. Upload BLE Server example.
2. Open nRF Connect.
3. Press Scan.
4. Wait a few seconds.

Result:

The ESP32 was successfully detected.

This confirmed:

- Bluetooth hardware operational
- BLE stack functioning correctly
- Successful advertisement packets
- Android device able to discover the ESP32

---

# Concepts Learned

By completing this first project, the following embedded concepts were learned:

- Installing ESP32 Board Package
- Arduino IDE workflow
- C++ project structure
- setup()
- loop()
- RGB color mixing
- NeoPixel library
- Flashing firmware
- Bluetooth Low Energy
- BLE Advertising
- BLE Peripheral
- BLE Central
- GATT Server
- Using nRF Connect

---

# Challenges Encountered

### Missing Library

Issue:

```
Adafruit_NeoPixel.h:
No such file or directory
```

Solution:

Installed Adafruit NeoPixel library.

---

### RGB Testing

Initial color testing required verifying that the onboard RGB LED responded correctly.

A simplified Red → Off test confirmed the hardware was functioning correctly.

---

### Bluetooth Detection

Initially expected the ESP32 to appear in Android Bluetooth settings.

After learning the difference between Classic Bluetooth and BLE, the device was scanned successfully using nRF Connect.

---

# Key Takeaways

This project established the foundation of embedded programming.

Although simple, it introduced the complete firmware development workflow:

- Writing code
- Compiling
- Flashing
- Hardware interaction
- Bluetooth communication
- Embedded debugging

These concepts serve as the basis for future IoT and embedded systems development.

---

# Future Roadmap

- Control RGB LED over BLE
- Send commands from Android to ESP32
- Read push button input
- Learn GPIO programming
- Interface sensors
- Build Wi-Fi web server
- MQTT communication
- IoT Dashboard
- OTA Firmware Updates
- Home Automation Projects
- IoT Security Research

---

# Conclusion

This project marks the first milestone in my Embedded Systems and IoT learning journey.

Starting from zero knowledge, I learned how an ESP32 executes firmware, how Arduino IDE converts C++ into machine code, how an RGB LED can be controlled using NeoPixel functions, and how Bluetooth Low Energy differs from traditional Bluetooth.

Successfully detecting the ESP32 using nRF Connect demonstrated the complete BLE advertising process and laid the groundwork for future wireless communication projects.

The next step will be building interactive BLE applications where a smartphone communicates directly with the ESP32 to control hardware such as LEDs, sensors, and other peripherals, gradually progressing toward more advanced IoT and cybersecurity-focused embedded systems.
