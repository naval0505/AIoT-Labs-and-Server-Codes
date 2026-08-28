# Day 07 — Timers & Non-Blocking Programming

> **ESP32-S3 Embedded Systems Journey**
> Board: OceanLabz ESP32-S3 DevKit N16R8
> Programming: Arduino IDE + Arduino C++

---

# Table of Contents

1. [Introduction](#introduction)
2. [The Problem With delay()](#the-problem-with-delay)
3. [What Does Blocking Mean?](#what-does-blocking-mean)
4. [What Is Non-Blocking Programming?](#what-is-non-blocking-programming)
5. [Understanding millis()](#understanding-millis)
6. [Our Day 07 Project](#our-day-07-project)
7. [Project Hardware](#project-hardware)
8. [How the Timer Works](#how-the-timer-works)
9. [Complete Code](#complete-code)
10. [Code Walkthrough](#code-walkthrough)
11. [Understanding previousTime](#understanding-previoustime)
12. [Understanding interval](#understanding-interval)
13. [Understanding the Button Logic](#understanding-the-button-logic)
14. [Understanding the Timer Logic](#understanding-the-timer-logic)
15. [How Multiple Tasks Work](#how-multiple-tasks-work)
16. [delay() vs millis()](#delay-vs-millis)
17. [Testing the Project](#testing-the-project)
18. [Important Design Improvement](#important-design-improvement)
19. [What We Learned](#what-we-learned)
20. [Real-World Applications](#real-world-applications)
21. [Next Step](#next-step)

---

# Introduction

In the previous days of this ESP32-S3 journey, we learned about:

```text
ESP32 Basics
     ↓
RGB LED
     ↓
BLE
     ↓
GPIO
     ↓
Interrupts
     ↓
Button Debouncing
```

Now we move to another extremely important embedded programming concept:

# Timers & Non-Blocking Programming

Until now, we have often used:

```cpp
delay();
```

For example:

```cpp
delay(1000);
```

This looks simple.

But there is an important question:

> What is the ESP32 doing during that 1-second delay?

The answer is simple:

```text
Waiting...
Waiting...
Waiting...
Waiting...
```

The program cannot continue past that line until the delay finishes.

For very simple projects, this is fine.

However, real embedded systems often need to handle multiple things.

For example:

```text
Read Button
     +
Read Sensor
     +
Update Display
     +
Check Network
     +
Control LED
     +
Send Data
```

We do not want the ESP32 to stop everything just because one task needs to wait.

This is where **non-blocking programming** becomes important.

---

# The Problem With `delay()`

Consider this code:

```cpp
void loop()
{
    // Turn LED ON

    delay(1000);

    // Turn LED OFF

    delay(1000);
}
```

The flow looks like this:

```text
Turn LED ON
     |
     v
delay(1000)
     |
     v
WAIT 1 SECOND
     |
     v
Turn LED OFF
     |
     v
delay(1000)
     |
     v
WAIT 1 SECOND
     |
     v
Repeat
```

During:

```cpp
delay(1000);
```

the program is waiting.

Conceptually:

```text
ESP32
  |
  v
Do Task
  |
  v
STOP HERE
  |
  v
Wait...
Wait...
Wait...
  |
  v
Continue
```

This is called **blocking**.

---

# What Does Blocking Mean?

Blocking means that one part of the program stops the program from continuing normally.

Imagine you are doing three things:

```text
Task A → Read a button
Task B → Check a timer
Task C → Control an LED
```

Now imagine Task B says:

> "Nobody can do anything for 10 seconds!"

The result becomes:

```text
Task A
  |
  v
Wait for Task B
  |
  v
Task B
  |
  v
WAIT 10 SECONDS
  |
  v
Now continue
```

Everything is forced to wait.

That can become a problem in embedded systems.

---

# A Simple Real-World Example

Imagine you are cooking.

You need to:

```text
1. Watch the stove
2. Check the oven
3. Answer the door
```

Now imagine you put something in the oven and say:

> "I will stand here for 30 minutes and do absolutely nothing."

That would be inefficient.

Instead, you can say:

```text
Check oven timer
      |
      v
Not ready yet?
      |
      v
Do something else
      |
      v
Check again later
```

This is similar to non-blocking programming.

The ESP32 checks:

> "Has enough time passed?"

If the answer is:

```text
NO
```

it continues doing other work.

If the answer is:

```text
YES
```

it performs the timed task.

---

# What Is Non-Blocking Programming?

Non-blocking programming means:

> Do not stop the entire program while waiting for something.

Instead of:

```text
Do Task
   |
   v
WAIT
WAIT
WAIT
   |
   v
Continue
```

we do:

```text
Check Task
   |
   v
Ready?
  / \
NO   YES
|     |
v     v
Do    Perform
Other Task
Work
```

The program keeps running.

---

# Understanding `millis()`

Arduino provides:

```cpp
millis()
```

This function tells us approximately how many milliseconds have passed since the ESP32 started.

For example:

```text
ESP32 Starts
     |
     v
0 ms
     |
     v
100 ms
     |
     v
500 ms
     |
     v
1000 ms
     |
     v
5000 ms
```

Remember:

```text
1000 milliseconds = 1 second
```

So:

```text
1000 ms = 1 second
2000 ms = 2 seconds
5000 ms = 5 seconds
```

We can use this information to create timers without using `delay()`.

---

# Our Day 07 Project

Our project uses:

```text
ESP32-S3
   |
   ├── GPIO48
   |      |
   |      v
   |   RGB NeoPixel
   |
   └── GPIO0
          |
          v
      BOOT Button
```

The program will:

```text
1. Continuously check the BOOT button

2. Continuously check the timer

3. Change the RGB LED when the timer interval is reached

4. Repeat immediately
```

The important point is:

> There is no `delay()` controlling the timer.

---

# Project Hardware

## Board

```text
OceanLabz ESP32-S3 DevKit N16R8
```

## Microcontroller

```text
ESP32-S3
```

## BOOT Button

```text
GPIO0
```

## Onboard RGB NeoPixel

```text
GPIO48
```

## Programming Environment

```text
Arduino IDE
Arduino C++
```

## RGB Library

```text
Adafruit NeoPixel
```

## Serial Monitor

```text
115200 baud
```

---

# How the Timer Works

The basic timer logic is:

```text
Remember previous time
        |
        v
Check current time
        |
        v
Calculate difference
        |
        v
Has enough time passed?
        |
      /   \
    NO     YES
    |       |
    v       v
Continue  Perform Task
            |
            v
       Save new time
```

In our project:

```cpp
unsigned long previousTime = 0;
const unsigned long interval = 1000;
```

This means:

```text
previousTime
     |
     v
Remembers when the timer last performed its task
```

And:

```text
interval = 1000 ms
```

means:

```text
Perform the timed task every 1 second
```

---

# Complete Code

```cpp
#include <Adafruit_NeoPixel.h>

#define LED_PIN 48
#define NUMPIXELS 1
#define BUTTON_PIN 0

Adafruit_NeoPixel pixels(
  NUMPIXELS,
  LED_PIN,
  NEO_GRB + NEO_KHZ800
);

// Timer variables
unsigned long previousTime = 0;
const unsigned long interval = 1000;

// LED state
bool ledState = false;

// Button variables
bool lastButtonState = HIGH;
unsigned long lastButtonTime = 0;
const unsigned long debounceTime = 200;

void setup()
{
  Serial.begin(115200);

  // Start the RGB LED
  pixels.begin();
  pixels.clear();
  pixels.show();

  // Configure BOOT button
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.println("Day 07 - Timers and Non-Blocking Programming");
}

void loop()
{
  // Get the current time
  unsigned long currentTime = millis();

  // -------------------------
  // BUTTON TASK
  // -------------------------

  bool currentButtonState = digitalRead(BUTTON_PIN);

  if (currentButtonState == LOW &&
      lastButtonState == HIGH &&
      currentTime - lastButtonTime > debounceTime)
  {
    ledState = !ledState;

    lastButtonTime = currentTime;

    Serial.println("Button Pressed");
  }

  lastButtonState = currentButtonState;

  // -------------------------
  // TIMER TASK
  // -------------------------

  if (currentTime - previousTime >= interval)
  {
    previousTime = currentTime;

    if (ledState)
    {
      // Blue
      pixels.setPixelColor(
        0,
        pixels.Color(0, 0, 255)
      );
    }
    else
    {
      // Red
      pixels.setPixelColor(
        0,
        pixels.Color(255, 0, 0)
      );
    }

    pixels.show();

    ledState = !ledState;
  }
}
```

---

# Code Walkthrough

We will now understand the important parts of the code.

---

# Understanding `previousTime`

```cpp
unsigned long previousTime = 0;
```

Think of this variable as the ESP32's memory.

It remembers:

> "When was the last time I performed my timer task?"

At the beginning:

```text
previousTime = 0
```

Suppose the ESP32 starts.

The timer checks the current time.

Eventually:

```text
currentTime = 1000
```

Now:

```text
1000 - 0 = 1000
```

Our interval is:

```text
1000
```

Therefore the timer task runs.

Then:

```cpp
previousTime = currentTime;
```

So now:

```text
previousTime = 1000
```

The ESP32 remembers:

> "I last performed this task at 1000 ms."

---

# Understanding `interval`

Our code contains:

```cpp
const unsigned long interval = 1000;
```

This means:

```text
1000 milliseconds
```

which equals:

```text
1 second
```

We are telling the timer:

> "Perform this task once every second."

We could change it.

For example:

```cpp
const unsigned long interval = 500;
```

means:

```text
500 ms
```

or:

```text
0.5 seconds
```

While:

```cpp
const unsigned long interval = 5000;
```

means:

```text
5 seconds
```

---

# Understanding `currentTime`

Inside `loop()`:

```cpp
unsigned long currentTime = millis();
```

The ESP32 asks:

> "How many milliseconds have passed since I started?"

The answer is stored in:

```text
currentTime
```

For example:

```text
currentTime = 3521
```

This means approximately:

```text
3.521 seconds
```

have passed.

---

# Understanding the Timer Condition

The main timer condition is:

```cpp
if (currentTime - previousTime >= interval)
```

This means:

> "Has the required amount of time passed?"

The ESP32 calculates:

```text
Current Time
      -
Previous Time
      =
Elapsed Time
```

Then compares it with:

```text
interval
```

---

## Example

Suppose:

```text
currentTime = 1500
previousTime = 1000
```

Then:

```text
1500 - 1000 = 500
```

Our interval is:

```text
1000
```

So:

```text
500 >= 1000
```

is:

```text
FALSE
```

The timer does nothing.

But the program does not stop.

It continues running.

---

## Another Example

Suppose:

```text
currentTime = 2050
previousTime = 1000
```

Then:

```text
2050 - 1000 = 1050
```

Now:

```text
1050 >= 1000
```

is:

```text
TRUE
```

Therefore the timer performs its task.

---

# Save the New Time

After the timer task runs:

```cpp
previousTime = currentTime;
```

This updates our timer memory.

For example:

```text
Before:

previousTime = 1000
```

Timer runs at:

```text
currentTime = 2050
```

Then:

```text
previousTime = 2050
```

Now the ESP32 waits until another interval has passed.

---

# Understanding the Button Logic

Our button uses:

```cpp
bool currentButtonState = digitalRead(BUTTON_PIN);
```

This reads GPIO0.

Because we use:

```cpp
INPUT_PULLUP
```

the BOOT button behaves like:

```text
Released → HIGH

Pressed  → LOW
```

---

# Detecting a New Button Press

Our condition is:

```cpp
if (currentButtonState == LOW &&
    lastButtonState == HIGH &&
    currentTime - lastButtonTime > debounceTime)
```

This checks three things.

---

## Condition 1

```cpp
currentButtonState == LOW
```

This means:

> The button is currently pressed.

---

## Condition 2

```cpp
lastButtonState == HIGH
```

This means:

> The button was previously released.

Together:

```text
Previously HIGH
       |
       v
Currently LOW
```

This means:

```text
HIGH → LOW
```

So we are detecting a new button press.

---

## Condition 3

```cpp
currentTime - lastButtonTime > debounceTime
```

This is our debounce protection from Day 06.

The ESP32 asks:

> "Has enough time passed since the last accepted press?"

If yes:

```text
Accept the button press
```

If no:

```text
Ignore it
```

---

# Toggle the State

When a valid press is detected:

```cpp
ledState = !ledState;
```

The `!` means:

```text
NOT
```

Therefore:

```text
false → true
true  → false
```

This creates a toggle.

---

# Remember the Button Time

After accepting a press:

```cpp
lastButtonTime = currentTime;
```

The ESP32 remembers when that button event occurred.

This helps the debounce logic reject very fast unwanted events.

---

# Remember the Button State

At the end of the button section:

```cpp
lastButtonState = currentButtonState;
```

This is important.

The ESP32 remembers what the button looked like during this loop.

On the next loop, it can compare:

```text
Previous State
      |
      v
Current State
```

This allows the program to detect transitions such as:

```text
HIGH → LOW
```

instead of continuously treating a held button as many separate presses.

---

# Understanding the Timer LED Logic

When the timer interval is reached:

```cpp
if (ledState)
```

the program checks whether:

```text
ledState = true
```

If true, the LED becomes blue:

```cpp
pixels.setPixelColor(
  0,
  pixels.Color(0, 0, 255)
);
```

RGB:

```text
Red   = 0
Green = 0
Blue  = 255
```

Result:

```text
BLUE
```

Otherwise:

```text
ledState = false
```

and the LED becomes red:

```cpp
pixels.setPixelColor(
  0,
  pixels.Color(255, 0, 0)
);
```

Result:

```text
RED
```

---

# Update the LED

After selecting the color:

```cpp
pixels.show();
```

This sends the new color information to the physical NeoPixel.

Without:

```cpp
pixels.show();
```

the selected color may not appear on the LED.

---

# How Multiple Tasks Work

This is the most important part of Day 07.

The `loop()` function repeats extremely quickly.

Conceptually:

```text
LOOP
 |
 ├── Check current time
 |
 ├── Check button
 |
 ├── Check debounce time
 |
 ├── Check timer
 |
 ├── Update LED if needed
 |
 └── Repeat immediately
```

Then again:

```text
LOOP
 |
 ├── Check current time
 |
 ├── Check button
 |
 ├── Check timer
 |
 └── Repeat
```

And again:

```text
LOOP
 |
 ├── Check button
 |
 ├── Check timer
 |
 └── Repeat
```

There is no:

```cpp
delay(1000);
```

forcing the program to stop.

Instead:

```text
Check timer
     |
     v
Not ready?
     |
     v
Continue immediately
```

When the time is ready:

```text
Check timer
     |
     v
Ready?
     |
     v
Perform timer task
     |
     v
Continue immediately
```

This is the core idea of non-blocking programming.

---

# `delay()` vs `millis()`

## Using `delay()`

```cpp
Turn LED ON;

delay(1000);

Turn LED OFF;
```

Conceptually:

```text
Task
 |
 v
WAIT
 |
 v
WAIT
 |
 v
WAIT
 |
 v
Continue
```

The program is blocked during the delay.

---

## Using `millis()`

```text
Check Time
    |
    v
Has 1 second passed?
   / \
 NO   YES
 |      |
 v      v
Continue Do Task
```

The ESP32 does not need to stop and wait.

It simply checks whether the task is ready.

---

# Why This Is Important

Real embedded systems often need to do many things.

For example:

```text
ESP32
 |
 ├── Read Button
 |
 ├── Read Temperature Sensor
 |
 ├── Check Wi-Fi
 |
 ├── Send MQTT Data
 |
 ├── Update LED
 |
 └── Check Timers
```

If one task contains a large blocking delay:

```text
One Task
    |
    v
delay(10000)
    |
    v
10 Seconds of Waiting
```

other parts of the program may not be handled as expected.

Using timers allows the program to structure work like:

```text
Check Task A
      |
Check Task B
      |
Check Task C
      |
Check Timer
      |
Repeat
```

This becomes increasingly important as projects become more complex.

---

# Testing the Project

Upload the code to the ESP32-S3.

Open the Serial Monitor:

```text
Baud Rate: 115200
```

You should see:

```text
Day 07 - Timers and Non-Blocking Programming
```

When the BOOT button is pressed successfully:

```text
Button Pressed
```

should appear.

The program should continuously:

```text
Check Button
     +
Check Timer
     +
Update LED when required
```

without using a blocking timer delay.

---

# Important Design Improvement

The current project is intentionally a learning implementation.

Notice that:

```cpp
bool ledState = false;
```

is involved in both:

```text
Button logic
```

and:

```text
Timer logic
```

The button changes:

```cpp
ledState = !ledState;
```

and the timer also changes:

```cpp
ledState = !ledState;
```

This helps us see how state changes can affect program behavior.

However, as firmware becomes more complex, it is usually better to give different tasks their own clearly defined state.

For example:

```text
Button State
Timer State
LED State
Application Mode
```

could be separated.

A cleaner architecture could look like:

```text
Button Task
    |
    v
Button State

Timer Task
    |
    v
Timer State

Application Logic
    |
    v
LED State
```

This is an important lesson:

> As embedded programs become larger, separating responsibilities makes the firmware easier to understand and maintain.

We will improve our design as we continue learning more advanced embedded concepts.

---

# What We Learned

During Day 07, we learned:

* What `delay()` does
* What blocking means
* Why blocking can become a problem
* What non-blocking programming means
* How `millis()` works
* How to create a software timer
* What `previousTime` does
* What `interval` does
* How to calculate elapsed time
* How to perform a task at timed intervals
* How to continuously check multiple tasks
* How to detect a new button press
* How to combine button debouncing with timers
* How to avoid blocking delays
* Why separating program state becomes important

---

# Key Concepts to Remember

## Blocking

```text
Program stops and waits
```

Example:

```cpp
delay(1000);
```

---

## Non-Blocking

```text
Program checks whether something is ready
and continues running
```

Example:

```cpp
if (currentTime - previousTime >= interval)
```

---

## `millis()`

```text
Time since ESP32 started
```

---

## `previousTime`

```text
Remembers when a timed task last occurred
```

---

## `interval`

```text
How long to wait before performing the task again
```

---

# Day 07 in One Picture

```text
                  ESP32-S3
                      |
                      v
                 loop() runs
                      |
          ┌───────────┴───────────┐
          |                       |
          v                       v
     Check Button             Check Timer
          |                       |
          v                       v
     Button Pressed?       Time Reached?
          |                       |
        YES/NO                  YES/NO
          |                       |
          v                       v
      Handle Event           Perform Task
          |                       |
          └───────────┬───────────┘
                      |
                      v
                 Repeat Fast
```

The important point is:

> The ESP32 keeps checking instead of stopping and waiting.

---

# The Bigger Picture

We started with:

```text
GPIO
```

Then:

```text
GPIO
  ↓
Interrupts
```

Then:

```text
GPIO
  ↓
Interrupts
  ↓
Debouncing
```

Now:

```text
GPIO
  ↓
Interrupts
  ↓
Debouncing
  ↓
Timers
  ↓
Non-Blocking Programming
```

We are gradually building the foundations needed for more advanced embedded systems.

Eventually, the ESP32 will need to handle:

```text
Sensors
    +
Timers
    +
Communication
    +
Wi-Fi
    +
BLE
    +
MQTT
    +
Security
```

Understanding non-blocking programming is an important step toward that.

---

# Real-World Applications

Timers and non-blocking programming are used in many embedded applications.

Examples include:

```text
Sensor Reading
```

Read a sensor every 5 seconds without stopping the entire system.

```text
IoT Devices
```

Check Wi-Fi and send data while still monitoring sensors.

```text
Smart Home Devices
```

Control lights while continuously listening for user input.

```text
Security Systems
```

Monitor sensors, timers, alarms, and communication at the same time.

```text
Robotics
```

Read sensors and control motors without freezing the entire program.

```text
Embedded Cybersecurity
```

A device may need to monitor security events while continuing normal firmware operations.

---

# Day 07 Final Architecture

Our current project can be represented as:

```text
                    ESP32-S3
                        |
             ┌──────────┴──────────┐
             |                     |
          GPIO0                  GPIO48
             |                     |
             v                     v
       BOOT Button            RGB NeoPixel
             |
             v
      digitalRead()
             |
             v
       Button Logic
             |
             v
       Debounce Check
             |
             |
             ├─────────────────┐
             |                 |
             v                 v
        loop() running    millis() Timer
                                  |
                                  v
                           Time Comparison
                                  |
                                  v
                            Timer Event
                                  |
                                  v
                              Update LED
```

---

# Day 06 vs Day 07

| Day    | Topic                             | Main Goal                                              |
| ------ | --------------------------------- | ------------------------------------------------------ |
| Day 06 | Button Debouncing                 | Convert one physical press into one reliable event     |
| Day 07 | Timers & Non-Blocking Programming | Perform timed tasks without stopping the whole program |

The progression is:

```text
Reliable Input
      |
      v
Reliable Events
      |
      v
Reliable Timing
      |
      v
More Complex Embedded Systems
```

---

# Next Step

The next topic in the roadmap is:

# Day 08 — PWM

PWM means:

```text
Pulse Width Modulation
```

We will move from simple:

```text
LED ON
LED OFF
```

to controlling output levels.

For example:

```text
Low Brightness
     ↓
Medium Brightness
     ↓
High Brightness
```

We will learn how digital signals can be used to control things such as:

* LED brightness
* Motor speed
* Signal generation
* Hardware control

The roadmap continues:

```text
Day 07
Timers & Non-Blocking Programming
        |
        v
Day 08
PWM
        |
        v
Day 09
ADC / Analog Inputs
        |
        v
Day 10
Sensors
        |
        v
Day 11
UART
        |
        v
Day 12
I2C
        |
        v
Day 13
SPI
        |
        v
Wi-Fi
        |
        v
MQTT / Web Server
        |
        v
FreeRTOS
        |
        v
Secure IoT
        |
        v
Embedded Cybersecurity
```

---

# Conclusion

Day 07 introduced an important change in how we think about embedded programs.

Instead of writing:

```text
Do Something
     |
     v
Wait
     |
     v
Do Next Thing
```

we can write programs that behave more like:

```text
Check Button
     |
Check Sensor
     |
Check Timer
     |
Update Hardware
     |
Repeat
```

The ESP32 does not need to stop just because a timed event has not happened yet.

It can continue checking and handling other tasks.

The core lesson is:

> **Do not stop the entire program while waiting if the program can continue doing useful work.**

This concept will become increasingly important as we move toward sensors, networking, multitasking, FreeRTOS, and more advanced IoT firmware.

---

## Day 07 Status

```text
[✓] Understand delay()
[✓] Understand blocking
[✓] Understand non-blocking programming
[✓] Understand millis()
[✓] Create a software timer
[✓] Understand previousTime
[✓] Understand interval
[✓] Calculate elapsed time
[✓] Check a button without blocking
[✓] Combine button logic with timer logic
[✓] Understand state changes
[✓] Begin thinking about cleaner firmware architecture
```

**Completed: Day 07 — Timers & Non-Blocking Programming**

**Next: Day 08 — PWM**
