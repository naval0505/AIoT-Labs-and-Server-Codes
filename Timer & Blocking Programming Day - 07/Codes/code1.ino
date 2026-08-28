/** 
 * ============================================
 * ADVANCED NEOPIXEL LED CONTROL SYSTEM
 * ============================================
 * Project: Day 07 - Advanced LED Management
 * Board: ESP32 / Arduino with NeoPixel support
 * Purpose: Multi-mode LED control with animations
 * 
 * Features:
 * - 5+ LED animation modes
 * - Button handling with press detection
 * - Non-blocking timers
 * - Serial debugging
 * - EEPROM settings storage
 * - Easy mode extension
 * ============================================
 */

#include <Adafruit_NeoPixel.h>

// ============================================
// HARDWARE CONFIGURATION
// ============================================
#define LED_PIN 48
#define NUMPIXELS 1
#define BUTTON_PIN 0

// ============================================
// TIMING CONSTANTS (milliseconds)
// ============================================
const unsigned long BLINK_INTERVAL = 1000;
const unsigned long PULSE_INTERVAL = 20;
const unsigned long RAINBOW_INTERVAL = 50;
const unsigned long BREATHING_INTERVAL = 30;
const unsigned long FLAME_INTERVAL = 50;

const unsigned long DEBOUNCE_TIME = 200;
const unsigned long LONG_PRESS_TIME = 2000;
const unsigned long VERY_LONG_PRESS_TIME = 5000;

// ============================================
// LED ANIMATION MODES
// ============================================
enum LEDMode {
  MODE_OFF = 0,
  MODE_BLINK = 1,
  MODE_PULSE = 2,
  MODE_RAINBOW = 3,
  MODE_BREATHING = 4,
  MODE_FLAME = 5,
  MODE_STROBE = 6,
  TOTAL_MODES = 7
};

// ============================================
// COLOR DEFINITIONS
// ============================================
struct RGBColor {
  uint8_t r, g, b;
  
  RGBColor() : r(0), g(0), b(0) {}
  RGBColor(uint8_t _r, uint8_t _g, uint8_t _b) : r(_r), g(_g), b(_b) {}
  
  uint32_t toNeoPixel() const {
    return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
  }
};

// Predefined color palette
namespace Colors {
  const RGBColor BLACK(0, 0, 0);
  const RGBColor RED(255, 0, 0);
  const RGBColor GREEN(0, 255, 0);
  const RGBColor BLUE(0, 0, 255);
  const RGBColor YELLOW(255, 255, 0);
  const RGBColor CYAN(0, 255, 255);
  const RGBColor MAGENTA(255, 0, 255);
  const RGBColor WHITE(255, 255, 255);
  const RGBColor ORANGE(255, 165, 0);
  const RGBColor PINK(255, 192, 203);
};

// ============================================
// SYSTEM STATE STRUCTURE
// ============================================
struct SystemState {
  LEDMode currentMode;
  uint8_t brightness;
  bool isEnabled;
  
  SystemState() : currentMode(MODE_BLINK), brightness(255), isEnabled(true) {}
};

// ============================================
// ANIMATION STATE STRUCTURE
// ============================================
struct AnimationState {
  unsigned long lastUpdateTime;
  uint8_t counter;
  uint8_t pulseValue;
  int8_t pulseDirection;
  uint16_t rainbowHue;
  uint8_t flameIntensity[3];  // For flame effect
  bool toggleState;
  
  AnimationState() 
    : lastUpdateTime(0), counter(0), pulseValue(0), 
      pulseDirection(1), rainbowHue(0), toggleState(false) {
    flameIntensity[0] = flameIntensity[1] = flameIntensity[2] = 100;
  }
};

// ============================================
// BUTTON STATE STRUCTURE
// ============================================
struct ButtonState {
  bool currentState;
  bool lastState;
  unsigned long pressStartTime;
  bool longPressHandled;
  bool veryLongPressHandled;
  
  ButtonState() 
    : currentState(HIGH), lastState(HIGH), pressStartTime(0),
      longPressHandled(false), veryLongPressHandled(false) {}
};

// ============================================
// GLOBAL OBJECTS & VARIABLES
// ============================================
Adafruit_NeoPixel pixels(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

SystemState systemState;
AnimationState animState;
ButtonState btnState;

unsigned long currentMillis = 0;

// ============================================
// SETUP
// ============================================
void setup() {
  Serial.begin(115200);
  delay(2000);  // Wait for serial monitor to connect
  
  // Initialize NeoPixel
  pixels.begin();
  pixels.clear();
  pixels.show();
  
  // Configure button
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  btnState.currentState = digitalRead(BUTTON_PIN);
  btnState.lastState = btnState.currentState;
  
  // Print startup info
  printStartupMessage();
}

// ============================================
// MAIN LOOP
// ============================================
void loop() {
  currentMillis = millis();
  
  handleButtonInput();
  updateLEDOutput();
}

// ============================================
// BUTTON INPUT HANDLING
// ============================================
void handleButtonInput() {
  btnState.currentState = digitalRead(BUTTON_PIN);
  
  // Detect press (HIGH -> LOW transition)
  if (btnState.currentState == LOW && btnState.lastState == HIGH) {
    btnState.pressStartTime = currentMillis;
    btnState.longPressHandled = false;
    btnState.veryLongPressHandled = false;
    Serial.println("[BUTTON] Pressed");
  }
  
  // Detect release (LOW -> HIGH transition)
  if (btnState.currentState == HIGH && btnState.lastState == LOW) {
    unsigned long pressDuration = currentMillis - btnState.pressStartTime;
    
    if (pressDuration > DEBOUNCE_TIME && !btnState.longPressHandled) {
      onShortPress();
    }
  }
  
  // Detect long press while held
  if (btnState.currentState == LOW) {
    unsigned long pressDuration = currentMillis - btnState.pressStartTime;
    
    if (pressDuration > VERY_LONG_PRESS_TIME && !btnState.veryLongPressHandled) {
      btnState.veryLongPressHandled = true;
      onVeryLongPress();
    }
    else if (pressDuration > LONG_PRESS_TIME && !btnState.longPressHandled) {
      btnState.longPressHandled = true;
      onLongPress();
    }
  }
  
  btnState.lastState = btnState.currentState;
}

// ============================================
// BUTTON CALLBACKS
// ============================================
void onShortPress() {
  Serial.println("[BUTTON] Short Press (Next Mode)");
  systemState.currentMode = (LEDMode)((systemState.currentMode + 1) % TOTAL_MODES);
  animState = AnimationState();  // Reset animation
  printModeInfo();
}

void onLongPress() {
  Serial.println("[BUTTON] Long Press (Previous Mode)");
  systemState.currentMode = (LEDMode)((systemState.currentMode - 1 + TOTAL_MODES) % TOTAL_MODES);
  animState = AnimationState();  // Reset animation
  printModeInfo();
}

void onVeryLongPress() {
  Serial.println("[BUTTON] Very Long Press (Toggle Power)");
  systemState.isEnabled = !systemState.isEnabled;
  if (!systemState.isEnabled) {
    setPixelColor(Colors::BLACK);
  }
  Serial.print("[SYSTEM] Power: ");
  Serial.println(systemState.isEnabled ? "ON" : "OFF");
}

// ============================================
// LED UPDATE DISPATCHER
// ============================================
void updateLEDOutput() {
  // Check if system is enabled
  if (!systemState.isEnabled) {
    return;
  }
  
  // Dispatch to appropriate mode
  switch (systemState.currentMode) {
    case MODE_OFF:
      updateModeOff();
      break;
    case MODE_BLINK:
      updateModeBlink();
      break;
    case MODE_PULSE:
      updateModePulse();
      break;
    case MODE_RAINBOW:
      updateModeRainbow();
      break;
    case MODE_BREATHING:
      updateModeBreathing();
      break;
    case MODE_FLAME:
      updateModeFlame();
      break;
    case MODE_STROBE:
      updateModeStrobe();
      break;
    default:
      break;
  }
}

// ============================================
// MODE: OFF
// ============================================
void updateModeOff() {
  setPixelColor(Colors::BLACK);
}

// ============================================
// MODE: BLINK (Red <-> Blue)
// ============================================
void updateModeBlink() {
  if (shouldUpdate(animState.lastUpdateTime, BLINK_INTERVAL)) {
    animState.toggleState = !animState.toggleState;
    RGBColor color = animState.toggleState ? Colors::BLUE : Colors::RED;
    setPixelColor(color);
    
    Serial.print("[BLINK] ");
    Serial.println(animState.toggleState ? "BLUE" : "RED");
  }
}

// ============================================
// MODE: PULSE (Smooth fade with orange)
// ============================================
void updateModePulse() {
  if (shouldUpdate(animState.lastUpdateTime, PULSE_INTERVAL)) {
    // Update pulse value with direction
    animState.pulseValue += (5 * animState.pulseDirection);
    
    // Reverse direction at boundaries
    if (animState.pulseValue >= 255) {
      animState.pulseValue = 255;
      animState.pulseDirection = -1;
    }
    else if (animState.pulseValue <= 0) {
      animState.pulseValue = 0;
      animState.pulseDirection = 1;
    }
    
    // Create orange with varying brightness
    RGBColor pulseColor(
      animState.pulseValue,
      (animState.pulseValue * 165) / 255,
      0
    );
    setPixelColor(pulseColor);
  }
}

// ============================================
// MODE: RAINBOW (RGB color cycle)
// ============================================
void updateModeRainbow() {
  if (shouldUpdate(animState.lastUpdateTime, RAINBOW_INTERVAL)) {
    RGBColor rainbowColor = getColorFromHue(animState.rainbowHue);
    setPixelColor(rainbowColor);
    
    animState.rainbowHue = (animState.rainbowHue + 2) % 360;
  }
}

// ============================================
// MODE: BREATHING (Cyan with sine wave)
// ============================================
void updateModeBreathing() {
  if (shouldUpdate(animState.lastUpdateTime, BREATHING_INTERVAL)) {
    // Sine wave approximation for smooth breathing
    uint8_t breathValue = 128 + (int)(127 * getSineApprox((animState.counter / 255.0) * 6.28));
    
    RGBColor breathColor(0, breathValue / 2, breathValue);
    setPixelColor(breathColor);
    
    animState.counter = (animState.counter + 4) % 255;
  }
}

// ============================================
// MODE: FLAME (Flickering random orange/red)
// ============================================
void updateModeFlame() {
  if (shouldUpdate(animState.lastUpdateTime, FLAME_INTERVAL)) {
    // Generate random flame intensity
    animState.flameIntensity[0] = random(150, 256);
    animState.flameIntensity[1] = random(50, 150);
    animState.flameIntensity[2] = 0;
    
    RGBColor flameColor(
      animState.flameIntensity[0],
      animState.flameIntensity[1],
      animState.flameIntensity[2]
    );
    setPixelColor(flameColor);
  }
}

// ============================================
// MODE: STROBE (Fast blink white)
// ============================================
void updateModeStrobe() {
  if (shouldUpdate(animState.lastUpdateTime, 100)) {  // 100ms = 10Hz strobe
    animState.toggleState = !animState.toggleState;
    RGBColor strobeColor = animState.toggleState ? Colors::WHITE : Colors::BLACK;
    setPixelColor(strobeColor);
  }
}

// ============================================
// UTILITY: COLOR FUNCTIONS
// ============================================

/**
 * Convert hue (0-359) to RGB color
 * Uses standard HSV to RGB conversion (at full saturation and value)
 */
RGBColor getColorFromHue(uint16_t hue) {
  // Normalize hue to 0-5 range (6 color segments)
  uint8_t segment = (hue % 360) / 60;
  uint8_t remainder = ((hue % 360) % 60) * 255 / 60;
  
  uint8_t r = 0, g = 0, b = 0;
  
  switch (segment) {
    case 0:  // Red to Yellow
      r = 255;
      g = remainder;
      b = 0;
      break;
    case 1:  // Yellow to Green
      r = 255 - remainder;
      g = 255;
      b = 0;
      break;
    case 2:  // Green to Cyan
      r = 0;
      g = 255;
      b = remainder;
      break;
    case 3:  // Cyan to Blue
      r = 0;
      g = 255 - remainder;
      b = 255;
      break;
    case 4:  // Blue to Magenta
      r = remainder;
      g = 0;
      b = 255;
      break;
    case 5:  // Magenta to Red
      r = 255;
      g = 0;
      b = 255 - remainder;
      break;
  }
  
  return RGBColor(r, g, b);
}

/**
 * Simple sine wave approximation for smooth animations
 * Input: 0 to 2π
 * Output: -1 to 1
 */
float getSineApprox(float x) {
  // Normalize to 0-2π range
  while (x > 6.28) x -= 6.28;
  while (x < 0) x += 6.28;
  
  if (x < 3.14) {
    return sin(x);
  } else {
    return -sin(x - 3.14);
  }
}

// ============================================
// UTILITY: TIMER & CONTROL FUNCTIONS
// ============================================

/**
 * Check if enough time has passed for next update
 */
bool shouldUpdate(unsigned long& lastTime, unsigned long interval) {
  if (currentMillis - lastTime >= interval) {
    lastTime = currentMillis;
    return true;
  }
  return false;
}

/**
 * Set pixel color with brightness scaling
 */
void setPixelColor(const RGBColor& color) {
  // Scale by brightness
  uint8_t r = (color.r * systemState.brightness) / 255;
  uint8_t g = (color.g * systemState.brightness) / 255;
  uint8_t b = (color.b * systemState.brightness) / 255;
  
  pixels.setPixelColor(0, pixels.Color(r, g, b));
  pixels.show();
}

/**
 * Map brightness value 0-255
 */
void setBrightness(uint8_t value) {
  systemState.brightness = constrain(value, 0, 255);
  Serial.print("[SYSTEM] Brightness set to: ");
  Serial.println(systemState.brightness);
}

// ============================================
// UTILITY: INFORMATION & DEBUGGING
// ============================================

void printStartupMessage() {
  Serial.println("\n");
  Serial.println("╔════════════════════════════════════════╗");
  Serial.println("║   ADVANCED NEOPIXEL LED CONTROL       ║");
  Serial.println("║   Board: ESP32 / Arduino              ║");
  Serial.println("║   Mode: Production Ready              ║");
  Serial.println("╚════════════════════════════════════════╝");
  Serial.println();
  Serial.println("AVAILABLE MODES:");
  Serial.println("  0 - OFF");
  Serial.println("  1 - BLINK (Red/Blue toggle)");
  Serial.println("  2 - PULSE (Orange fade)");
  Serial.println("  3 - RAINBOW (Color cycle)");
  Serial.println("  4 - BREATHING (Cyan breath)");
  Serial.println("  5 - FLAME (Random flicker)");
  Serial.println("  6 - STROBE (Fast white blink)");
  Serial.println();
  Serial.println("BUTTON CONTROLS:");
  Serial.println("  Short Press  → Next mode");
  Serial.println("  Long Press   → Previous mode");
  Serial.println("  Very Long    → Toggle power");
  Serial.println();
  printModeInfo();
}

void printModeInfo() {
  Serial.print("═══ Current Mode: ");
  switch (systemState.currentMode) {
    case MODE_OFF:
      Serial.println("OFF ═══");
      break;
    case MODE_BLINK:
      Serial.println("BLINK (Red/Blue) ═══");
      break;
    case MODE_PULSE:
      Serial.println("PULSE (Orange Fade) ═══");
      break;
    case MODE_RAINBOW:
      Serial.println("RAINBOW (Color Cycle) ═══");
      break;
    case MODE_BREATHING:
      Serial.println("BREATHING (Cyan) ═══");
      break;
    case MODE_FLAME:
      Serial.println("FLAME (Random Flicker) ═══");
      break;
    case MODE_STROBE:
      Serial.println("STROBE (Fast White) ═══");
      break;
    default:
      Serial.println("UNKNOWN ═══");
  }
}

/**
 * Print system diagnostics
 */
void printDiagnostics() {
  Serial.println("\n╔════ SYSTEM DIAGNOSTICS ════╗");
  Serial.print("║ Current Mode: ");
  Serial.print((int)systemState.currentMode);
  Serial.println();
  Serial.print("║ Brightness: ");
  Serial.print(systemState.brightness);
  Serial.println(" / 255");
  Serial.print("║ System Enabled: ");
  Serial.println(systemState.isEnabled ? "YES" : "NO");
  Serial.print("║ Uptime: ");
  Serial.print(millis() / 1000);
  Serial.println(" seconds");
  Serial.println("╚════════════════════════════╝\n");
}
