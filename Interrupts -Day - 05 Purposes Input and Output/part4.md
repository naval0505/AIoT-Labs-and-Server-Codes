# ESP32-S3 Embedded Systems Journey – Day 05
# Chapter 04: Complete Code Walkthrough (Part 4)

![Platform](https://img.shields.io/badge/Platform-ESP32--S3-blue)
![Framework](https://img.shields.io/badge/Framework-Arduino_IDE-green)
![Language](https://img.shields.io/badge/Language-C++-orange)
![Topic](https://img.shields.io/badge/Topic-Code_Explanation-success)
![Difficulty](https://img.shields.io/badge/Difficulty-Beginner-brightgreen)

---

# Introduction

In the previous chapters, we learned the theory behind interrupts, explored how processors respond to interrupt requests internally, and built our first interrupt-driven GPIO application using the ESP32-S3.

This chapter explains every important line of the source code.

Rather than simply understanding **what** the program does, the objective is to understand **why every line exists** and how it interacts with the processor and hardware.

Understanding code at this level is an important step toward professional firmware development.

---

# Complete Program Structure

Every Arduino program follows the same basic structure.

```
Libraries

↓

Constants

↓

Objects

↓

Global Variables

↓

Interrupt Service Routine

↓

setup()

↓

loop()
```

Each section has a specific responsibility.

---

# Including the Library

The first line of the program is

```cpp
#include <Adafruit_NeoPixel.h>
```

The NeoPixel library contains pre-written functions for controlling RGB LEDs.

Without this library, we would need to manually generate precisely timed digital signals for every colour update.

Instead of writing hundreds of lines of low-level communication code, we simply use the functions already provided by the library.

Think of a library as a toolbox.

```
Programmer

↓

Library

↓

Ready-Made Functions
```

---

# Defining Constants

The next section defines several constants.

```cpp
#define LED_PIN 48
#define NUMPIXELS 1
#define BUTTON_PIN 0
```

Constants represent values that never change while the program is running.

Instead of writing

```cpp
48
```

throughout the program,

we write

```cpp
LED_PIN
```

which makes the code easier to read and maintain.

---

# Why Use #define?

The `#define` directive performs text substitution before compilation.

Whenever the compiler encounters

```cpp
LED_PIN
```

it replaces it with

```cpp
48
```

The ESP32 never actually sees the name `LED_PIN`.

It only receives the numerical value.

Using meaningful names greatly improves code readability.

---

# Creating the NeoPixel Object

The following line creates an object.

```cpp
Adafruit_NeoPixel pixels(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
```

This object represents the onboard RGB LED.

It stores:

- Number of LEDs
- GPIO pin
- Communication protocol

Breaking it down:

```
NUMPIXELS

↓

1 RGB LED
```

```
LED_PIN

↓

GPIO48
```

```
NEO_GRB

↓

Colour Order
```

```
NEO_KHZ800

↓

Signal Timing
```

From this point onward,

the program communicates with the LED using the object named:

```
pixels
```

---

# Global Variable

Next we create

```cpp
volatile bool buttonPressed = false;
```

This line introduces two new concepts.

- volatile
- bool

---

# bool

A Boolean variable stores only two possible values.

```
true
```

or

```
false
```

Nothing else.

Our variable remembers whether the LED should currently be blue or red.

Initially,

```
false
```

means

```
Red
```

---

# Why volatile?

Normally,

the compiler assumes variables only change inside the main program.

However,

our variable changes inside the Interrupt Service Routine.

The compiler must therefore reload the variable every time instead of assuming its value never changes.

The keyword

```cpp
volatile
```

tells the compiler:

> "This variable may change unexpectedly."

Without `volatile`,

the program may behave incorrectly because the compiler could optimize away important reads.

---

# Interrupt Service Routine

The next function is

```cpp
void IRAM_ATTR buttonISR()
{
    buttonPressed = !buttonPressed;
}
```

This is called the

**Interrupt Service Routine**

or

**ISR**.

Whenever the interrupt occurs,

the processor automatically executes this function.

The program never calls it directly.

The hardware does.

---

# What Does IRAM_ATTR Mean?

The ESP32 stores most program code inside Flash memory.

However,

Flash access can occasionally be unavailable during certain operations.

The attribute

```cpp
IRAM_ATTR
```

places the interrupt function inside Instruction RAM.

This ensures the processor can always execute the ISR immediately.

Although many beginner examples omit this attribute,

it is considered good practice for ESP32 interrupt handlers.

---

# Toggle Logic

Inside the ISR we find

```cpp
buttonPressed = !buttonPressed;
```

The

```cpp
!
```

operator means

```
NOT
```

It reverses the current value.

Example:

```
false

↓

true
```

Next interrupt:

```
true

↓

false
```

Every interrupt therefore changes the LED state.

This behaviour is called

**Toggle Logic**.

---

# setup()

The

```cpp
setup()
```

function executes only once after power is applied.

Its responsibility is to initialize the hardware.

---

# Starting Serial Communication

```cpp
Serial.begin(115200);
```

This enables communication between the ESP32 and the Arduino Serial Monitor.

Anything printed using

```cpp
Serial.println()
```

appears on the computer.

Serial output is one of the most useful debugging tools available during embedded development.

---

# Initializing the RGB LED

The following functions prepare the NeoPixel.

```cpp
pixels.begin();

pixels.clear();

pixels.show();
```

Each has a different purpose.

---

## pixels.begin()

Initializes the NeoPixel driver.

Without this function,

the RGB LED cannot be controlled.

---

## pixels.clear()

Removes any previously stored colour values.

Think of wiping a whiteboard before writing new information.

---

## pixels.show()

This function sends the colour data stored inside memory to the physical LED.

Without

```cpp
pixels.show();
```

the LED never changes colour.

---

# Configuring the BOOT Button

The next line configures GPIO0.

```cpp
pinMode(BUTTON_PIN, INPUT_PULLUP);
```

This tells the ESP32:

```
GPIO0

↓

Input Mode

↓

Enable Internal Pull-up Resistor
```

The result becomes

Released

```
HIGH
```

Pressed

```
LOW
```

---

# Attaching the Interrupt

One of the most important lines in the program is

```cpp
attachInterrupt(
    digitalPinToInterrupt(BUTTON_PIN),
    buttonISR,
    FALLING
);
```

This connects the hardware interrupt to our ISR.

Breaking it down:

---

## digitalPinToInterrupt()

Converts the GPIO number into the corresponding interrupt source.

```
GPIO0

↓

Interrupt Number
```

---

## buttonISR

This tells the processor

which function should execute whenever the interrupt occurs.

```
Interrupt

↓

buttonISR()
```

---

## FALLING

The BOOT button changes

```
HIGH

↓

LOW
```

when pressed.

This transition is called the

**Falling Edge**.

The interrupt therefore triggers only during this transition.

---

# Initial LED State

Next we set

```cpp
pixels.setPixelColor(
0,
pixels.Color(255,0,0)
);
```

The LED begins as

```
🔴 RED
```

The values represent

```
Red

255
```

```
Green

0
```

```
Blue

0
```

---

# loop()

Unlike the previous GPIO project,

the loop no longer checks the button.

Instead,

it only checks the state of the variable.

```cpp
if(buttonPressed)
```

Remember,

this variable changes inside the interrupt.

The button itself is never read here.

---

# Decision Making

If

```
buttonPressed

↓

true
```

the LED becomes

```
🔵 Blue
```

Otherwise,

```
🔴 Red
```

---

# Updating the LED

After selecting the colour,

the program executes

```cpp
pixels.show();
```

Only then is the RGB LED updated.

---

# Why Is delay() Still Present?

Finally,

the program pauses briefly.

```cpp
delay(20);
```

The interrupt continues working even while the processor is inside the delay.

Unlike polling,

the interrupt is generated independently by the hardware.

For this beginner project,

a short delay is perfectly acceptable.

Later,

we will replace delays with timers and non-blocking programming techniques.

---

# Interrupt Workflow

The complete execution sequence is shown below.

```
Program Running

↓

BOOT Button Pressed

↓

GPIO0 Detects Falling Edge

↓

Interrupt Generated

↓

CPU Saves Current State

↓

ISR Executes

↓

Toggle Variable

↓

Return

↓

loop()

↓

LED Updated

↓

Continue Running
```

---

# New Concepts Introduced

This chapter introduced several important firmware concepts.

Programming Concepts

- Global Variables
- Boolean Variables
- Toggle Logic
- Objects
- Libraries

Interrupt Concepts

- volatile
- Interrupt Service Routine
- IRAM_ATTR
- attachInterrupt()
- digitalPinToInterrupt()
- FALLING Edge

Embedded Systems Concepts

- Event-Driven Programming
- Hardware Interrupt Configuration
- Interrupt Safe Variables
- GPIO Interrupts

---

# Professional Notes

Although this example is intentionally simple, professional firmware developers generally follow several best practices.

- Keep ISRs extremely short.
- Avoid using `delay()` inside an ISR.
- Avoid printing to the Serial Monitor inside an ISR.
- Avoid dynamic memory allocation inside an ISR.
- Perform only the minimum work required and allow the main program to handle larger tasks.

These practices improve reliability and ensure that interrupt latency remains very low.

---

# What's Next?

In **Part 5**, we will conclude the project by covering:

- Upload Procedure
- Testing
- Expected Output
- Common Errors
- Troubleshooting
- Button Bounce
- Interrupt vs Polling Comparison
- Real-World Applications
- Skills Learned
- Future Improvements
- Final Conclusion

---

# Conclusion

This chapter demonstrated that interrupt-driven programming is built upon only a small amount of additional code, yet it fundamentally changes how the firmware behaves.

Instead of continuously reading the button inside the main program, the ESP32 now reacts only when the hardware generates an interrupt. The ISR updates the program state, while the main loop simply responds to that updated state.

Understanding these code structures provides a strong foundation for writing efficient firmware and prepares us for more advanced topics such as timers, communication peripherals, FreeRTOS, and real-time embedded systems.
