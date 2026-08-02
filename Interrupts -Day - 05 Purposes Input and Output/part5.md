# ESP32-S3 Embedded Systems Journey – Day 05
# Chapter 05: Testing, Troubleshooting, Applications & Conclusion (Part 5)

![Platform](https://img.shields.io/badge/Platform-ESP32--S3-blue)
![Framework](https://img.shields.io/badge/Framework-Arduino_IDE-green)
![Language](https://img.shields.io/badge/Language-C++-orange)
![Project](https://img.shields.io/badge/Project-Interrupts-success)
![Status](https://img.shields.io/badge/Status-Completed-brightgreen)

---

# Introduction

After learning the theory of interrupts, understanding how processors respond to hardware events internally, building our first interrupt-driven project, and analyzing every line of code, the final step is validating the implementation.

Testing is one of the most important phases of embedded systems development.

A program that compiles successfully is not necessarily a program that behaves correctly.

Proper testing ensures that the firmware responds to real hardware events exactly as intended.

This chapter documents the testing procedure, expected observations, common troubleshooting steps, limitations of the project, and real-world applications of interrupt-driven programming.

---

# Testing Environment

## Hardware

- OceanLabz ESP32-S3 DevKit
- ESP32-S3-WROOM Module
- USB Type-C Cable
- Built-in BOOT Button
- Built-in RGB NeoPixel LED

---

## Software

- Arduino IDE 2.x
- ESP32 Arduino Core
- Adafruit NeoPixel Library
- Serial Monitor (115200 Baud)

---

# Upload Procedure

The following steps were performed.

### Step 1

Connect the ESP32-S3 using the USB Type-C cable.

---

### Step 2

Open Arduino IDE.

---

### Step 3

Select the correct board.

```
Tools

↓

Board

↓

ESP32 Arduino

↓

ESP32S3 Dev Module
```

---

### Step 4

Select the correct COM Port.

Example:

```
COM4
```

---

### Step 5

Compile the program.

Expected result:

```
Compilation Successful
```

---

### Step 6

Upload the firmware.

Expected result:

```
Uploading...

↓

Done Uploading
```

---

### Step 7

Open the Serial Monitor.

Baud Rate:

```
115200
```

---

# Expected Behaviour

Immediately after powering on,

the RGB LED should display

```
🔴 RED
```

This indicates that

```
buttonPressed == false
```

---

# Interrupt Test

Press the BOOT button once.

Expected result:

```
🔵 BLUE
```

The LED should remain blue even after releasing the button.

---

Press the BOOT button again.

Expected result:

```
🔴 RED
```

The LED should toggle back to red.

---

Each additional button press should alternate between the two colours.

```
Power ON

↓

🔴 Red

↓

Press

↓

🔵 Blue

↓

Press

↓

🔴 Red

↓

Press

↓

🔵 Blue
```

---

# Complete Workflow

```
ESP32 Starts

↓

Initialize Hardware

↓

Attach Interrupt

↓

Wait

↓

BOOT Button Pressed

↓

GPIO0 Falling Edge

↓

Interrupt Generated

↓

ISR Executes

↓

Toggle Variable

↓

Return

↓

Main Loop Updates LED

↓

Wait For Next Interrupt
```

---

# Observations

Unlike the previous GPIO project,

the processor no longer checks the BOOT button continuously.

Instead,

the GPIO hardware detects the electrical transition and immediately notifies the CPU.

The firmware reacts only when an event occurs.

This demonstrates one of the most important characteristics of modern embedded systems.

---

# Common Errors

## Upload Error

Example:

```
Failed uploading:
no upload port provided
```

### Solution

- Reconnect the USB cable.
- Select the correct COM Port.
- Verify the board selection.
- Close and reopen Arduino IDE if necessary.

---

## Library Not Installed

Example:

```
Adafruit_NeoPixel.h

No such file or directory
```

### Solution

Open:

```
Arduino IDE

↓

Library Manager

↓

Search

↓

Adafruit NeoPixel

↓

Install
```

---

## Wrong GPIO Number

If

```cpp
#define LED_PIN
```

is changed from

```
48
```

to another GPIO,

the onboard RGB LED will no longer respond.

This is because the LED is permanently connected to GPIO48.

---

## Wrong Interrupt Edge

Suppose

```cpp
RISING
```

is selected.

The interrupt will trigger when

```
LOW

↓

HIGH
```

However,

the BOOT button produces

```
HIGH

↓

LOW
```

Therefore,

the correct interrupt type for this project is

```
FALLING
```

---

## Missing volatile

Without

```cpp
volatile
```

the compiler may optimize the variable,

causing unreliable behaviour.

Whenever a variable is modified inside an ISR,

it should generally be declared as

```cpp
volatile
```

---

# Button Bounce

One of the first real-world problems encountered with mechanical buttons is called **Button Bounce**.

A beginner might expect one button press to generate one interrupt.

In reality,

the electrical contacts inside a mechanical switch physically bounce.

Instead of producing

```
HIGH

↓

LOW
```

the signal may actually look like

```
HIGH

↓

LOW

↓

HIGH

↓

LOW

↓

HIGH

↓

LOW
```

within only a few milliseconds.

As a result,

one button press may generate multiple interrupts.

This behaviour explains why embedded systems often implement **debouncing**.

Debouncing can be achieved using:

- Software
- Hardware
- Timers
- RC Circuits

This topic will be explored in the next project.

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

Characteristics:

- Constant CPU usage
- Simple implementation
- Less efficient
- Suitable for small projects

---

## Interrupts

```
CPU Running

↓

Event Occurs

↓

Interrupt

↓

ISR

↓

Resume Program
```

Characteristics:

- Event Driven
- Efficient CPU usage
- Faster response
- Suitable for real-time systems

---

# Advantages of Interrupts

Interrupts provide several important benefits.

- Faster hardware response
- Lower CPU utilization
- Better power efficiency
- Cleaner firmware architecture
- Real-time event handling
- Scalable embedded software design

These advantages explain why interrupts are used extensively in professional embedded systems.

---

# Skills Learned

During this project,

the following concepts were introduced.

Hardware Concepts

- GPIO Interrupts
- Falling Edge Detection
- Hardware Events

Programming Concepts

- Event-Driven Programming
- Interrupt Service Routines
- Toggle Logic
- Boolean Variables
- Volatile Variables

Embedded Systems Concepts

- Program Counter
- CPU Registers
- Stack
- Interrupt Vector Table
- CPU State
- Interrupt Workflow

Professional Concepts

- Efficient Firmware Design
- Interrupt Safety
- ISR Best Practices
- Hardware Abstraction

---

# Real-World Applications

The same interrupt mechanism is used in countless embedded products.

Examples include:

## Smart Door Locks

```
Door Button

↓

Interrupt

↓

Unlock Door
```

---

## Industrial Automation

```
Emergency Stop

↓

Interrupt

↓

Machine Shutdown
```

---

## Robotics

```
Limit Switch

↓

Interrupt

↓

Stop Motor
```

---

## Medical Equipment

```
Sensor Alert

↓

Interrupt

↓

Activate Alarm
```

---

## Automotive Electronics

```
Brake Pedal

↓

Interrupt

↓

Brake Controller
```

---

## IoT Devices

```
Motion Sensor

↓

Interrupt

↓

Send Notification
```

---

## Wireless Communication

```
Bluetooth Packet

↓

Interrupt

↓

Process Data
```

---

## Security Systems

```
Door Sensor

↓

Interrupt

↓

Trigger Alarm
```

Almost every embedded system uses interrupts somewhere within its firmware.

---

# Future Improvements

The next projects will build upon the concepts introduced here.

Upcoming topics include:

- Button Debouncing
- Software Debouncing
- Hardware Debouncing
- Multiple Interrupt Sources
- Timer Interrupts
- PWM
- Analog Inputs (ADC)
- External Sensors
- UART
- I²C
- SPI
- Wi-Fi
- MQTT
- FreeRTOS
- Secure IoT Firmware

Each topic will build naturally upon the interrupt concepts learned in this project.

---

# Key Takeaways

This project introduced one of the most important concepts in embedded systems programming.

Key lessons include:

- Polling repeatedly checks hardware.
- Interrupts respond only when an event occurs.
- Interrupts improve CPU efficiency.
- The CPU saves its execution state before servicing an interrupt.
- The Interrupt Service Routine should remain short.
- The BOOT button generates a Falling Edge interrupt.
- `volatile` ensures variables shared with the ISR remain reliable.
- Interrupt-driven programming forms the foundation of real-time embedded systems.

---

# Conclusion

This project marks an important milestone in the embedded systems learning journey.

By replacing polling with hardware interrupts, the ESP32 now reacts immediately to user input while allowing the processor to spend its time performing other useful work. This event-driven approach is far more efficient and represents the design philosophy used in professional firmware for industrial automation, robotics, automotive electronics, medical devices, and IoT systems.

Beyond writing code, this project provided a deeper understanding of how processors handle hardware events internally through the Program Counter, CPU registers, the Stack, the Interrupt Vector Table, and Interrupt Service Routines. These concepts bridge the gap between high-level programming and the low-level operation of a microcontroller.

With interrupts now understood, the next stage of the journey will focus on **button debouncing**, where we will solve one of the first real-world challenges encountered in embedded systems and continue building toward professional-grade firmware development.
