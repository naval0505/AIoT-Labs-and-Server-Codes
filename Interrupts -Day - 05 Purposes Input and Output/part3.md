# ESP32-S3 Embedded Systems Journey – Day 05
# Chapter 03: Building Our First Interrupt-Driven Project (Part 3)

![Platform](https://img.shields.io/badge/Platform-ESP32--S3-blue)
![Framework](https://img.shields.io/badge/Framework-Arduino_IDE-green)
![Language](https://img.shields.io/badge/Language-C++-orange)
![Project](https://img.shields.io/badge/Project-Interrupt_Button_Toggle-success)
![Difficulty](https://img.shields.io/badge/Difficulty-Beginner-brightgreen)

---

# Introduction

After understanding what interrupts are and how processors handle them internally, it is finally time to build our first interrupt-driven embedded systems project.

Unlike the previous GPIO project, where the processor continuously checked the BOOT button inside the `loop()` function, this project allows the hardware itself to notify the processor whenever the button is pressed.

Instead of asking,

> "Has the button been pressed?"

thousands of times every second,

the ESP32 simply waits until an interrupt occurs.

This represents one of the biggest transitions in embedded systems programming—from **Polling** to **Event-Driven Programming**.

---

# Project Objective

The objective of this project is to configure the onboard BOOT button to generate a hardware interrupt.

Whenever the interrupt occurs, the processor executes an **Interrupt Service Routine (ISR)** that changes the state of the onboard RGB LED.

Unlike the previous GPIO project, pressing the button does **not** simply turn the LED on while it is held.

Instead, every button press toggles the LED between two states.

---

# Hardware Used

No external hardware is required.

The ESP32 development board already contains everything needed.

## Development Board

- OceanLabz ESP32-S3 DevKit
- ESP32-S3-WROOM Module

## Built-in Components

- BOOT Button
- RGB NeoPixel LED

## Software

- Arduino IDE
- ESP32 Arduino Core
- Adafruit NeoPixel Library

---

# Why Reuse the BOOT Button?

The BOOT button is already connected to GPIO0.

```
BOOT Button

↓

GPIO0

↓

ESP32
```

This allows us to focus entirely on interrupts without introducing additional wiring or external push buttons.

It also demonstrates that many development boards already contain useful hardware for learning embedded systems.

---

# Why Use the Onboard RGB LED?

The onboard NeoPixel RGB LED is connected to GPIO48.

```
GPIO48

↓

NeoPixel RGB LED
```

Since the LED is already wired to the ESP32,

we can immediately observe the interrupt behaviour without needing external LEDs or resistors.

---

# Project Behaviour

The project begins with the LED displaying:

```
🔴 RED
```

Whenever the BOOT button is pressed,

the interrupt changes the internal state.

```
BOOT Press

↓

Interrupt

↓

LED = BLUE
```

Pressing the button again produces another interrupt.

```
BOOT Press

↓

Interrupt

↓

LED = RED
```

Each button press toggles between the two colours.

---

# Project Workflow

The overall workflow of the program is shown below.

```
Power ON

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

Interrupt Generated

↓

ISR Executes

↓

Toggle Variable

↓

Update RGB LED

↓

Wait For Next Interrupt
```

Notice that the program is no longer constantly checking the BOOT button.

The processor responds only when the hardware generates an interrupt.

---

# Understanding the Workflow

Let's follow the entire sequence from the moment the user presses the BOOT button.

---

## Step 1

The user presses the BOOT button.

```
Finger

↓

BOOT Button
```

---

## Step 2

GPIO0 changes electrical state.

```
HIGH

↓

LOW
```

This electrical transition is called the **Falling Edge**.

---

## Step 3

The GPIO hardware detects the Falling Edge.

```
GPIO Hardware

↓

Interrupt Request
```

---

## Step 4

The interrupt controller notifies the processor.

```
Interrupt Controller

↓

CPU
```

---

## Step 5

The CPU temporarily pauses the running program.

```
Running Program

↓

Pause
```

---

## Step 6

The processor executes the Interrupt Service Routine.

```
ISR

↓

Toggle LED State
```

---

## Step 7

The CPU restores its previous execution state.

```
Restore CPU State

↓

Resume Program
```

---

## Step 8

The main program updates the RGB LED.

```
RGB LED

↓

Blue

or

Red
```

This entire process typically completes within microseconds.

---

# Understanding the Toggle Behaviour

Unlike the previous project,

the LED does **not** stay blue only while the button is pressed.

Instead,

every interrupt changes the stored LED state.

Example:

```
Power ON

↓

🔴 Red
```

First button press

↓

```
🔵 Blue
```

Second button press

↓

```
🔴 Red
```

Third button press

↓

```
🔵 Blue
```

This behaviour is commonly called **Toggle Logic**.

---

# Why Doesn't the ISR Change the LED Directly?

A beginner might ask:

> "Why not simply change the LED inside the interrupt?"

Although this may work in very small examples,

professional embedded systems usually avoid performing lengthy operations inside an ISR.

Instead,

the ISR performs only the minimum amount of work.

```
Interrupt

↓

Set Flag

↓

Return
```

The main program later performs the larger task.

This approach keeps interrupt response times extremely fast.

---

# GPIO Interrupt Configuration

The interrupt is attached to GPIO0.

```
GPIO0

↓

Interrupt

↓

ISR
```

Whenever GPIO0 detects the specified signal transition,

the processor automatically executes the interrupt function.

No polling is required.

---

# Why Use FALLING Edge?

Earlier,

we learned that the BOOT button uses an internal pull-up resistor.

Released:

```
HIGH
```

Pressed:

```
LOW
```

The transition therefore becomes:

```
HIGH

↓

LOW
```

This transition is called a:

```
FALLING EDGE
```

Therefore,

the interrupt is configured to trigger whenever the voltage falls from HIGH to LOW.

---

# Polling vs Interrupt Project

Previous GPIO Project

```
loop()

↓

Read Button

↓

Read Button

↓

Read Button

↓

Repeat Forever
```

Interrupt Project

```
Program Running

↓

Interrupt Occurs

↓

ISR Executes

↓

Continue Running
```

The interrupt-based approach allows the processor to spend its time performing useful work instead of continuously checking hardware.

---

# Advantages of This Design

Using interrupts provides several important advantages.

- Faster response time.
- Better CPU utilization.
- Cleaner program structure.
- Lower power consumption.
- More scalable firmware architecture.
- Foundation for real-time embedded systems.

These benefits become increasingly important as projects grow in complexity.

---

# Real-World Applications

The same interrupt mechanism is used in countless professional systems.

Examples include:

## Keyboard

```
Key Press

↓

Interrupt

↓

Display Character
```

---

## Industrial Control

```
Emergency Stop

↓

Interrupt

↓

Shutdown Machine
```

---

## Medical Devices

```
Sensor Alert

↓

Interrupt

↓

Activate Alarm
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

## Automotive Systems

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

Although this project only changes an RGB LED,

the underlying design is identical to those used in real embedded products.

---

# Skills Learned

This project introduces several important firmware development concepts.

Hardware Skills

- GPIO Interrupts
- Event Detection
- Hardware Signalling

Programming Skills

- Event-Driven Programming
- Interrupt Service Routines
- Toggle Logic
- Interrupt Configuration

Engineering Skills

- Hardware Abstraction
- Firmware Design
- Real-Time Programming
- Efficient CPU Utilization

---

# What's Next?

In **Part 4**, we will perform a complete walkthrough of the source code.

Every line will be explained in detail.

Topics include:

- `volatile`
- `bool`
- `IRAM_ATTR`
- Interrupt Service Routine
- `attachInterrupt()`
- `digitalPinToInterrupt()`
- `FALLING`
- Toggle Logic
- `setup()`
- `loop()`
- Why `delay()` still exists
- Professional interrupt programming practices

By the end of Part 4, every statement in the program will be fully understood.

---

# Conclusion

This project represents the transition from basic GPIO programming to real-time embedded systems development.

Instead of continuously checking the BOOT button inside the main program, the ESP32 now relies on hardware interrupts to detect user interaction. The processor immediately responds to each button press by executing an Interrupt Service Routine, updating the system state, and then resuming normal execution.

Although the application itself is intentionally simple, the concepts introduced here form the foundation for advanced embedded applications including robotics, industrial automation, wireless communication, operating systems, and IoT firmware.

Understanding interrupt-driven programming is a major milestone toward becoming an embedded systems engineer.
