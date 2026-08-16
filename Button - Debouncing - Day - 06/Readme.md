# Day 06 — Button Debouncing

> **ESP32-S3 Embedded Systems Journey**
> OceanLabz ESP32-S3 DevKit N16R8
> Arduino IDE + Arduino C++

---

## Table of Contents

1. [Introduction](#introduction)
2. [The Problem With Mechanical Buttons](#the-problem-with-mechanical-buttons)
3. [What Is Button Bounce?](#what-is-button-bounce)
4. [Why Bounce Is a Problem With Interrupts](#why-bounce-is-a-problem-with-interrupts)
5. [What Is Debouncing?](#what-is-debouncing)
6. [Our Debouncing Approach](#our-debouncing-approach)
7. [Understanding `millis()`](#understanding-millis)
8. [Understanding `debounceTime`](#understanding-debouncetime)
9. [Understanding `lastInterruptTime`](#understanding-lastinterrupttime)
10. [The Debounce Logic](#the-debounce-logic)
11. [Complete Project Code](#complete-project-code)
12. [Code Walkthrough](#code-walkthrough)
13. [What Happens During a Button Press](#what-happens-during-a-button-press)
14. [Testing Different Debounce Times](#testing-different-debounce-times)
15. [Why 20000 Is a Bad Practical Value](#why-20000-is-a-bad-practical-value)
16. [Polling vs Interrupt + Debouncing](#polling-vs-interrupt--debouncing)
17. [Important ISR Consideration](#important-isr-consideration)
18. [What We Learned](#what-we-learned)
19. [Real-World Applications](#real-world-applications)
20. [Next Step](#next-step)

---

# Introduction

In Day 05, we learned about **interrupts**.

Our ESP32-S3 was able to react to the BOOT button without continuously checking the button inside the main program.

The basic flow was:

```text
BOOT Button
     |
     v
GPIO0
     |
     v
Interrupt
     |
     v
ISR
     |
     v
ESP32 reacts
```

However, we discovered an important problem.

A real mechanical button does not always produce one perfectly clean electrical signal.

Instead, when pressed, it can rapidly switch between HIGH and LOW several times.

This is called **button bounce**.

Day 06 is about solving that problem.

Our goal is:

```text
ONE physical button press
          |
          v
ONE reliable software event
```

---

# The Problem With Mechanical Buttons

A button looks simple from the outside.

You press it:

```text
      👆
      |
   [ BUTTON ]
      |
      v
    GPIO0
```

We might expect the ESP32 to see:

```text
HIGH ─────────────────┐
                     |
                     └──────── LOW
```

One clean transition.

But real mechanical switches contain physical metal contacts.

When the contacts meet, they can physically move, vibrate, and briefly separate from each other.

This creates a noisy electrical signal.

---

# What Is Button Bounce?

Imagine pressing the button once.

You physically perform:

```text
ONE PRESS
```

But the electrical signal might look conceptually like:

```text
HIGH
 |
 |        ┌──┐
 |        │  │
 |        │  └──┐
 |        │     │
 |        └─┐   └──┐
 |          │      │
 |          └──────┴──────── LOW
 |
 +--------------------------------> Time
```

Instead of:

```text
HIGH → LOW
```

the ESP32 may briefly see:

```text
HIGH → LOW → HIGH → LOW → HIGH → LOW
```

all within a very short period.

The user pressed the button **once**.

But electrically, the ESP32 may see several transitions.

---

# A Simple Real-World Example

Imagine a doorbell.

You press the doorbell once:

```text
👆
```

You expect:

```text
DING!
```

But imagine the switch is bouncing and produces:

```text
DING!
DING!
DING!
DING!
```

You only pressed it once.

The button is effectively saying:

> "I was pressed! I was pressed! I was pressed!"

The ESP32 needs to be smart enough to understand:

> "Relax. That's just one press."

That is the purpose of **debouncing**.

---

# Why Bounce Is a Problem With Interrupts

This becomes especially important because we are using interrupts.

Our Day 05 architecture was:

```text
Button
   |
   v
GPIO0
   |
   v
Interrupt
   |
   v
ISR
   |
   v
Toggle LED
```

If the button bounces:

```text
Physical Press
      |
      v
LOW
HIGH
LOW
HIGH
LOW
      |
      v
Multiple Interrupt Events
```

The ESP32 could potentially execute the interrupt handler multiple times.

That means our LED might toggle multiple times.

For example:

```text
One physical press
        |
        v
Interrupt #1 → LED ON
Interrupt #2 → LED OFF
Interrupt #3 → LED ON
Interrupt #4 → LED OFF
```

The result could appear random or unreliable.

---

# What Is Debouncing?

**Debouncing** means filtering out those tiny unwanted changes caused by the physical button.

The goal is:

```text
Noisy Signal
     |
     v
Debounce Logic
     |
     v
Clean Software Event
```

Instead of allowing:

```text
LOW HIGH LOW HIGH LOW
```

to become several button presses, we want:

```text
LOW HIGH LOW HIGH LOW
          |
          v
     ONE real press
```

So the fundamental idea is:

> **One physical press should become one software event.**

---

# Our Debouncing Approach

For this project, we use **software debouncing based on time**.

The basic idea is:

```text
Button press
     |
     v
Accept the press
     |
     v
Start a timer
     |
     v
Ignore very quick additional signals
     |
     v
After enough time
     |
     v
Accept another press
```

We use:

```cpp
millis()
```

to measure time.

---

# Understanding `millis()`

The ESP32 needs a way to know how much time has passed.

Arduino provides:

```cpp
millis()
```

Think of `millis()` as the ESP32 asking:

> "How many milliseconds have passed since I started?"

For example:

```text
ESP32 starts
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

Therefore:

```text
200 milliseconds = 0.2 seconds
```

and:

```text
20,000 milliseconds = 20 seconds
```

---

# Understanding `debounceTime`

Our code contains:

```cpp
const unsigned long debounceTime = 200;
```

The important value here is:

```text
200
```

which means:

```text
200 milliseconds
```

We are telling the ESP32:

> "After accepting a button press, don't accept another one immediately. Wait 200 milliseconds."

Think of it as a small **cooldown period**.

```text
BUTTON PRESS
     |
     v
 ACCEPT
     |
     v
 200 ms cooldown
     |
     v
 Accept next valid press
```

---

# Understanding `lastInterruptTime`

We also need to remember **when the last valid interrupt happened**.

That's why we have:

```cpp
volatile unsigned long lastInterruptTime = 0;
```

Think of this variable as a small notebook.

The ESP32 can write:

```text
Last accepted button press:
15320 ms
```

Then, when another interrupt occurs, it can compare the current time against the previous time.

For example:

```text
Current time       = 15550 ms
Last press         = 15320 ms
```

The difference is:

```text
15550 - 15320 = 230 ms
```

Since:

```text
230 ms > 200 ms
```

we accept the new press.

---

# The Debounce Logic

The core of our project is:

```cpp
unsigned long currentTime = millis();

if (currentTime - lastInterruptTime > debounceTime)
{
    // Accept button press
}
```

Let's translate this into simple language.

The ESP32 asks:

> "What time is it now?"

Then:

> "How long has it been since the last button press?"

Then:

> "Has enough time passed?"

---

## Example 1 — Too Soon

Suppose:

```text
Current time       = 15320 ms
Last press         = 15300 ms
```

Difference:

```text
15320 - 15300 = 20 ms
```

Our debounce time is:

```text
200 ms
```

So:

```text
20 > 200
```

is false.

Therefore:

```text
❌ Ignore it
```

This could be button bounce.

---

## Example 2 — Long Enough

Suppose:

```text
Current time       = 15550 ms
Last press         = 15300 ms
```

Difference:

```text
15550 - 15300 = 250 ms
```

Now:

```text
250 > 200
```

is true.

Therefore:

```text
✅ Accept the button press
```

---

# Complete Project Code

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

// Button state
volatile bool buttonPressed = false;

// Debouncing variables
volatile unsigned long lastInterruptTime = 0;

const unsigned long debounceTime = 200;

void IRAM_ATTR buttonISR()
{
  unsigned long currentTime = millis();

  if (currentTime - lastInterruptTime > debounceTime)
  {
    buttonPressed = !buttonPressed;

    lastInterruptTime = currentTime;
  }
}

void setup()
{
  Serial.begin(115200);

  // Start RGB LED
  pixels.begin();
  pixels.clear();
  pixels.show();

  // Configure BOOT button
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Configure interrupt
  attachInterrupt(
    digitalPinToInterrupt(BUTTON_PIN),
    buttonISR,
    FALLING
  );

  // Initial LED state
  pixels.setPixelColor(
    0,
    pixels.Color(255, 0, 0)
  );

  pixels.show();

  Serial.println("Day 06 - Button Debouncing");
  Serial.println("Press BOOT button to toggle LED");
}

void loop()
{
  if (buttonPressed)
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

  delay(10);
}
```

---

# Code Walkthrough

## 1. Include the NeoPixel Library

```cpp
#include <Adafruit_NeoPixel.h>
```

This allows us to control the RGB LED.

Without the library, functions such as:

```cpp
pixels.setPixelColor()
pixels.show()
```

would not be available.

---

# 2. Define the LED Pin

```cpp
#define LED_PIN 48
```

Our onboard RGB NeoPixel is connected to:

```text
GPIO48
```

---

# 3. Define the Number of LEDs

```cpp
#define NUMPIXELS 1
```

We have one onboard RGB NeoPixel.

---

# 4. Define the Button Pin

```cpp
#define BUTTON_PIN 0
```

Our BOOT button is connected to:

```text
GPIO0
```

---

# 5. Create the NeoPixel Object

```cpp
Adafruit_NeoPixel pixels(
  NUMPIXELS,
  LED_PIN,
  NEO_GRB + NEO_KHZ800
);
```

This creates the object that allows our program to control the RGB LED.

---

# 6. Create the Button State

```cpp
volatile bool buttonPressed = false;
```

`bool` means the variable can contain only:

```text
true
false
```

We use this as our LED state.

```text
false → Red
true  → Blue
```

The word:

```cpp
volatile
```

is important because the variable can be changed by an interrupt.

---

# 7. Remember the Last Interrupt

```cpp
volatile unsigned long lastInterruptTime = 0;
```

This stores the time when the last accepted interrupt occurred.

Initially:

```text
lastInterruptTime = 0
```

As soon as we accept a button press, we update it.

---

# 8. Set the Debounce Time

```cpp
const unsigned long debounceTime = 200;
```

Our debounce period is:

```text
200 milliseconds
```

or:

```text
0.2 seconds
```

---

# 9. Interrupt Service Routine

```cpp
void IRAM_ATTR buttonISR()
```

This is our **Interrupt Service Routine**.

The ESP32 calls this function when the configured button interrupt occurs.

Our interrupt configuration is:

```cpp
attachInterrupt(
  digitalPinToInterrupt(BUTTON_PIN),
  buttonISR,
  FALLING
);
```

So the basic flow is:

```text
GPIO0
  |
  | Falling edge
  v
Interrupt
  |
  v
buttonISR()
```

---

# 10. Get the Current Time

Inside the ISR:

```cpp
unsigned long currentTime = millis();
```

The ESP32 asks:

> "What time is it?"

The answer is stored in:

```text
currentTime
```

---

# 11. Check the Debounce Period

```cpp
if (currentTime - lastInterruptTime > debounceTime)
```

This is the actual debounce filter.

The ESP32 calculates:

```text
Current time
      -
Last accepted press
      =
Time since last press
```

Then it checks whether that time is greater than:

```text
200 ms
```

---

# 12. Toggle the Button State

If enough time has passed:

```cpp
buttonPressed = !buttonPressed;
```

The `!` means **NOT**.

Therefore:

```text
false → true
true  → false
```

This gives us a toggle.

Like a physical light switch:

```text
OFF
 ↓
ON
 ↓
OFF
 ↓
ON
```

---

# 13. Save the Current Time

```cpp
lastInterruptTime = currentTime;
```

Now the ESP32 remembers:

> "This was the last accepted button press."

This is important because the next interrupt will compare its time against this value.

---

# 14. Configure the Button

Inside `setup()`:

```cpp
pinMode(BUTTON_PIN, INPUT_PULLUP);
```

The BOOT button uses the ESP32's internal pull-up configuration.

The simplified electrical behavior is:

```text
Released → HIGH

Pressed  → LOW
```

Therefore we use:

```cpp
FALLING
```

for the interrupt.

---

# 15. Configure the Interrupt

```cpp
attachInterrupt(
  digitalPinToInterrupt(BUTTON_PIN),
  buttonISR,
  FALLING
);
```

This tells the ESP32:

```text
"When GPIO0 changes from HIGH to LOW,
run buttonISR()."
```

---

# 16. Initial LED State

We set the initial color to red:

```cpp
pixels.setPixelColor(
  0,
  pixels.Color(255, 0, 0)
);
```

RGB values:

```text
Red   = 255
Green = 0
Blue  = 0
```

Therefore:

```text
RGB(255, 0, 0)
      ↓
     RED
```

---

# 17. The Main Loop

Our main loop checks:

```cpp
if (buttonPressed)
```

If the value is:

```text
true
```

we show blue.

```cpp
pixels.setPixelColor(
  0,
  pixels.Color(0, 0, 255)
);
```

If it is:

```text
false
```

we show red.

```cpp
pixels.setPixelColor(
  0,
  pixels.Color(255, 0, 0)
);
```

---

# What Happens During a Button Press?

Let's follow one complete press.

## Initial State

```text
buttonPressed = false

LED = RED
```

---

## User presses BOOT

```text
BOOT
 ↓
GPIO0
 ↓
HIGH → LOW
 ↓
FALLING EDGE
 ↓
INTERRUPT
```

---

## ISR Runs

```text
buttonISR()
```

The ESP32 checks:

```text
How long since the last accepted press?
```

Suppose:

```text
250 ms
```

has passed.

Our rule:

```text
200 ms
```

Therefore:

```text
250 > 200
```

So:

```text
✅ Accept
```

Then:

```cpp
buttonPressed = !buttonPressed;
```

Changes:

```text
false → true
```

The main loop sees:

```text
true
```

and changes the LED:

```text
RED → BLUE
```

---

# What Happens When the Button Bounces?

Suppose the button generates:

```text
LOW
HIGH
LOW
HIGH
LOW
```

The interrupt may trigger multiple times.

But our debounce logic checks the time.

For example:

```text
First interrupt
     |
     v
250 ms since previous
     |
     v
✅ Accept

Second interrupt
     |
     v
10 ms later
     |
     v
❌ Ignore

Third interrupt
     |
     v
15 ms later
     |
     v
❌ Ignore
```

The result:

```text
Several electrical events
          |
          v
One accepted button press
```

---

# Testing Different Debounce Times

One of the best ways to understand this concept is to experiment with the value.

---

## Test 1 — 20 ms

```cpp
const unsigned long debounceTime = 20;
```

This means:

```text
20 milliseconds
```

The ESP32 will only ignore very quick events.

---

## Test 2 — 200 ms

```cpp
const unsigned long debounceTime = 200;
```

This means:

```text
200 milliseconds
```

This makes the debounce behavior much easier to observe.

This is the value we used for the learning project.

---

## Test 3 — 20,000 ms

You experimented with:

```cpp
const unsigned long debounceTime = 20000;
```

Remember:

```text
1000 ms = 1 second

20000 ms = 20 seconds
```

So after one accepted press:

```text
PRESS
 ↓
✅ ACCEPT
 ↓
20 SECOND COOLDOWN
```

During those 20 seconds, another button press will be rejected by our condition.

Example:

```text
Time        Button       Result
--------------------------------------
0 sec       Press        ✅ Accepted
1 sec       Press        ❌ Ignored
5 sec       Press        ❌ Ignored
10 sec      Press        ❌ Ignored
19 sec      Press        ❌ Ignored
20+ sec     Press        ✅ Accepted
```

---

# Why 20000 Is a Bad Practical Value

Although:

```cpp
20000
```

is useful for experimenting, it is not a sensible debounce value for a normal mechanical button.

Why?

Because we don't just want to remove bounce.

We also want to accept legitimate button presses.

If the debounce period is too large:

```text
Real Press
   ↓
Accepted
   ↓
20-second lockout
```

Now the user has to wait 20 seconds before the next press can be recognized.

That is far too long for a normal button.

This teaches us an important engineering principle:

> **A larger debounce time does not automatically mean better debouncing.**

The debounce period needs to be appropriate for the hardware and application.

---

# Polling vs Interrupt + Debouncing

We have now learned two different approaches.

## Polling

The ESP32 repeatedly asks:

```text
Is the button pressed?
Is the button pressed?
Is the button pressed?
Is the button pressed?
```

This is:

```text
CPU
 |
 +--> Check button
 |
 +--> Check button
 |
 +--> Check button
 |
 +--> Check button
 |
 +--> Repeat
```

---

## Interrupt

With an interrupt:

```text
CPU running
     |
     v
Button event
     |
     v
Interrupt
     |
     v
ISR
```

The CPU doesn't need to continuously ask.

The hardware can notify the CPU.

---

## Interrupt + Debouncing

Now we have:

```text
Button
   |
   v
Physical Bounce
   |
   v
Interrupt
   |
   v
Debounce Filter
   |
   v
Valid Button Event
   |
   v
Application Logic
```

This is much closer to how real embedded systems are designed.

---

# Important ISR Consideration

There is an important lesson hidden in this project.

Our current learning implementation performs:

```cpp
millis()
```

inside the ISR.

For this beginner project, it helps demonstrate the concept clearly.

However, as we move toward professional ESP32 firmware, we need to be more careful about what we do inside an ISR.

A good general rule is:

> **Keep Interrupt Service Routines short and fast.**

An ISR should generally do only the minimum work necessary.

A more advanced architecture could look like:

```text
Button
   |
   v
Interrupt
   |
   v
Very Short ISR
   |
   v
Set a flag / record event
   |
   v
Main Program
   |
   v
Debouncing + Application Logic
```

We will improve our architecture as the project becomes more advanced.

---

# Button Debouncing in One Picture

The entire concept can be summarized as:

```text
             PHYSICAL WORLD
                    |
                    v
             Mechanical Button
                    |
                    v
                 BOUNCE
                    |
                    v
          HIGH / LOW / HIGH / LOW
                    |
                    v
                INTERRUPT
                    |
                    v
             DEBOUNCE LOGIC
                    |
          ┌─────────┴─────────┐
          |                   |
       Too Soon           Long Enough
          |                   |
          v                   v
       ❌ Ignore           ✅ Accept
                              |
                              v
                         Toggle LED
```

---

# What We Learned

During Day 06, we learned:

* What mechanical buttons are
* Why mechanical buttons bounce
* What button bounce looks like electrically
* Why bounce can cause multiple interrupts
* What software debouncing means
* How `millis()` measures elapsed time
* How `debounceTime` works
* How `lastInterruptTime` works
* How to compare elapsed time
* How to reject rapid unwanted events
* How to toggle a Boolean state
* How debouncing works with interrupts
* Why extremely large debounce values are impractical
* Why ISR functions should remain short
* The relationship between hardware behavior and software behavior

---

# Key Concepts to Remember

### Button Bounce

```text
One physical press
        ↓
Multiple tiny electrical transitions
```

### Debouncing

```text
Multiple electrical transitions
        ↓
One reliable software event
```

### `millis()`

```text
Tells us how much time has passed
```

### `debounceTime`

```text
Minimum time between accepted button events
```

### `lastInterruptTime`

```text
Stores when the last accepted event occurred
```

### Toggle

```text
false → true
true  → false
```

---

# The Bigger Embedded Systems Picture

We started with something that looks extremely simple:

```text
BUTTON → LED
```

But underneath it we discovered:

```text
Mechanical Hardware
        |
        v
Electrical Signal
        |
        v
GPIO
        |
        v
Interrupt
        |
        v
CPU
        |
        v
Software
        |
        v
Debouncing
        |
        v
Application Logic
        |
        v
LED
```

This is exactly why embedded systems are interesting.

A simple physical button can involve:

* Mechanical behavior
* Electrical signals
* GPIO hardware
* Interrupt controllers
* CPU execution
* Timing
* Software logic
* Firmware architecture

---

# Day 06 Final Architecture

Our current project can be represented as:

```text
                 ESP32-S3
                    |
          ┌─────────┴─────────┐
          |                   |
       GPIO0               GPIO48
          |                   |
          v                   v
      BOOT Button         RGB NeoPixel
          |
          v
       Interrupt
          |
          v
       buttonISR()
          |
          v
     Check elapsed time
          |
          v
       Debounce
          |
          v
     Toggle button state
          |
          v
       Main loop
          |
          v
       Update LED
```

---

# Day 05 vs Day 06

| Day    | Topic      | Main Problem                                                |
| ------ | ---------- | ----------------------------------------------------------- |
| Day 05 | Interrupts | Reacting to hardware events                                 |
| Day 06 | Debouncing | Preventing one physical press from becoming multiple events |

The progression is:

```text
GPIO
  ↓
Interrupt
  ↓
Interrupt + Debouncing
  ↓
Reliable Hardware Input
```

---

# Next Step — Day 07

Now that we understand:

```text
GPIO
Interrupts
Debouncing
```

the next important concept is:

# Timers & Non-Blocking Programming

We will learn why this:

```cpp
delay(1000);
```

can become a problem in real embedded systems.

We'll learn how the ESP32 can do:

```text
Task A
Task B
Task C
Task D
```

without sitting around doing nothing during a `delay()`.

The next major concept will be:

```text
millis()
     ↓
Non-Blocking Code
     ↓
Timers
     ↓
Multiple Tasks
```

And that will prepare us for:

```text
PWM
ADC
Sensors
UART
I2C
SPI
Wi-Fi
MQTT
FreeRTOS
Secure IoT Firmware
```

---

# Conclusion

Button debouncing looks like a small topic, but it teaches an important embedded-systems lesson:

> **The physical world is not always as clean as software expects it to be.**

A programmer may think:

```text
Press = ONE event
```

while the hardware may actually produce:

```text
Press = MANY tiny electrical transitions
```

Good firmware must account for the behavior of the physical hardware.

Our Day 06 project therefore takes:

```text
NOISY HARDWARE
      ↓
   INTERRUPT
      ↓
 DEBOUNCE LOGIC
      ↓
 CLEAN SOFTWARE EVENT
      ↓
   LED ACTION
```

This is another step toward understanding how real embedded and IoT devices work.

---

## Project Hardware

```text
Board:
OceanLabz ESP32-S3 DevKit N16R8

Microcontroller:
ESP32-S3

BOOT Button:
GPIO0

RGB NeoPixel:
GPIO48

Programming:
Arduino IDE

Language:
Arduino C++

RGB Library:
Adafruit NeoPixel

Serial:
115200 baud
```

---

## Day 06 Status

```text
[✓] Understand mechanical buttons
[✓] Understand button bounce
[✓] Understand interrupt-related bounce
[✓] Understand software debouncing
[✓] Understand millis()
[✓] Implement debounce timing
[✓] Test different debounce values
[✓] Understand why 20000 ms is excessive
[✓] Build interrupt + debounce project
[✓] Connect hardware behavior to firmware
```

**Next: Day 07 — Timers & Non-Blocking Programming**
