# ESP32-S3 Embedded Systems Journey – Day 05
# Chapter 02: Inside the CPU – How Interrupts Actually Work (Part 2)

![Platform](https://img.shields.io/badge/Platform-ESP32--S3-blue)
![Framework](https://img.shields.io/badge/Framework-Arduino_IDE-green)
![Language](https://img.shields.io/badge/Language-C++-orange)
![Topic](https://img.shields.io/badge/Topic-CPU_Interrupts-red)
![Difficulty](https://img.shields.io/badge/Difficulty-Beginner-brightgreen)

---

# Introduction

In Part 1, we learned what interrupts are and why they are significantly more efficient than continuously polling hardware devices.

However, a very important question still remains.

> **What actually happens inside the processor when an interrupt occurs?**

Many beginner tutorials simply introduce the `attachInterrupt()` function and move directly to writing code.

Professional embedded development goes one level deeper.

To write reliable firmware, it is important to understand what the processor is doing internally whenever an interrupt is generated.

This chapter explores the internal workflow of the CPU from the moment an interrupt signal arrives until the processor resumes normal execution.

---

# Learning Objectives

After completing this chapter, you should understand:

- Program Counter (PC)
- CPU Registers
- Stack
- Interrupt Request
- Interrupt Vector Table
- Interrupt Service Routine (ISR)
- Saving CPU State
- Restoring CPU State
- Returning from an Interrupt
- Why Interrupts are Extremely Fast

---

# The CPU Executes One Instruction at a Time

Many beginners imagine that a processor executes an entire function at once.

In reality, this is not how processors work.

Every program is broken into thousands or even millions of tiny machine instructions.

For example,

```
Instruction 1

↓

Instruction 2

↓

Instruction 3

↓

Instruction 4

↓

Instruction 5
```

The CPU executes these instructions one after another.

Even a simple line of C++ code may become multiple machine instructions.

---

# The Program Counter (PC)

Every processor contains a special register called the **Program Counter**.

The Program Counter always stores the address of the next instruction that the CPU must execute.

Think of reading a book.

You place a bookmark on the current page.

```
Book

↓

Bookmark

↓

Continue Reading Later
```

The Program Counter works exactly like that bookmark.

```
Instruction 1

Instruction 2

Instruction 3  ← Program Counter

Instruction 4

Instruction 5
```

Whenever the CPU finishes one instruction,

the Program Counter automatically moves to the next instruction.

---

# CPU Registers

Processors also contain extremely small and extremely fast memory locations called **Registers**.

Registers temporarily store information the CPU is currently using.

Examples include:

- Numbers
- Variables
- Memory Addresses
- Calculation Results
- CPU Status Information

Think of registers as the processor's pockets.

Instead of walking to a cupboard every time you need something,

you keep frequently used items inside your pocket.

Registers provide the CPU with immediate access to important data.

---

# Normal Program Execution

Suppose your program contains:

```cpp
int counter = 5;

counter++;

digitalWrite(...);
```

Internally,

the processor performs something similar to:

```
Load counter

↓

Add One

↓

Store Result

↓

Write GPIO

↓

Continue
```

The CPU simply moves from one instruction to the next.

---

# Suddenly... An Interrupt Occurs

Imagine the BOOT button is pressed.

```
BOOT Button

↓

GPIO0

↓

Interrupt Signal

↓

CPU
```

The GPIO hardware immediately sends an interrupt request to the processor.

The CPU must now temporarily stop what it is doing.

---

# Does the CPU Lose Its Work?

No.

This is one of the greatest features of modern processors.

Before responding to the interrupt,

the processor saves everything necessary to continue later.

Think of watching a movie.

```
Movie Playing

↓

Pause

↓

Answer Phone

↓

Resume Movie
```

The movie continues from exactly the same point.

The processor behaves in the same way.

---

# Saving the CPU State

Before executing the interrupt,

the processor saves several important pieces of information.

These include:

- Program Counter
- CPU Registers
- Processor Status Flags

Collectively,

this information is called the **CPU State**.

Saving this state allows the processor to continue exactly where it stopped once the interrupt has been handled.

---

# The Stack

Where is the CPU State stored?

It is stored inside a special memory area called the **Stack**.

The Stack operates using a simple principle.

```
Last In

↓

First Out
```

This is commonly known as **LIFO**.

Imagine stacking plates.

```
Plate

↓

Plate

↓

Plate

↓

Plate
```

The last plate placed on top is the first plate removed.

The processor uses the Stack in exactly the same way.

---

# Saving Information on the Stack

When an interrupt occurs,

the processor performs something similar to:

```
Program Counter

↓

Push to Stack
```

```
Registers

↓

Push to Stack
```

```
Status Flags

↓

Push to Stack
```

The CPU can now safely execute another function without losing its previous work.

---

# Interrupt Vector Table

Now another important question arises.

How does the CPU know which interrupt function to execute?

The answer is the **Interrupt Vector Table**.

Think of it as a directory.

```
Fire

↓

Emergency Number
```

```
Police

↓

Emergency Number
```

```
Ambulance

↓

Emergency Number
```

Similarly,

the processor maintains a table containing addresses for every possible interrupt.

Example:

```
GPIO Interrupt

↓

Address 0x1000
```

```
Timer Interrupt

↓

Address 0x2000
```

```
UART Interrupt

↓

Address 0x3000
```

```
Wi-Fi Interrupt

↓

Address 0x4000
```

Whenever an interrupt occurs,

the processor looks inside this table and jumps directly to the corresponding function.

---

# Interrupt Service Routine (ISR)

The function executed during an interrupt is called the:

**Interrupt Service Routine**

or simply

**ISR**.

The ISR is responsible for handling the event that caused the interrupt.

Example:

```
Interrupt

↓

ISR Executes

↓

Task Completed

↓

Return
```

The ISR should perform only the work required to respond to the event.

---

# Returning to the Program

After the ISR finishes,

the processor restores the information previously saved on the Stack.

```
Pop Program Counter

↓

Pop Registers

↓

Restore Status Flags
```

The CPU then continues executing the interrupted program.

Notice something remarkable.

The program resumes exactly where it stopped.

It does **not** restart from the beginning.

---

# Complete Interrupt Workflow

The complete process can now be visualized as follows.

```
Program Running

↓

Instruction 1

↓

Instruction 2

↓

GPIO Interrupt

↓

Save CPU State

↓

Push State to Stack

↓

Locate ISR

↓

Execute ISR

↓

Restore CPU State

↓

Continue Program
```

Everything happens within microseconds.

---

# Why Must ISRs Be Short?

An Interrupt Service Routine temporarily pauses the normal execution of the processor.

While the ISR is executing,

other important work may have to wait.

For this reason,

professional embedded developers follow a simple rule.

> Keep Interrupt Service Routines as short as possible.

Good ISR:

```
Button Pressed

↓

Set Flag

↓

Return
```

Poor ISR:

```
Button Pressed

↓

Delay Five Seconds

↓

Print Thousands of Lines

↓

Connect Wi-Fi

↓

Return
```

Long ISRs reduce system responsiveness and may cause other interrupts to be delayed.

---

# Interrupt Priorities (Introduction)

Not every interrupt has the same importance.

Some events require immediate attention,

while others can wait.

Examples:

High Priority:

- Emergency Stop Button
- Motor Protection
- Watchdog Timer

Lower Priority:

- LED Status Update
- Periodic Logging
- User Interface Refresh

Modern processors can assign different priorities so that critical events are handled first.

More advanced interrupt management will be explored later when working with real-time operating systems (RTOS).

---

# Where Are Interrupts Used?

Interrupts are present in almost every modern electronic device.

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

## Mouse

```
Movement

↓

Interrupt

↓

Cursor Updates
```

---

## Bluetooth

```
Incoming Packet

↓

Interrupt

↓

Read Data
```

---

## Wi-Fi

```
Network Frame

↓

Interrupt

↓

Process Packet
```

---

## GPS Receiver

```
New Coordinates

↓

Interrupt

↓

Update Navigation
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

## Automotive Electronics

```
Brake Pedal

↓

Interrupt

↓

Brake Controller
```

---

## Medical Equipment

```
Sensor Alert

↓

Interrupt

↓

Activate Monitoring
```

---

# Concepts Learned

By the end of Part 2, the following concepts have been introduced.

- Program Counter
- CPU Registers
- Stack
- LIFO Principle
- CPU State
- Interrupt Request
- Interrupt Vector Table
- Interrupt Service Routine
- Restoring Program Execution
- Interrupt Priorities
- Fast Event Processing

These concepts explain exactly what happens inside the processor whenever an interrupt occurs.

---

# What's Next?

In **Part 3**, theory becomes practice.

We will build our first interrupt-driven project using the ESP32-S3.

Topics include:

- Hardware Used
- Project Overview
- BOOT Button Interrupt
- RGB LED Toggle
- `attachInterrupt()`
- `digitalPinToInterrupt()`
- `FALLING` Edge Detection
- Program Workflow

This project demonstrates how a real hardware interrupt is configured and handled using the ESP32 Arduino framework.

---

# Conclusion

Interrupts are much more than a programming feature—they are a fundamental hardware mechanism that allows processors to respond immediately to important events.

By saving the CPU state, consulting the Interrupt Vector Table, executing an Interrupt Service Routine, and restoring execution afterward, the processor can react to external hardware without losing its place in the running program.

Understanding this internal workflow provides a much deeper appreciation of how embedded systems operate and prepares us for writing reliable interrupt-driven firmware on the ESP32 and other microcontroller platforms.
