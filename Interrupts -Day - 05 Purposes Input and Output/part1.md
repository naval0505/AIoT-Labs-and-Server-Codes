# ESP32-S3 Embedded Systems Journey – Day 05
# Chapter 01: Understanding Interrupts from Scratch (Part 1)

![Platform](https://img.shields.io/badge/Platform-ESP32--S3-blue)
![Framework](https://img.shields.io/badge/Framework-Arduino_IDE-green)
![Language](https://img.shields.io/badge/Language-C++-orange)
![Topic](https://img.shields.io/badge/Topic-Interrupts-red)
![Difficulty](https://img.shields.io/badge/Difficulty-Beginner-brightgreen)

---

# Introduction

In the previous chapter, we explored **GPIO (General Purpose Input/Output)** and learned how the ESP32 communicates with the outside world using digital input and output pins.

Our first GPIO project continuously checked the state of the onboard BOOT button inside the `loop()` function and changed the colour of the onboard RGB LED accordingly.

Although this approach worked correctly, it introduced an important question.

> **Does the processor really need to check the button thousands of times every second?**

The answer is **No.**

Modern embedded systems use a far more efficient technique called **Interrupts**.

Instead of continuously checking whether something has changed, the processor simply waits until the hardware notifies it that an event has occurred.

Interrupts are one of the most fundamental concepts in embedded systems and are used extensively in operating systems, robotics, industrial automation, automotive electronics, smartphones, IoT devices, networking equipment, and nearly every modern electronic device.

This chapter introduces the theory behind interrupts before writing any code.

---

# Learning Objectives

After completing this chapter, you should understand:

- What an Interrupt is
- Why Polling is inefficient
- Polling vs Interrupts
- Event-Driven Programming
- How processors react to hardware events
- Why interrupts improve performance
- Real-world examples of interrupts
- Why interrupts are essential in embedded systems

---

# What Problem Do Interrupts Solve?

Imagine writing a simple embedded program.

The processor repeatedly checks whether a button has been pressed.

```
Read Button

↓

Read Button

↓

Read Button

↓

Read Button

↓

Read Button

↓

Repeat Forever
```

This approach is called **Polling**.

Even when the button has not been pressed,

the processor continues checking.

Most of those checks accomplish absolutely nothing.

The CPU spends valuable time repeatedly asking the same question.

```
"Is the button pressed?"

"No."

"Is the button pressed?"

"No."

"Is the button pressed?"

"No."
```

This is acceptable for very small projects,

but it becomes inefficient as systems become more complex.

---

# What is Polling?

Polling is a programming technique where the processor continuously checks whether something has changed.

The CPU repeatedly asks a hardware device for its current state.

For example,

```
CPU

↓

Check Button

↓

Not Pressed

↓

Check Again

↓

Not Pressed

↓

Check Again

↓

Pressed
```

Notice that the processor performs many unnecessary checks before finally detecting an event.

Polling works,

but it is not always the most efficient solution.

---

# Everyday Example of Polling

Imagine waiting for a friend to arrive.

Instead of relaxing,

you walk to the front door every five seconds.

```
Door

↓

Nobody

↓

Door

↓

Nobody

↓

Door

↓

Nobody

↓

Door

↓

Friend Arrives
```

Eventually you find your friend,

but you spent most of your time checking unnecessarily.

This is exactly how polling behaves.

---

# Introducing Interrupts

Interrupts solve this problem completely.

Instead of repeatedly checking,

the processor simply waits.

When an important event occurs,

the hardware immediately informs the CPU.

```
Program Running

↓

Event Occurs

↓

Interrupt

↓

Processor Responds

↓

Continue Running
```

The processor no longer wastes time checking for events that have not happened.

---

# Everyday Example of an Interrupt

Imagine you are reading a book.

```
Reading

↓

Reading

↓

Reading

↓

Reading
```

Suddenly,

someone rings the doorbell.

```
Doorbell Rings

↓

You Stop Reading

↓

Open Door

↓

Return

↓

Continue Reading
```

Notice something important.

You did not continuously check whether someone was standing outside.

Instead,

the doorbell informed you when attention was required.

This is exactly how interrupts work inside a processor.

---

# Definition of an Interrupt

An interrupt is a hardware or software signal that temporarily stops the normal execution of a program so the processor can immediately respond to an important event.

Once the event has been handled,

the processor resumes normal execution exactly where it left off.

---

# Event-Driven Programming

Polling continuously asks whether something has happened.

Interrupts wait until something actually happens.

This style of programming is called:

> **Event-Driven Programming**

The processor responds only when an event occurs.

Examples include:

- Button Press
- Motion Detection
- Incoming Bluetooth Data
- Incoming Wi-Fi Packet
- Sensor Trigger
- Keyboard Input
- Mouse Click
- Timer Expiration

Instead of continuously checking these devices,

the processor simply waits.

---

# Polling vs Interrupts

## Polling

```
CPU

↓

Check

↓

Check

↓

Check

↓

Check

↓

Check

↓

Repeat
```

The processor remains busy checking hardware.

---

## Interrupt

```
CPU

↓

Do Useful Work

↓

Interrupt

↓

Handle Event

↓

Resume Work
```

The processor performs useful work until an event actually occurs.

---

# Why Are Interrupts Faster?

Imagine a fire inside a building.

Option 1:

Security personnel manually inspect every room.

```
Room 1

↓

Room 2

↓

Room 3

↓

Room 4
```

Eventually,

the fire is discovered.

---

Option 2:

A smoke detector immediately activates an alarm.

```
Fire

↓

Smoke Detector

↓

Alarm

↓

Emergency Response
```

The second approach is much faster.

Interrupts work in exactly the same way.

Instead of searching for problems,

the hardware immediately reports them.

---

# Real-World Examples

Interrupts are used everywhere.

## Keyboard

```
Key Press

↓

Interrupt

↓

Character Appears
```

The processor does not repeatedly ask every key whether it has been pressed.

Instead,

the keyboard hardware generates an interrupt.

---

## Computer Mouse

```
Mouse Movement

↓

Interrupt

↓

Cursor Moves
```

---

## Smartphone

```
Touch Screen

↓

Interrupt

↓

Application Responds
```

---

## Bluetooth

```
Packet Received

↓

Interrupt

↓

Data Processed
```

---

## Wi-Fi

```
Network Packet

↓

Interrupt

↓

CPU Reads Packet
```

---

## GPS

```
New Coordinates

↓

Interrupt

↓

Update Location
```

---

## Security Alarm

```
Door Opens

↓

Interrupt

↓

Alarm Activated
```

---

## Industrial Automation

```
Emergency Stop Button

↓

Interrupt

↓

Machine Stops
```

Industrial machines rely heavily on interrupts because immediate response can prevent equipment damage and improve operator safety.

---

# Advantages of Interrupts

Interrupts provide several important advantages.

### Faster Response

The processor reacts immediately.

---

### Better CPU Utilization

Instead of constantly checking hardware,

the processor performs useful work until interrupted.

---

### Lower Power Consumption

Many embedded devices spend most of their time sleeping.

An interrupt wakes the processor only when necessary.

This is especially important for battery-powered IoT devices.

---

### Real-Time Behaviour

Interrupts allow systems to respond within microseconds,

making them suitable for robotics, communication systems, and industrial control.

---

# Polling Still Has a Purpose

Although interrupts are extremely powerful,

polling is not obsolete.

Polling is often preferred when:

- Simplicity is more important than performance.
- Hardware changes very slowly.
- Precise timing is not required.
- The application is extremely small.

Choosing between polling and interrupts depends on the application requirements.

Professional embedded developers understand both techniques and know when each one is appropriate.

---

# Concepts Learned

After completing Part 1, the following concepts have been introduced.

- Interrupts
- Polling
- Event-Driven Programming
- Hardware Events
- CPU Utilization
- Response Time
- Real-Time Systems
- Embedded Event Processing

These concepts establish the theoretical foundation needed before exploring how processors actually implement interrupts internally.

---

# What's Next?

In **Part 2**, we move inside the processor.

Topics include:

- Program Counter (PC)
- CPU Registers
- Stack
- Saving CPU State
- Interrupt Vector Table
- Interrupt Service Routine (ISR)
- Returning from an Interrupt
- Why ISRs Must Be Short
- Interrupt Priorities (Introduction)

Rather than simply knowing that interrupts exist, we will understand exactly what happens inside the ESP32 from the moment the BOOT button is pressed until the processor resumes normal program execution.

---

# Conclusion

Interrupts represent one of the most important concepts in embedded systems programming.

Unlike polling, which continuously checks hardware regardless of whether anything has changed, interrupts allow the processor to remain productive until an event actually occurs. This event-driven model improves efficiency, reduces unnecessary CPU activity, and enables real-time responses to external hardware.

From simple push buttons to complex wireless communication systems, interrupts provide the mechanism that allows modern embedded devices to react quickly and efficiently to the world around them.

Understanding this concept is a major milestone in becoming an embedded systems developer and forms the foundation for advanced topics such as timers, communication peripherals, operating systems, and real-time firmware development.
