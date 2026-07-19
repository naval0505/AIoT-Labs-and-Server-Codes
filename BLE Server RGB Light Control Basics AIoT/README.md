# ESP32-S3 Embedded Systems Journey – Day 03: Bluetooth Low Energy (BLE) Controlled RGB LED

![Platform](https://img.shields.io/badge/Platform-ESP32--S3-blue)
![Framework](https://img.shields.io/badge/Framework-Arduino_IDE-green)
![Language](https://img.shields.io/badge/Language-C++-orange)
![Bluetooth](https://img.shields.io/badge/Protocol-Bluetooth_LE-blueviolet)
![Status](https://img.shields.io/badge/Status-Completed-success)

---

# Introduction

Welcome to **Day 03** of my Embedded Systems and Internet of Things (IoT) learning journey with the **ESP32-S3 DevKit N16R8**.

In the previous project, I learned how to configure the ESP32 as a Bluetooth Low Energy (BLE) Server and successfully advertised it so that it could be discovered using **nRF Connect** on Android.

The next logical step was to establish **two-way communication** between the smartphone and the ESP32.

The objective of this project was to allow an Android phone to send commands over Bluetooth Low Energy that would be processed by the ESP32 in real time to control the onboard RGB LED.

Unlike the previous project where the ESP32 simply advertised its presence, this project introduces the concept of **BLE Characteristics**, **Callbacks**, and **event-driven programming**, allowing the microcontroller to react instantly whenever data is received from a connected device.

---

# Objectives

The objectives of this project were:

- Understand BLE Services.
- Learn about BLE Characteristics.
- Configure the ESP32 as a writable BLE Server.
- Receive text commands from a smartphone.
- Process incoming Bluetooth data.
- Change the onboard RGB LED based on received commands.
- Monitor communication through the Serial Monitor.

---

# Hardware Used

- OceanLabz ESP32-S3 DevKit N16R8
- ESP32-S3-WROOM-1
- USB Type-C Cable
- Android Smartphone

---

# Software Used

- Arduino IDE 2.x
- ESP32 Arduino Core
- Built-in ESP32 BLE Library
- Adafruit NeoPixel Library
- nRF Connect for Mobile

---

# Project Architecture

```
Android Phone

        │

Bluetooth Low Energy

        │

        ▼

ESP32 BLE Server

        │

Characteristic (Write)

        │

Callback Function

        │

Command Processing

        │

RGB LED
```

---

# Understanding the Workflow

The complete communication flow is shown below.

```
Phone

↓

Connect

↓

Find BLE Service

↓

Find Characteristic

↓

Write Command

↓

ESP32 Receives Data

↓

Callback Executes

↓

LED Changes Color
```

Unlike previous projects where the ESP32 continuously executed logic inside the `loop()` function, BLE communication is **event-driven**.

The callback function is only executed when data is received.

---

# BLE Service

A Service is a logical container that groups related BLE Characteristics.

Think of it as a folder.

```
ESP32

↓

Service

↓

Characteristics
```

Each Service is identified by a unique identifier called a UUID.

Example:

```
4fafc201-1fb5-459e-8fcc-c5c9c331914b
```

---

# BLE Characteristic

A Characteristic is where data is stored.

The smartphone can:

- Read
- Write
- Receive Notifications

For this project the Characteristic was configured with:

- Read
- Write

Properties.

Example UUID:

```
beb5483e-36e1-4688-b7f5-ea07361b26a8
```

---

# BLE Callback

One of the most important concepts introduced in this project was **Callbacks**.

Instead of constantly checking whether new Bluetooth data had arrived, the ESP32 waits until the BLE stack automatically calls the callback function.

```
Phone Writes Data

↓

BLE Detects Change

↓

Callback Function Executes

↓

Process Command
```

This approach is much more efficient than repeatedly checking inside the main loop.

---

# RGB LED Control

The onboard NeoPixel RGB LED was initialized using the Adafruit NeoPixel library.

Supported commands included:

| Command | LED Color |
|----------|-----------|
| RED | 🔴 Red |
| GREEN | 🟢 Green |
| BLUE | 🔵 Blue |
| WHITE | ⚪ White |
| OFF | ⚫ LED Off |

Every command received from the phone immediately updated the LED state.

---

# Event-Driven Programming

Previous embedded programs relied heavily on:

```cpp
loop()
```

where the ESP32 continuously executed code.

This project introduced an entirely different programming model.

Instead of continuously checking for Bluetooth data, the ESP32 simply waits.

Whenever the phone writes new data, the BLE library automatically executes the callback.

```
Waiting...

Waiting...

Waiting...

Phone Sends Data

↓

Callback Runs
```

This model is widely used in embedded systems because it is efficient and conserves CPU resources.

---

# Serial Monitoring

Every received command was also displayed inside the Arduino Serial Monitor.

Example:

```
Received: RED

Received: BLUE

Received: WHITE

Received: OFF
```

Using Serial output greatly simplifies debugging during embedded development.

---

# Testing Procedure

## Step 1

Upload firmware to ESP32.

---

## Step 2

Open Serial Monitor.

```
115200 Baud
```

---

## Step 3

Open nRF Connect.

---

## Step 4

Scan for BLE devices.

Device discovered:

```
Kabir-ESP32
```

---

## Step 5

Connect to the ESP32.

---

## Step 6

Locate the writable Characteristic.

---

## Step 7

Send one of the supported commands.

Example:

```
RED
```

Result:

```
RGB LED → Red
```

Example:

```
BLUE
```

Result:

```
RGB LED → Blue
```

Example:

```
WHITE
```

Result:

```
RGB LED → White
```

Example:

```
OFF
```

Result:

```
LED Off
```

---

# Challenges Faced

## BLE Device Discovery

Initially, the ESP32 was not visible inside Android Bluetooth settings.

### Cause

ESP32-S3 supports Bluetooth Low Energy rather than Classic Bluetooth.

### Solution

Used **nRF Connect** to discover BLE advertisements.

---

## Library Installation

NeoPixel functionality required installing the Adafruit NeoPixel library.

Once installed, the RGB LED was successfully controlled.

---

## Understanding BLE Structure

Initially, the concepts of Services, Characteristics and UUIDs appeared confusing.

Visualizing BLE as a folder containing writable data significantly simplified the understanding of BLE architecture.

---

# Concepts Learned

During this project I learned:

- Bluetooth Low Energy communication
- BLE Services
- BLE Characteristics
- UUIDs
- BLE Read Property
- BLE Write Property
- Callback Functions
- Event-Driven Programming
- NeoPixel RGB LED control
- Processing incoming Bluetooth data
- Serial debugging
- Smartphone to Microcontroller communication

---

# Applications

The same communication model can be expanded into real-world projects such as:

- Smart Home Automation
- Bluetooth Door Locks
- Industrial Sensor Nodes
- Smart Lighting Systems
- BLE Remote Controllers
- Home Security Devices
- Medical Monitoring Devices
- Robotics
- Asset Tracking
- Environmental Monitoring

---

# Skills Gained

This project introduced several important embedded programming concepts:

- BLE Server Development
- Real-Time Communication
- Event-Based Programming
- Data Processing
- RGB LED Control
- Mobile Device Integration
- Embedded Debugging
- Hardware Interaction

---

# Future Improvements

Future versions of this project will include:

- RGB Brightness Control
- Custom Android Controller
- Multiple BLE Characteristics
- Sensor Data Transmission
- Temperature Monitoring
- Battery Status
- BLE Notifications
- Smartphone Dashboard
- Password Protected BLE Commands
- Home Automation Integration

---

# Key Takeaways

This project transformed the ESP32 from a simple BLE advertising device into an interactive embedded system capable of receiving and processing commands from an external device.

The introduction of BLE Characteristics and Callback Functions demonstrated how modern IoT devices communicate efficiently using event-driven programming rather than continuous polling.

By successfully sending commands from an Android smartphone to control the onboard RGB LED, this project established the foundation for future wireless control systems, IoT automation, and embedded cybersecurity projects.

---

# Conclusion

Day 03 represents an important milestone in my Embedded Systems and IoT learning journey.

Building upon the BLE advertising concepts learned previously, I implemented two-way Bluetooth communication between an Android smartphone and the ESP32-S3. The project demonstrated how BLE Services and Characteristics can be used to exchange data, how callback functions respond to incoming commands, and how embedded hardware can react instantly to user input.

This project forms the basis for future developments including Bluetooth-controlled smart devices, wireless sensor networks, home automation systems, and secure IoT applications. Understanding this communication model is a fundamental step toward designing intelligent embedded systems capable of interacting seamlessly with mobile devices and other connected technologies.
