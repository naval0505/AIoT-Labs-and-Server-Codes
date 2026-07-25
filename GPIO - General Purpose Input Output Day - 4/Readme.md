# ESP32-S3 Embedded Systems Journey – Day 04
# Chapter 01: Understanding GPIO from Scratch (Part 1)

![Platform](https://img.shields.io/badge/Platform-ESP32--S3-blue)
![Framework](https://img.shields.io/badge/Framework-Arduino_IDE-green)
![Language](https://img.shields.io/badge/Language-C++-orange)
![Topic](https://img.shields.io/badge/Topic-GPIO-success)
![Difficulty](https://img.shields.io/badge/Difficulty-Beginner-brightgreen)

---

# Introduction

After successfully learning how to control the onboard RGB LED and communicate with the ESP32-S3 using Bluetooth Low Energy (BLE), the next step is understanding one of the most fundamental concepts in embedded systems:

> **GPIO (General Purpose Input Output).**

Almost every electronic device—from keyboards and smartwatches to industrial robots and IoT devices—relies on GPIO pins to interact with the physical world.

Before connecting sensors, relays, motors, displays, or communication modules, it is essential to understand how a microcontroller sends and receives electrical signals through its GPIO pins.

This chapter is intentionally focused on concepts rather than code. By understanding how electricity flows and how GPIO pins behave, future embedded programming becomes much easier.

---

# Learning Objectives

After completing this chapter, you should understand:

- What GPIO means.
- Difference between Input and Output.
- How a microcontroller communicates with hardware.
- Basic electrical concepts.
- Voltage.
- Current.
- Ground (GND).
- Open and Closed Circuits.
- Digital signals.
- HIGH and LOW logic levels.
- Why GPIO is the foundation of Embedded Systems.

---

# What is Embedded Systems?

An Embedded System is a computer designed to perform a specific task.

Unlike desktop computers that can run many different applications, embedded systems are built for dedicated purposes.

Examples include:

- Washing Machines
- Smart TVs
- Routers
- Smart Watches
- Cars
- Security Cameras
- Medical Devices
- Industrial Controllers
- Smart Home Devices
- Drones

The ESP32 is also an embedded system.

It contains:

- CPU
- RAM
- Flash Memory
- Wi-Fi
- Bluetooth
- GPIO Pins
- Timers
- Communication Interfaces

Unlike a desktop computer, it directly interacts with electronic hardware.

---

# What is GPIO?

GPIO stands for:

```
General
Purpose
Input
Output
```

Every word has an important meaning.

---

# General

The word **General** means the pin is flexible.

The same pin can perform many different tasks depending on how it is programmed.

For example:

Today:

```
GPIO18

↓

LED
```

Tomorrow:

```
GPIO18

↓

Temperature Sensor
```

Next week:

```
GPIO18

↓

Relay
```

The hardware remains the same.

Only the software changes.

This flexibility makes GPIO extremely powerful.

---

# Input

An Input pin receives information from the outside world.

Think of the ESP32 as a person.

When someone asks you a question, you listen.

Listening is receiving information.

That is exactly what an Input pin does.

Examples:

```
Button

↓

GPIO

↓

ESP32
```

```
Temperature Sensor

↓

GPIO

↓

ESP32
```

```
Motion Sensor

↓

GPIO

↓

ESP32
```

The ESP32 simply waits for information to arrive.

---

# Output

An Output pin sends information.

Instead of listening, the ESP32 now speaks.

```
ESP32

↓

GPIO

↓

LED
```

```
ESP32

↓

GPIO

↓

Relay
```

```
ESP32

↓

GPIO

↓

Buzzer
```

The ESP32 tells external hardware what to do.

---

# GPIO in Everyday Life

Imagine yourself.

You have:

- Eyes
- Ears
- Mouth
- Hands

Your eyes receive information.

Your ears receive information.

Your mouth sends information.

Your hands perform actions.

GPIO works exactly the same way.

Input:

```
Outside World

↓

ESP32
```

Output:

```
ESP32

↓

Outside World
```

---

# Why GPIO is Important

Without GPIO, a microcontroller would never know what is happening outside.

It could not detect:

- Button presses
- Motion
- Temperature
- Humidity
- Sound
- Light
- Distance
- Water level

Likewise, it could never control:

- LEDs
- Buzzers
- Motors
- Displays
- Relays
- Locks
- Fans
- Pumps

GPIO acts as the bridge between software and the real world.

---

# Understanding Electricity

Everything in electronics ultimately comes down to one thing:

> **The movement of electrons.**

Imagine a water tank.

```
      _________
     |         |
     | Water   |
     |         |
     |_________|
          │
          │
          ▼
```

Water naturally flows from higher pressure to lower pressure.

Electricity behaves in a similar way.

Instead of water flowing through pipes, electrons flow through wires.

---

# Voltage

Voltage is electrical pressure.

Just like water pressure pushes water through pipes, voltage pushes electrons through a circuit.

Higher voltage creates more electrical pressure.

The ESP32 operates at:

```
3.3 Volts
```

Not:

```
5 Volts
```

This is an important distinction because applying higher voltages directly to ESP32 GPIO pins can permanently damage the microcontroller.

---

# Current

Voltage creates pressure.

Current represents movement.

Imagine opening a tap.

```
Water Tank

↓

Pressure

↓

Water Flow
```

The moving water represents electrical current.

No movement means no current.

Current only exists when electricity has a complete path to travel.

---

# Ground (GND)

Ground is one of the most misunderstood concepts in electronics.

Ground is **not** negative electricity.

Ground is simply the reference point from which all voltages are measured.

Think of standing on a mountain.

```
Mountain Peak

↓

Hill

↓

Ground Level
```

Everything is measured relative to the ground.

Electronics works exactly the same way.

```
3.3V

↓

Ground (0V)
```

Every voltage measurement inside the ESP32 is referenced to Ground.

---

# Why Electricity Needs a Complete Circuit

Electricity cannot simply appear inside a wire.

It must travel in a complete loop.

Imagine disconnecting one side of a wire.

```
Battery

↓

Wire

✖
```

Nothing happens.

Now connect both ends.

```
Battery (+)

↓

LED

↓

Battery (-)
```

Current now flows.

The LED turns ON.

This complete path is called a **Closed Circuit**.

---

# Open Circuit

An Open Circuit has a broken path.

```
Battery

↓

LED

✖ Broken Wire
```

No current flows.

Nothing operates.

---

# Closed Circuit

A Closed Circuit provides a complete electrical path.

```
Battery

↓

LED

↓

Ground

↓

Battery
```

Current flows continuously.

The LED illuminates.

Every electronic device you use operates using thousands or even millions of closed circuits every second.

---

# Key Concepts Learned

By the end of Part 1, the following concepts have been introduced:

- Embedded Systems
- ESP32 Overview
- GPIO
- Input Devices
- Output Devices
- Electrical Flow
- Voltage
- Current
- Ground
- Open Circuits
- Closed Circuits
- Hardware Interaction

These concepts form the theoretical foundation for understanding how a microcontroller communicates with the physical world.

---

# What's Next?

In **Part 2**, we will move inside the ESP32 itself.

Topics include:

- Digital Signals
- HIGH vs LOW
- How GPIO Works Internally
- Internal Transistors
- Pin Drivers
- Why GPIO Pins Can Be Input or Output
- Why Every Pin Cannot Be Used for Everything
- BOOT Button Hardware
- Why Pressing the BOOT Button Reads LOW
- Pull-up Resistors
- Floating Inputs

From this point onward, we transition from understanding electricity to understanding how the ESP32 interprets electrical signals and turns them into software decisions.

---

# Conclusion

GPIO is the bridge between software and the physical world. Every sensor reading, every LED blink, every motor movement, and every button press begins with a GPIO pin.

Before writing programs, it is essential to understand the electrical principles that make those programs possible. Concepts such as voltage, current, ground, and complete circuits form the foundation upon which all embedded systems are built.

With these fundamentals in place, the next chapter will explore the internal architecture of the ESP32's GPIO system, preparing us to build our first interactive hardware project using the BOOT button and onboard RGB LED.

# ESP32-S3 Embedded Systems Journey – Day 04
# Chapter 02: Inside the ESP32 GPIO Architecture (Part 2)

![Platform](https://img.shields.io/badge/Platform-ESP32--S3-blue)
![Framework](https://img.shields.io/badge/Framework-Arduino_IDE-green)
![Language](https://img.shields.io/badge/Language-C++-orange)
![Topic](https://img.shields.io/badge/Topic-GPIO_Architecture-success)
![Difficulty](https://img.shields.io/badge/Difficulty-Beginner-brightgreen)

---

# Introduction

In Part 1, we explored the basic concepts of electricity, GPIO, voltage, current, and circuits. We learned that GPIO pins act as the bridge between the microcontroller and the physical world.

Now it is time to move one level deeper.

Instead of looking at the outside of the ESP32, we will explore what happens **inside the microcontroller** when a GPIO pin is configured as an input or an output.

Understanding these concepts removes the mystery behind functions such as:

```cpp
pinMode()
digitalRead()
digitalWrite()
```

These are not magic functions—they simply control electronic hardware inside the ESP32.

---

# How Does the ESP32 Control Hardware?

Imagine the ESP32 as a tiny factory.

```
                ESP32

        ┌───────────────────┐
        │                   │
        │      CPU          │
        │                   │
        │   GPIO Control    │
        │                   │
        └───────────────────┘
                │
                │
         GPIO Pins
                │
        Outside World
```

The CPU never directly touches an LED or a button.

Instead, it communicates through GPIO pins.

These pins act as doors connecting the processor to external electronics.

---

# Every GPIO Pin is a Tiny Door

Think of every GPIO pin as a small doorway.

```
Inside ESP32

↓

Door (GPIO)

↓

Outside World
```

If the door is configured as an Output,

the ESP32 sends information outside.

If the door is configured as an Input,

the ESP32 waits for information to come inside.

---

# Digital Signals

Before understanding GPIO, we must understand digital electronics.

Unlike humans, a microcontroller cannot understand:

- Warm
- Cold
- Bright
- Dim

Instead, it understands only two electrical states.

```
0

or

1
```

These two states are called **Digital Signals**.

---

# HIGH and LOW

Every digital GPIO pin operates using two logic levels.

```
HIGH

↓

Logic 1

↓

Voltage Present
```

and

```
LOW

↓

Logic 0

↓

Ground
```

For the ESP32,

HIGH is approximately:

```
3.3 Volts
```

LOW is:

```
0 Volts
```

There are no middle values for digital GPIO.

A pin is either HIGH or LOW.

---

# Why Only Two States?

Imagine a room light.

It can be:

```
OFF
```

or

```
ON
```

Nothing exists between ON and OFF.

Digital electronics works exactly like this.

```
LOW

↓

OFF
```

```
HIGH

↓

ON
```

This simplicity allows computers to make decisions extremely quickly.

---

# GPIO as an Output

Suppose GPIO48 controls an LED.

```
ESP32

↓

GPIO48

↓

LED
```

When the program executes

```cpp
digitalWrite(48, HIGH);
```

the ESP32 places approximately **3.3 volts** on GPIO48.

```
CPU

↓

GPIO48

↓

3.3V

↓

LED
```

Electricity flows through the LED.

The LED turns ON.

---

When the program executes

```cpp
digitalWrite(48, LOW);
```

GPIO48 is driven to Ground.

```
CPU

↓

GPIO48

↓

0V

↓

LED OFF
```

No useful voltage difference exists across the LED.

The LED turns OFF.

---

# GPIO as an Input

Now imagine connecting a button.

```
Button

↓

GPIO0

↓

ESP32
```

Instead of sending electricity,

the ESP32 now waits.

It continuously checks:

```
Is GPIO HIGH?

or

Is GPIO LOW?
```

This is exactly what

```cpp
digitalRead()
```

does.

---

# What Happens Inside the ESP32?

Inside every GPIO pin is a tiny electronic switch.

This switch is called a **Transistor**.

Modern processors contain millions—or even billions—of transistors.

For GPIO,

the transistor decides whether the pin should:

- Send voltage
- Read voltage

```
CPU

↓

Internal Transistor

↓

GPIO Pin
```

Although software controls the transistor,

the actual work is performed by hardware.

---

# pinMode()

One of the first functions used in Arduino programming is

```cpp
pinMode()
```

Its job is very simple.

It tells the ESP32 how the GPIO should behave.

Example:

```cpp
pinMode(48, OUTPUT);
```

Meaning:

```
GPIO48

↓

Output Mode

↓

Send Voltage
```

Example:

```cpp
pinMode(0, INPUT);
```

Meaning:

```
GPIO0

↓

Input Mode

↓

Read Voltage
```

The physical pin does not change.

Only its behavior changes.

---

# Why Every GPIO Cannot Do Everything

One of the most common beginner questions is:

> Why don't we simply use any GPIO pin?

The answer is because many pins have dedicated hardware functions.

Some GPIO pins are connected to:

- Flash Memory
- USB
- UART
- Boot Configuration
- RGB LED
- JTAG Debugging

These connections already exist on the circuit board.

Therefore, software must respect the hardware design.

---

# GPIO48 on the ESP32-S3

Earlier we used

```cpp
#define LED_PIN 48
```

This was not chosen randomly.

On the OceanLabz ESP32-S3 DevKit,

the onboard RGB NeoPixel is permanently connected to GPIO48.

```
GPIO48

↓

NeoPixel RGB LED
```

Changing

```cpp
#define LED_PIN 48
```

to

```cpp
#define LED_PIN 10
```

would not control the onboard LED,

because GPIO10 is not physically connected to it.

This demonstrates an important rule of embedded systems:

> Software can only control hardware that actually exists.

---

# The BOOT Button

Your ESP32 board contains two physical buttons.

```
BOOT
```

and

```
RST
```

Although they appear similar,

their purposes are completely different.

---

# RST Button

RST means Reset.

Pressing it simply restarts the processor.

```
Program Running

↓

RST Pressed

↓

ESP32 Restart

↓

setup()

↓

loop()
```

The Reset button is **not** intended to be used as a normal GPIO input.

---

# BOOT Button

The BOOT button is connected to a GPIO pin.

Specifically,

GPIO0.

```
BOOT Button

↓

GPIO0

↓

ESP32
```

Because it is connected to a GPIO,

the software can detect when it is pressed.

This makes it perfect for learning digital inputs without additional hardware.

---

# Why Does Pressing the BOOT Button Read LOW?

This surprises almost every beginner.

Most people expect

```
Button Pressed

↓

HIGH
```

but the opposite happens.

```
Button Pressed

↓

LOW
```

Why?

Because the button is wired to Ground.

The simplified circuit looks like this.

```
          3.3V
            │
            │
      Pull-up Resistor
            │
            │
         GPIO0
            │
        BOOT Button
            │
           GND
```

---

# Button Released

When the button is not pressed,

the pull-up resistor gently connects GPIO0 to 3.3V.

```
GPIO0

↓

HIGH
```

---

# Button Pressed

When the button is pressed,

GPIO0 becomes directly connected to Ground.

```
GPIO0

↓

0V

↓

LOW
```

The ESP32 therefore reads

```
LOW
```

instead of HIGH.

---

# Floating Inputs

Imagine disconnecting everything from GPIO0.

```
GPIO0

↓

Nothing
```

Now ask the ESP32

"What voltage is present?"

There is no reliable answer.

Sometimes electrical noise may cause:

```
HIGH
```

Sometimes

```
LOW
```

Sometimes random switching.

This condition is called a **Floating Input**.

Floating inputs produce unreliable results.

---

# Pull-up Resistor

To solve this problem,

the ESP32 provides an internal pull-up resistor.

Instead of leaving the GPIO floating,

the resistor keeps the pin connected to 3.3V until the button is pressed.

Released:

```
HIGH
```

Pressed:

```
LOW
```

This creates stable and predictable behavior.

---

# INPUT_PULLUP

Instead of adding an external resistor,

Arduino provides

```cpp
INPUT_PULLUP
```

Example:

```cpp
pinMode(0, INPUT_PULLUP);
```

This enables the ESP32's built-in pull-up resistor.

No additional electronic components are required.

---

# Why INPUT_PULLUP is Perfect for Learning

Using INPUT_PULLUP provides several advantages.

- No external resistor required.
- Stable GPIO readings.
- Simpler wiring.
- Ideal for push buttons.
- Built directly into the ESP32.

Many beginner embedded projects rely on this feature.

---

# Key Concepts Learned

By the end of Part 2, we have learned:

- Digital Electronics
- HIGH and LOW Logic
- GPIO Architecture
- Internal Transistors
- pinMode()
- Input Mode
- Output Mode
- GPIO48 and the Onboard RGB LED
- BOOT Button
- RESET Button
- Floating Inputs
- Pull-up Resistors
- INPUT_PULLUP

These concepts explain what actually happens inside the ESP32 whenever software interacts with a GPIO pin.

---

# What's Next?

In **Part 3**, theory becomes practice.

We will build our first GPIO application using only the hardware already available on the ESP32 board.

Project:

```
BOOT Released

↓

🔴 RGB LED = Red
```

```
BOOT Pressed

↓

🔵 RGB LED = Blue
```

During this project, we will also introduce:

- Variables
- digitalRead()
- if / else Statements
- Reading GPIO Inputs
- Controlling Outputs
- Real-Time Decision Making

For the first time, software will react instantly to a physical action performed by the user.

---

# Conclusion

GPIO is much more than a numbered pin on a development board.

Inside every GPIO lies dedicated hardware capable of sending or receiving digital signals through tiny electronic switches called transistors. By configuring these pins correctly, the ESP32 can interact with LEDs, buttons, sensors, motors, and countless other electronic devices.

Understanding concepts such as HIGH, LOW, pull-up resistors, floating inputs, and GPIO modes transforms embedded programming from simple code writing into a clear understanding of how software controls physical hardware.

With this knowledge, we are now ready to build our first real GPIO application using the BOOT button and the onboard RGB LED.

# ESP32-S3 Embedded Systems Journey – Day 04
# Chapter 03: Building Our First GPIO Project (Part 3)

![Platform](https://img.shields.io/badge/Platform-ESP32--S3-blue)
![Framework](https://img.shields.io/badge/Framework-Arduino_IDE-green)
![Language](https://img.shields.io/badge/Language-C++-orange)
![Project](https://img.shields.io/badge/Project-GPIO_Button_Control-success)
![Difficulty](https://img.shields.io/badge/Difficulty-Beginner-brightgreen)

---

# Introduction

After understanding the theory behind GPIO, voltage, current, digital logic, pull-up resistors, and the internal architecture of the ESP32, it is finally time to build our first real embedded systems project.

Unlike previous projects where the ESP32 simply blinked LEDs or advertised itself over Bluetooth, this project introduces something much more important:

> **The ability for software to react to physical events.**

This is one of the defining characteristics of embedded systems.

Whenever a user presses a button, the ESP32 immediately detects that event and performs an action.

This interaction between software and hardware is the foundation of nearly every embedded device in existence.

---

# Project Objective

The objective of this project is simple.

Use the onboard BOOT button as an input device and the onboard RGB LED as an output device.

The ESP32 will continuously monitor the BOOT button.

When the button is pressed:

```
BOOT Button

↓

GPIO0

↓

ESP32

↓

RGB LED

↓

Blue
```

When the button is released:

```
BOOT Button Released

↓

GPIO0

↓

ESP32

↓

RGB LED

↓

Red
```

Although this project appears simple, it introduces several important embedded programming concepts.

---

# Project Requirements

No external hardware is required.

Everything used in this project already exists on the development board.

Hardware:

- ESP32-S3 DevKit
- USB Type-C Cable

Built-in Components:

- BOOT Button
- RGB NeoPixel LED

Software:

- Arduino IDE
- ESP32 Board Package
- Adafruit NeoPixel Library

---

# Why Use the BOOT Button?

Many beginner projects require:

- Breadboards
- Push Buttons
- Resistors
- Jumper Wires

Since the ESP32-S3 already includes a BOOT button connected to GPIO0, there is no need for additional hardware.

The board itself becomes the learning platform.

```
ESP32 Board

├── BOOT Button ✅
├── RGB LED ✅
└── USB Connection ✅
```

This makes the project simple while still teaching professional embedded programming concepts.

---

# Understanding the Project Flow

The logic behind this project can be represented using the following flow.

```
Program Starts

↓

Initialize RGB LED

↓

Configure BOOT Button

↓

Read GPIO0

↓

Is Button Pressed?

↓

YES

↓

Turn LED Blue

↓

NO

↓

Turn LED Red

↓

Repeat Forever
```

Notice that the ESP32 continuously repeats this process.

This happens hundreds of times every second.

---

# Hardware Communication

The project demonstrates communication in both directions.

Input:

```
Human

↓

BOOT Button

↓

GPIO0

↓

ESP32
```

Output:

```
ESP32

↓

GPIO48

↓

RGB LED
```

This is why GPIO stands for **Input Output**.

One pin receives information.

Another pin sends information.

---

# Why GPIO0?

The BOOT button is physically connected to GPIO0.

```
BOOT Button

↓

GPIO0
```

Whenever the button is pressed,

GPIO0 becomes connected to Ground.

The ESP32 therefore reads:

```
LOW
```

Whenever the button is released,

the internal pull-up resistor pulls GPIO0 back to 3.3V.

The ESP32 reads:

```
HIGH
```

---

# Why GPIO48?

The onboard NeoPixel RGB LED is permanently connected to GPIO48.

```
GPIO48

↓

RGB LED
```

Unlike external LEDs,

the onboard RGB LED cannot simply be moved to another GPIO.

Its hardware connection is fixed during manufacturing.

This is why our program defines:

```cpp
#define LED_PIN 48
```

---

# Understanding the Complete Data Flow

Let's follow what happens from the moment your finger presses the BOOT button.

## Step 1

You press the BOOT button.

```
Finger

↓

BOOT Button
```

---

## Step 2

The button connects GPIO0 to Ground.

```
GPIO0

↓

0 Volts

↓

LOW
```

---

## Step 3

The ESP32 detects the LOW signal.

```
GPIO0

↓

LOW

↓

CPU
```

---

## Step 4

The CPU evaluates the condition.

```
Button == LOW ?

↓

YES
```

---

## Step 5

The ESP32 sends data to GPIO48.

```
GPIO48

↓

Blue RGB Values

↓

NeoPixel LED
```

---

## Step 6

The LED changes colour.

```
🔵 BLUE
```

All of these operations occur in just a few milliseconds.

---

# Embedded Programming is Event Based

Humans react to events.

For example,

someone rings a doorbell.

You hear it.

You open the door.

Embedded systems behave similarly.

```
Event

↓

Microcontroller Detects

↓

Decision

↓

Action
```

In this project:

```
Button Press

↓

GPIO Reads LOW

↓

Decision

↓

Blue LED
```

When the button is released:

```
Button Released

↓

GPIO Reads HIGH

↓

Decision

↓

Red LED
```

---

# Continuous Monitoring

The ESP32 never stops checking the button.

The program continuously executes.

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
```

This continuous monitoring allows the LED to respond almost instantly whenever the button is pressed.

---

# Why Doesn't the Program End?

Unlike desktop applications,

embedded programs are expected to run forever.

A washing machine,

traffic signal,

security camera,

Wi-Fi router,

or smart thermostat never stops running.

Similarly,

our ESP32 continuously executes the program.

```
Power ON

↓

Program Starts

↓

Runs Forever
```

---

# Real-World Applications

Although this example controls only an LED,

the same programming logic can control much more complex systems.

Instead of changing an LED colour,

the ESP32 could:

- Unlock a Smart Door
- Activate an Alarm
- Start a Water Pump
- Control Industrial Machinery
- Switch Home Appliances
- Trigger Emergency Systems
- Operate Smart Lighting
- Send IoT Notifications
- Activate Surveillance Cameras

The only difference is the output device.

The programming logic remains almost identical.

---

# Skills Learned During This Project

This project introduces the first complete interaction between hardware and software.

New concepts include:

- Reading Digital Inputs
- Controlling Digital Outputs
- Using GPIO Pins
- Hardware Decision Making
- Continuous Program Execution
- Embedded Control Flow
- Human to Microcontroller Interaction
- Microcontroller to Hardware Communication

These concepts form the foundation for all future embedded systems projects.

---

# Challenges Faced

During this project, several observations helped improve understanding.

### Why Doesn't Pressing the Button Return HIGH?

Initially, it seems logical that pressing a button should produce a HIGH signal.

However,

because the BOOT button is connected to Ground through an internal pull-up resistor,

pressing it actually produces:

```
LOW
```

Understanding the hardware wiring explains why the software checks for LOW instead of HIGH.

---

### Why Use the Built-In Components?

Using the onboard BOOT button and RGB LED allows us to focus on programming concepts without introducing additional wiring complexity.

This simplifies learning while still exposing us to real embedded systems development.

---

# What Comes Next?

In Part 4,

we will perform a complete code walkthrough.

Every line of the program will be explained in detail.

Topics include:

- `#include`
- `#define`
- Objects
- Variables
- `setup()`
- `loop()`
- `pinMode()`
- `digitalRead()`
- `if`
- `else`
- `pixels.setPixelColor()`
- `pixels.show()`
- `delay()`

Rather than simply understanding what the code does,

we will understand **why every line exists**.

---

# Conclusion

This project marks an important milestone in the embedded systems journey.

For the first time, the ESP32 is no longer executing predefined actions—it is responding to events generated by a user.

By reading the BOOT button through GPIO0 and controlling the onboard RGB LED through GPIO48, we created a complete input-output system entirely with the hardware already available on the development board.

Although the project consists of only a button and an LED, it demonstrates the same interaction model used in professional embedded systems, industrial controllers, IoT devices, automotive electronics, robotics, and smart home automation.

Understanding this project provides the practical foundation needed for every future GPIO-based application.

# ESP32-S3 Embedded Systems Journey – Day 04
# Chapter 04: Complete Code Walkthrough (Part 4)

![Platform](https://img.shields.io/badge/Platform-ESP32--S3-blue)
![Framework](https://img.shields.io/badge/Framework-Arduino_IDE-green)
![Language](https://img.shields.io/badge/Language-C++-orange)
![Topic](https://img.shields.io/badge/Topic-Code_Explanation-success)
![Difficulty](https://img.shields.io/badge/Difficulty-Beginner-brightgreen)

---

# Introduction

In the previous chapters, we learned the electrical theory behind GPIO, explored how GPIO works inside the ESP32, and built our first GPIO-based project using the onboard BOOT button and RGB LED.

Now we will examine the complete program line by line.

The objective of this chapter is not simply to understand what the code does, but to understand **why each line exists** and how it interacts with the hardware inside the ESP32.

The complete source code for this project is available in the repository as:

```
main.ino
```

This chapter explains every important statement used in that file.

---

# Program Structure

Every Arduino program follows the same general structure.

```
Libraries

↓

Constants

↓

Objects

↓

setup()

↓

loop()
```

Each section has a different responsibility.

---

# Including Libraries

The first line of the program is:

```cpp
#include <Adafruit_NeoPixel.h>
```

A library is a collection of pre-written code that allows programmers to perform complex tasks without writing everything from scratch.

Instead of manually generating the electrical timing required to control a NeoPixel LED, the Adafruit NeoPixel library already provides reliable functions for doing so.

Without this library, controlling the RGB LED would require sending precisely timed digital pulses, which is much more complex.

Think of a library as a toolbox.

```
Programmer

↓

Uses Toolbox

↓

Gets Ready-Made Tools
```

Rather than building every tool yourself, you simply use the appropriate one.

---

# Defining Constants

The next section defines several constants.

```cpp
#define LED_PIN 48

#define NUMPIXELS 1

#define BUTTON_PIN 0
```

A constant represents a fixed value that does not change while the program is running.

Instead of repeatedly writing:

```cpp
48
```

throughout the program,

we create a meaningful name.

```
LED_PIN

↓

48
```

This improves readability.

Imagine returning to the project after six months.

Compare these two examples.

Poor readability:

```cpp
digitalWrite(48, HIGH);
```

Better readability:

```cpp
digitalWrite(LED_PIN, HIGH);
```

The second example immediately explains the purpose of the number.

---

# Why Use #define?

The `#define` directive performs a text substitution before the program is compiled.

Whenever the compiler encounters:

```cpp
LED_PIN
```

it replaces it with:

```cpp
48
```

The ESP32 never actually sees the name `LED_PIN`.

It only receives the numerical value.

This technique improves readability while producing efficient machine code.

---

# Creating the NeoPixel Object

The following line creates an object.

```cpp
Adafruit_NeoPixel pixels(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
```

Although this line appears complicated, it simply prepares the RGB LED for use.

It tells the library three things.

- How many LEDs exist.
- Which GPIO controls them.
- Which communication protocol they use.

Breaking it down:

```
NUMPIXELS

↓

1 LED
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

Communication Speed
```

The result is an object named:

```
pixels
```

This object acts as our controller for the onboard RGB LED.

Whenever we want to change the LED,

we communicate with this object.

---

# Understanding Objects

An object is something created from a class that contains both data and functions.

For beginners,

think of an object as a remote control.

```
TV

↓

Remote Control

↓

Buttons

↓

TV Responds
```

The NeoPixel library works similarly.

```
RGB LED

↓

pixels Object

↓

Functions

↓

LED Changes Colour
```

Instead of directly controlling electrical signals,

we simply tell the object what we want.

---

# setup()

Every Arduino program contains a function called:

```cpp
setup()
```

This function executes only once.

```
Power On

↓

setup()

↓

Finished
```

Anything that needs to happen during startup belongs here.

Examples include:

- Initializing hardware.
- Starting Serial communication.
- Configuring GPIO pins.
- Preparing sensors.
- Starting Wi-Fi.

---

# Starting Serial Communication

Inside setup(),

we begin with:

```cpp
Serial.begin(115200);
```

Serial communication allows the ESP32 to send text to the computer.

```
ESP32

↓

USB Cable

↓

Arduino IDE

↓

Serial Monitor
```

This is extremely useful for debugging programs.

Instead of guessing what the microcontroller is doing,

we can simply print information.

---

# Initializing the RGB LED

The next line is:

```cpp
pixels.begin();
```

This initializes the NeoPixel library.

Before this function executes,

the RGB LED cannot be controlled.

Think of it as turning on a television before using the remote.

---

# Clearing the LED

Next we have:

```cpp
pixels.clear();
```

This removes any previous colour values stored inside the NeoPixel library.

Imagine cleaning a whiteboard before writing new notes.

Old information is removed.

New information can now be displayed.

---

# Updating the Hardware

Immediately afterwards,

we execute:

```cpp
pixels.show();
```

This is one of the most important functions in the program.

Changing colours with:

```cpp
pixels.setPixelColor();
```

does **not** immediately change the LED.

Instead,

the library stores the colour in memory.

Only when

```cpp
pixels.show();
```

is executed

are those colours transmitted to the actual RGB LED.

Think of writing a document.

```
Typing

↓

Not Saved Yet
```

Then pressing Save.

```
Document Saved
```

Similarly,

```
setPixelColor()

↓

Store Colour
```

```
show()

↓

Display Colour
```

---

# Configuring the BOOT Button

Next we configure GPIO0.

```cpp
pinMode(BUTTON_PIN, INPUT_PULLUP);
```

This tells the ESP32:

```
GPIO0

↓

Input

↓

Enable Internal Pull-up
```

Now the pin behaves correctly.

Released:

```
HIGH
```

Pressed:

```
LOW
```

Without the pull-up resistor,

the pin would randomly change between HIGH and LOW because of electrical noise.

---

# Printing a Startup Message

The next statement is:

```cpp
Serial.println("GPIO Input Demo Started");
```

Whenever the board boots,

the Serial Monitor displays

```
GPIO Input Demo Started
```

This confirms that the program has started successfully.

---

# loop()

After setup() finishes,

Arduino automatically begins executing

```cpp
loop()
```

Unlike setup(),

loop() never ends.

```
loop()

↓

loop()

↓

loop()

↓

loop()

↓

Forever
```

Embedded systems are expected to operate continuously.

---

# Reading the Button

Inside loop(),

the first statement is:

```cpp
int buttonState = digitalRead(BUTTON_PIN);
```

The ESP32 measures the voltage present on GPIO0.

Possible results are:

```
HIGH

or

LOW
```

That value is stored inside the variable

```
buttonState
```

This variable temporarily remembers the current button status.

---

# Variables

A variable is simply a named memory location.

Imagine placing information inside a labelled box.

```
Button Status

↓

buttonState

↓

HIGH
```

or

```
Button Status

↓

buttonState

↓

LOW
```

Instead of repeatedly reading the GPIO,

the program stores the value once and uses it for decision making.

---

# Making Decisions

Next comes the decision.

```cpp
if(buttonState == LOW)
```

This asks:

```
Is the button pressed?
```

Remember,

because of INPUT_PULLUP,

LOW means

```
Button Pressed
```

If the answer is YES,

the program executes the first block.

Otherwise,

it executes the second block.

---

# Changing the LED Colour

When the button is pressed,

the program executes:

```cpp
pixels.setPixelColor(0, pixels.Color(0,0,255));
```

Breaking this down:

```
0

↓

LED Number
```

```
0

↓

Red
```

```
0

↓

Green
```

```
255

↓

Blue
```

Result:

```
🔵 Blue
```

When the button is released,

the program executes:

```cpp
pixels.setPixelColor(0, pixels.Color(255,0,0));
```

This produces:

```
🔴 Red
```

---

# Printing Button Status

Each time the button changes state,

the program also prints:

```
Button Pressed
```

or

```
Button Released
```

to the Serial Monitor.

This allows us to observe what the ESP32 is detecting internally.

---

# Updating the LED

After selecting a colour,

the program executes:

```cpp
pixels.show();
```

Once again,

this sends the new RGB data to the NeoPixel.

Without this function,

the LED would never change colour.

---

# delay()

Finally,

the program pauses briefly.

```cpp
delay(100);
```

This means:

```
Wait

↓

100 milliseconds
```

The pause reduces unnecessary CPU usage and prevents the Serial Monitor from filling with thousands of messages every second.

For beginner projects,

delay() is perfectly acceptable.

Later, more advanced embedded applications often replace delay() with timers or non-blocking techniques.

---

# Complete Program Workflow

The entire program can now be visualized as:

```
Power ON

↓

setup()

↓

Initialize RGB

↓

Configure GPIO0

↓

Start loop()

↓

Read Button

↓

Pressed?

↓

YES

↓

Blue LED

↓

NO

↓

Red LED

↓

Update LED

↓

Repeat Forever
```

---

# Concepts Learned

This chapter introduced the practical side of embedded programming.

New concepts include:

- Libraries
- Constants
- Objects
- Functions
- setup()
- loop()
- Variables
- pinMode()
- digitalRead()
- if / else
- NeoPixel Functions
- Serial Communication
- Program Flow

Together, these concepts form the basic programming toolkit used in almost every Arduino and ESP32 project.

---

# What's Next?

In **Part 5**, we will conclude the project by covering:

- Testing the Project
- Expected Output
- Troubleshooting Common Issues
- Real-World Applications
- What We Learned
- Future Improvements
- Key Takeaways
- Final Conclusion

We will also connect everything learned in this chapter to larger embedded systems and IoT applications.

---

# Conclusion

Every line of code in this project serves a specific purpose. From initializing hardware and configuring GPIO pins to reading inputs and controlling outputs, the program demonstrates how software and electronics work together to create an interactive embedded system.

Although the source code is relatively short, it introduces the fundamental programming structures used in professional firmware development. Mastering these concepts provides a strong foundation for future work with sensors, displays, communication protocols, and IoT devices built on the ESP32 platform.

# ESP32-S3 Embedded Systems Journey – Day 04
# Chapter 05: Testing, Troubleshooting, Applications & Conclusion (Part 5)

![Platform](https://img.shields.io/badge/Platform-ESP32--S3-blue)
![Framework](https://img.shields.io/badge/Framework-Arduino_IDE-green)
![Language](https://img.shields.io/badge/Language-C++-orange)
![Project](https://img.shields.io/badge/Project-GPIO_Input_Output-success)
![Status](https://img.shields.io/badge/Status-Completed-brightgreen)

---

# Introduction

After understanding the electrical theory, learning how GPIO works internally, building our first GPIO application, and studying every line of code, the final step is verifying that everything works as expected.

Testing is one of the most important stages of embedded systems development.

Even if the code compiles successfully, it is still necessary to confirm that the hardware behaves correctly in real-world conditions.

This chapter documents the testing procedure, expected observations, common troubleshooting techniques, real-world applications, key concepts learned, and future improvements.

---

# Testing Environment

The following hardware and software were used during testing.

## Hardware

- OceanLabz ESP32-S3 DevKit
- USB Type-C Cable
- Built-in BOOT Button
- Built-in RGB NeoPixel LED

---

## Software

- Arduino IDE
- ESP32 Board Package
- Adafruit NeoPixel Library
- Serial Monitor (115200 Baud)

---

# Upload Procedure

The following steps were performed.

### Step 1

Connect the ESP32 using the USB Type-C cable.

---

### Step 2

Select the correct board inside Arduino IDE.

```
ESP32S3 Dev Module
```

---

### Step 3

Select the correct COM Port.

Example:

```
COM4
```

---

### Step 4

Verify the code.

```
✔ Compilation Successful
```

---

### Step 5

Upload the program.

```
Uploading...

↓

Done Uploading
```

---

### Step 6

Open the Serial Monitor.

```
Baud Rate

115200
```

---

# Expected Serial Output

Immediately after booting, the Serial Monitor displays:

```text
GPIO Input Demo Started
```

Since the BOOT button is not pressed,

the ESP32 continuously prints:

```text
Button Released

Button Released

Button Released
```

while the RGB LED remains:

```
🔴 RED
```

---

# Testing Button Press

Press and hold the BOOT button.

Expected Serial Output:

```text
Button Pressed

Button Pressed

Button Pressed
```

Expected LED:

```
🔵 BLUE
```

---

# Testing Button Release

Release the BOOT button.

Expected Serial Output:

```text
Button Released
```

Expected LED:

```
🔴 RED
```

---

# Complete Behaviour

The complete application should behave exactly as shown below.

| BOOT Button | GPIO0 | LED Colour |
|-------------|-------|------------|
| Released | HIGH | 🔴 Red |
| Pressed | LOW | 🔵 Blue |

---

# Complete Program Workflow

```
ESP32 Starts

↓

setup()

↓

RGB Initialized

↓

GPIO Configured

↓

loop()

↓

Read GPIO0

↓

LOW ?

↓

YES

↓

Blue LED

↓

NO

↓

Red LED

↓

Repeat Forever
```

---

# Common Beginner Mistakes

## Wrong GPIO Number

Using

```cpp
#define LED_PIN 2
```

instead of

```cpp
#define LED_PIN 48
```

means the onboard RGB LED will never respond because it is physically connected to GPIO48.

---

## Forgetting INPUT_PULLUP

Writing

```cpp
pinMode(BUTTON_PIN, INPUT);
```

instead of

```cpp
pinMode(BUTTON_PIN, INPUT_PULLUP);
```

can cause the GPIO input to float.

The button may randomly appear pressed even when it is not.

---

## Forgetting pixels.show()

Many beginners only change the colour:

```cpp
pixels.setPixelColor(...)
```

and expect the LED to update immediately.

However,

the colour remains stored inside memory until

```cpp
pixels.show();
```

is called.

Without it,

nothing changes on the LED.

---

## Incorrect Baud Rate

If the Serial Monitor shows random unreadable characters,

verify that the baud rate is:

```
115200
```

Both the program and Serial Monitor must use the same speed.

---

## Library Not Installed

If the compiler displays an error similar to:

```text
Adafruit_NeoPixel.h: No such file or directory
```

the NeoPixel library has not been installed.

Install it using:

```
Arduino IDE

↓

Library Manager

↓

Search

↓

Adafruit NeoPixel
```

---

# Troubleshooting Checklist

If the program does not work correctly, verify the following.

✅ Correct ESP32 board selected

✅ Correct COM port selected

✅ NeoPixel library installed

✅ LED_PIN = 48

✅ BUTTON_PIN = 0

✅ INPUT_PULLUP enabled

✅ Baud rate set to 115200

✅ Code uploaded successfully

---

# Skills Gained

This project introduced several new embedded systems concepts.

Hardware Skills

- Understanding GPIO
- Reading Digital Inputs
- Controlling Digital Outputs
- Understanding Pull-up Resistors
- Using Built-in Hardware

Programming Skills

- Variables
- Constants
- Functions
- Conditional Statements
- Program Flow
- Libraries
- Objects
- Serial Debugging

Engineering Skills

- Hardware Testing
- Debugging
- Problem Solving
- Firmware Development
- Understanding Hardware-Software Interaction

---

# Real-World Applications

Although the project only changes the colour of an RGB LED, the same logic exists in countless embedded systems.

Examples include:

## Smart Door Locks

```
Button

↓

GPIO

↓

Unlock Door
```

---

## Industrial Machines

```
Emergency Stop Button

↓

GPIO

↓

Shutdown Machine
```

---

## Smart Home Automation

```
Switch

↓

GPIO

↓

Turn ON Light
```

---

## Security Systems

```
Door Sensor

↓

GPIO

↓

Activate Alarm
```

---

## Medical Devices

```
Button Press

↓

GPIO

↓

Start Monitoring
```

---

## Automotive Systems

```
Brake Pedal

↓

GPIO

↓

Brake Lights
```

---

## Robotics

```
Limit Switch

↓

GPIO

↓

Stop Robot Movement
```

---

Every one of these systems follows the same fundamental principle learned in this project.

```
Input

↓

Decision

↓

Output
```

---

# Knowledge Progress

With this project completed, the embedded systems journey now includes:

✅ Arduino IDE Setup

✅ ESP32 Development Environment

✅ NeoPixel RGB LED Control

✅ Bluetooth Low Energy (BLE)

✅ GPIO Theory

✅ Digital Electronics

✅ GPIO Architecture

✅ Digital Input

✅ Digital Output

✅ BOOT Button

✅ Pull-up Resistors

✅ Event Driven Programming

---

# Future Learning Roadmap

The next projects will gradually introduce more advanced embedded concepts.

Upcoming topics include:

- Multiple GPIO Inputs
- External Push Buttons
- Analog Inputs
- Potentiometers
- PWM
- Servo Motors
- Temperature Sensors
- OLED Displays
- Wi-Fi Networking
- ESP32 Web Server
- REST APIs
- MQTT
- IoT Communication
- Embedded Security
- Secure Firmware Design

Every new topic will build upon the GPIO concepts learned in this chapter.

---

# Key Takeaways

Throughout this project, several important lessons became clear.

- GPIO allows the ESP32 to communicate with the physical world.
- Input pins receive information from external hardware.
- Output pins send signals to external devices.
- Digital electronics operates using HIGH and LOW logic levels.
- The BOOT button is connected to GPIO0.
- The onboard RGB LED is connected to GPIO48.
- INPUT_PULLUP prevents floating inputs.
- Software continuously monitors hardware inside the `loop()` function.
- Embedded systems react to events generated by users or sensors.
- Simple GPIO concepts form the foundation for complex IoT and embedded systems.

---

# Repository Structure

```
Day-04-GPIO/

│── README.md
│── GPIO_Button_Demo.ino
│── Images/
│     ├── gpio-architecture.png
│     ├── boot-button-flow.png
│     ├── circuit-diagram.png
│     ├── serial-monitor.png
│     ├── button-pressed.png
│     └── button-released.png
```

---

# Conclusion

This project represents an important milestone in the embedded systems learning journey.

For the first time, software running on the ESP32 responded directly to a physical action performed by the user. By combining the onboard BOOT button as a digital input with the onboard RGB NeoPixel LED as a digital output, we built a complete embedded control system using only the hardware available on the development board.

Beyond writing code, this project introduced the fundamental relationship between electricity, digital logic, GPIO architecture, firmware, and hardware interaction. Concepts such as HIGH and LOW logic levels, pull-up resistors, continuous program execution, and event-driven decision making form the core of nearly every embedded application.

Although the project itself is intentionally simple, the same programming model is used in industrial automation, robotics, medical devices, automotive electronics, consumer electronics, and Internet of Things (IoT) systems.

With a solid understanding of GPIO and digital input/output, the foundation has now been established for more advanced topics including analog sensors, PWM, communication protocols, networking, and secure IoT application development.

This concludes **Day 04 – GPIO Fundamentals**, providing both the theoretical knowledge and practical experience required for the next stage of the ESP32 embedded systems journey.
