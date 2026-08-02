# ESP32-S3 Embedded Systems Journey – Day 05: Interrupts from Scratch

![Platform](https://img.shields.io/badge/Platform-ESP32--S3-blue)
![Framework](https://img.shields.io/badge/Framework-Arduino_IDE-green)
![Language](https://img.shields.io/badge/Language-C++-orange)
![Topic](https://img.shields.io/badge/Topic-Interrupts-red)
![Difficulty](https://img.shields.io/badge/Difficulty-Beginner-brightgreen)
![Status](https://img.shields.io/badge/Status-Completed-success)

---

# Introduction

Welcome to **Day 05** of my Embedded Systems and IoT learning journey using the **ESP32-S3 DevKit**.

After understanding GPIO, digital electronics, voltage, current, pull-up resistors, and building a button-controlled RGB LED project, the next logical step was learning one of the most important concepts in embedded programming:

> **Interrupts**

Most beginner projects constantly check the state of buttons inside the `loop()` function, a technique known as **Polling**. While simple to understand, polling wastes CPU time because the processor repeatedly checks whether something has changed.

Interrupts solve this problem by allowing the hardware to notify the processor only when an event occurs.

Instead of continuously asking:

```
"Has the button been pressed?"
```

the processor simply waits.

When the button is pressed, the hardware immediately interrupts the CPU, executes a small function called an **Interrupt Service Routine (ISR)**, and then resumes normal program execution.

This event-driven model is used throughout modern embedded systems including keyboards, smartphones, automotive electronics, industrial controllers, wireless communication systems, medical devices, and IoT products.

---

# Project Objectives

The objectives of this project were:

- Understand what interrupts are.
- Learn the difference between polling and interrupts.
- Understand how CPUs respond to hardware events.
- Learn about Interrupt Service Routines (ISR).
- Configure GPIO interrupts on the ESP32.
- Detect BOOT button presses using interrupts.
- Toggle the onboard RGB LED without continuously polling the button.
- Introduce event-driven programming.

---

# Hardware Used

- OceanLabz ESP32-S3 DevKit
- ESP32-S3-WROOM Module
- USB Type-C Cable

Built-in Components:

- BOOT Button (GPIO0)
- RGB NeoPixel LED (GPIO48)

No external hardware was required.

---

# Software Used

- Arduino IDE 2.x
- ESP32 Arduino Core
- Adafruit NeoPixel Library

---

# Project Overview

The ESP32 waits for the BOOT button to generate an interrupt.

Whenever the button is pressed:

```
BOOT Button

↓

GPIO0 Interrupt

↓

Interrupt Service Routine

↓

Toggle LED State

↓

RGB LED Updates
```

Unlike the previous GPIO project, the processor no longer needs to repeatedly read the button inside the `loop()` function.

---

# Polling vs Interrupts

## Polling

```
CPU

↓

Read Button

↓

Read Button

↓

Read Button

↓

Read Button

↓

Forever
```

The processor constantly checks the button regardless of whether anything has changed.

---

## Interrupts

```
CPU Running

↓

Button Pressed

↓

Interrupt Generated

↓

ISR Executes

↓

CPU Continues
```

The processor responds only when an event occurs.

This approach is significantly more efficient.

---

# Concepts Covered

This project introduces the following embedded systems concepts:

- Interrupts
- Polling
- Event-Driven Programming
- Interrupt Service Routine (ISR)
- Program Counter (PC)
- CPU Registers
- Stack
- Interrupt Vector Table
- GPIO Interrupts
- Rising Edge
- Falling Edge
- CHANGE Interrupts
- `attachInterrupt()`
- `digitalPinToInterrupt()`
- `volatile`
- Toggle Logic

---

# Project Workflow

```
ESP32 Starts

↓

Initialize RGB LED

↓

Configure GPIO0

↓

Attach Interrupt

↓

Program Waits

↓

BOOT Button Pressed

↓

ISR Executes

↓

Toggle LED State

↓

RGB LED Changes

↓

Continue Running
```

---

# Program Behaviour

| BOOT Button | RGB LED |
|-------------|----------|
| Initial State | 🔴 Red |
| Press Once | 🔵 Blue |
| Press Again | 🔴 Red |
| Press Again | 🔵 Blue |

Unlike the previous project where the LED remained blue only while the button was held, this implementation behaves like a toggle switch.

---



---

# Skills Learned

Through this project I learned:

- Interrupt-driven programming
- Difference between polling and interrupts
- GPIO interrupts
- Event-based firmware design
- Interrupt Service Routines
- Basic CPU interrupt handling
- Toggle logic
- ESP32 interrupt configuration
- Hardware event processing
- Embedded debugging

---

# Real-World Applications

The concepts introduced in this project are used extensively in professional embedded systems.

Examples include:

- Computer Keyboards
- Computer Mice
- Smartphones
- Smart Door Locks
- Medical Devices
- Industrial Automation
- Robotics
- Automotive Electronics
- IoT Devices
- Wireless Communication Systems
- Alarm Systems
- Motion Detection Systems

---

# Future Improvements

Future versions of this project may include:

- Software Debouncing
- Hardware Debouncing
- Multiple Interrupt Sources
- Timer Interrupts
- External Push Buttons
- Interrupt Priorities
- FreeRTOS Interrupt Handling
- Interrupt-safe Programming
- Interrupt-based Sensor Reading
- Low Power Wake-up using GPIO Interrupts

---

# Learning Outcome

This project marks an important milestone in my embedded systems journey.

Rather than continuously checking hardware inside the `loop()` function, I learned how embedded processors respond immediately to external events through hardware interrupts.

Understanding interrupts provides the foundation for real-time embedded programming and prepares me for future topics including timers, PWM, sensors, communication protocols, RTOS development, and secure IoT firmware.

---

# Jai Shri Ram

⭐ If you found this project helpful, consider giving the repository a star and following my Embedded Systems learning journey as I continue exploring the ESP32, IoT development, firmware engineering, and embedded cybersecurity.
