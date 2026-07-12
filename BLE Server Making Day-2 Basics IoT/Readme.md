# ESP32-S3 Embedded Systems Journey – Day 02: Bluetooth Low Energy (BLE) Server

![Platform](https://img.shields.io/badge/Platform-ESP32--S3-blue)
![Framework](https://img.shields.io/badge/Framework-Arduino_IDE-green)
![Language](https://img.shields.io/badge/Language-C++-orange)
![Bluetooth](https://img.shields.io/badge/Bluetooth-BLE-blueviolet)
![Status](https://img.shields.io/badge/Status-Completed-success)

---

# Introduction

This repository documents **Day 02** of my Embedded Systems and Internet of Things (IoT) learning journey using the **ESP32-S3 DevKit N16R8**. The primary objective of this project was to understand **Bluetooth Low Energy (BLE)**, how it differs from Classic Bluetooth, and how an ESP32 can advertise itself as a BLE server.

Unlike traditional Bluetooth devices such as speakers or headphones, BLE devices are designed for low-power communication and are commonly found in smartwatches, fitness trackers, smart locks, industrial sensors, and other IoT devices.

By the end of this project, I successfully configured my ESP32-S3 as a BLE Server and detected it using the **nRF Connect for Mobile** application on Android.

---

# Project Objectives

The main goals of this project were:

- Learn the basics of Bluetooth Low Energy.
- Understand the difference between Classic Bluetooth and BLE.
- Configure the ESP32-S3 as a BLE Peripheral.
- Upload and run the BLE Server example.
- Detect the ESP32 using a BLE scanner.
- Understand BLE advertising and GATT architecture.

---

# Hardware Used

- OceanLabz ESP32-S3 DevKit N16R8
- ESP32-S3-WROOM-1 Module
- USB Type-C Cable
- Android Smartphone

---

# Software Used

- Arduino IDE 2.x
- ESP32 Arduino Board Package
- Built-in ESP32 BLE Library
- nRF Connect for Mobile (Android)

---

# What is Bluetooth Low Energy (BLE)?

Bluetooth Low Energy (BLE), also known as Bluetooth Smart, is a wireless communication protocol designed for devices that require minimal power consumption.

Unlike Classic Bluetooth, BLE focuses on transmitting small amounts of data while consuming very little battery power.

BLE is commonly used in:

- Smart Watches
- Fitness Bands
- Heart Rate Monitors
- Smart Locks
- Smart Home Devices
- Environmental Sensors
- Industrial IoT Devices

---

# Classic Bluetooth vs BLE

One of the biggest learning points during this project was understanding that the **ESP32-S3 only supports Bluetooth Low Energy (BLE)**.

It does **not** support Classic Bluetooth (BR/EDR).

Because of this, the ESP32 does **not** appear inside the normal Android Bluetooth settings like:

- Speakers
- Headphones
- Mouse
- Keyboard

Instead, BLE devices are discovered using BLE scanning applications.

| Classic Bluetooth | Bluetooth Low Energy |
|-------------------|----------------------|
| Audio Streaming | Sensor Communication |
| Speakers | Smart Watches |
| Headphones | Fitness Bands |
| Keyboard | IoT Devices |
| High Power Usage | Low Power Usage |

---

# BLE Architecture

```
Android Phone
       │
       ▼
BLE Scan
       │
       ▼
ESP32 BLE Advertisement
```

The ESP32 continuously broadcasts advertisement packets.

A BLE scanner listens for these packets and displays the device.

---

# BLE Roles

## Peripheral

The ESP32 acts as a Peripheral.

Responsibilities:

- Advertise itself
- Provide services
- Wait for connections

---

## Central

The Android phone acts as the Central device.

Responsibilities:

- Scan nearby BLE devices
- Connect to peripherals
- Read and write data

---

# Understanding BLE Advertisement

Before two BLE devices communicate, the Peripheral continuously broadcasts small packets called **Advertisement Packets**.

These packets contain information such as:

- Device Name
- Available Services
- Manufacturer Data
- Device UUID

The Central device scans these advertisements before deciding whether to connect.

---

# GATT (Generic Attribute Profile)

BLE communication uses a structure called **GATT**.

The hierarchy is:

```
ESP32

↓

Service

↓

Characteristic

↓

Value
```

A Characteristic stores data that can be:

- Read
- Written
- Notified

---

# Project Implementation

The built-in BLE Server example from Arduino IDE was used.

Navigation:

```
Arduino IDE

↓

File

↓

Examples

↓

BLE

↓

Server
```

No modifications were made to the example code during the initial implementation.

---

# Upload Process

The firmware was compiled and uploaded successfully to the ESP32-S3 using Arduino IDE.

After uploading, the Serial Monitor displayed BLE initialization messages, confirming that the BLE server had started successfully.

---

# Testing the BLE Server

Instead of using the normal Android Bluetooth settings, the **nRF Connect for Mobile** application was installed.

Testing Procedure:

1. Upload the BLE Server example.
2. Open nRF Connect.
3. Start scanning.
4. Wait a few seconds.
5. Observe nearby BLE advertisements.

Result:

The ESP32-S3 appeared successfully inside the scanner application, confirming that BLE advertising was functioning correctly.

---

# Why the ESP32 Was Not Visible in Android Bluetooth Settings

Initially, I expected the ESP32 to appear in Android's Bluetooth device list.

However, I learned that:

- Android Bluetooth settings primarily display devices using Classic Bluetooth profiles.
- The ESP32-S3 only advertises using Bluetooth Low Energy.
- BLE devices require dedicated scanner applications such as nRF Connect or BLE Scanner.

This was an important distinction in understanding modern IoT communication.

---

# Concepts Learned

During this project, I learned:

- What Bluetooth Low Energy is
- Difference between BLE and Classic Bluetooth
- BLE Peripheral
- BLE Central
- BLE Advertisement
- GATT Architecture
- BLE Services
- BLE Characteristics
- Using the built-in BLE Server example
- Flashing firmware to ESP32
- Testing BLE using nRF Connect

---

# Challenges Faced

## Device Not Visible

Initially, the ESP32 did not appear in Android Bluetooth settings.

### Cause

The ESP32-S3 only supports Bluetooth Low Energy.

### Solution

Used **nRF Connect for Mobile** to scan BLE advertisements instead of the standard Bluetooth menu.

---

# Applications of BLE

Bluetooth Low Energy is widely used in:

- Home Automation
- Smart Lighting
- Wearable Devices
- Smart Agriculture
- Industrial Monitoring
- Healthcare Devices
- Asset Tracking
- Smart Security Systems

---

# Key Takeaways

This project introduced the complete BLE workflow using the ESP32-S3.

The most important lesson was understanding that Bluetooth Low Energy operates differently from Classic Bluetooth and requires specialized tools for discovery and communication.

Successfully detecting the ESP32 in nRF Connect confirmed that the BLE stack was configured correctly and that the device was advertising as expected.

---

# Future Improvements

The next stages of this project will include:

- Connect the smartphone to the ESP32.
- Read and write BLE characteristics.
- Control the onboard RGB LED using BLE.
- Send text messages from the phone to the ESP32.
- Build a custom Android controller.
- Interface sensors over BLE.
- Create real-world IoT applications.

---

# Conclusion

Day 02 focused on learning the fundamentals of Bluetooth Low Energy using the ESP32-S3. By configuring the board as a BLE Server, understanding the concepts of Peripheral, Central, Advertisement, and GATT, and successfully detecting the device using nRF Connect, I gained practical experience with one of the most widely used wireless communication protocols in modern embedded systems and IoT.

This project establishes the wireless communication foundation required for future projects involving smartphone control, sensor monitoring, home automation, and secure IoT applications.
