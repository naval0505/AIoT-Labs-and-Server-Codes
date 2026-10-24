#include <WiFi.h>
#include <time.h>

#include <Chirale_TensorFlowLite.h>
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "threat_model.h"

// NeoPixel/WS2812B RGB LED Library
#include <Adafruit_NeoPixel.h>

// =============================
// WiFi Configuration
// =============================

const char* ssid = "lk";
const char* password = "hellosir123456797845612365487946513254879@9784651";

// =============================
// RGB LED Configuration (NeoPixel)
// =============================

#define RGB_PIN 38  // ESP32 S3 GPIO pin for NeoPixel data
#define NUM_LEDS 5  // 5 RGB LEDs (1 for each service + 1 status)

// LED Indices
#define SSH_LED_IDX 0
#define TELNET_LED_IDX 1
#define HTTP_LED_IDX 2
#define ADMIN_LED_IDX 3
#define STATUS_LED_IDX 4

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, RGB_PIN, NEO_GRB + NEO_KHZ800);

// Color definitions
#define COLOR_GREEN strip.Color(0, 255, 0)      // Green
#define COLOR_YELLOW strip.Color(255, 255, 0)   // Yellow
#define COLOR_RED strip.Color(255, 0, 0)        // Red
#define COLOR_BLUE strip.Color(0, 0, 255)       // Blue
#define COLOR_CYAN strip.Color(0, 255, 255)     // Cyan
#define COLOR_MAGENTA strip.Color(255, 0, 255)  // Magenta
#define COLOR_WHITE strip.Color(255, 255, 255)  // White
#define COLOR_OFF strip.Color(0, 0, 0)          // Off

WiFiServer telnetServer(23);
WiFiServer httpServer(80);
WiFiServer sshServer(22);
WiFiServer adminServer(8080);

// =============================
// Struct Definition
// =============================

struct ThreatAnalysis {
  float ml_score;
  String threat_level;
};

// =============================
// Statistics
// =============================

unsigned long totalConnections = 0;

// =============================
// TensorFlow Lite Variables
// =============================

const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;

// Threat scoring thresholds
const float GREEN_THRESHOLD = 0.4;
const float YELLOW_THRESHOLD = 0.7;

// =============================
// TensorFlow Lite Setup
// =============================

void setupTensorFlow() {
  Serial.println("[TF] Initializing TensorFlow Lite...");

  // Load model from embedded array
  model = tflite::GetModel(threat_detection_model);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    Serial.println("[TF] ERROR: Model schema mismatch!");
    return;
  }

  static tflite::AllOpsResolver resolver;
  static uint8_t tensor_arena[4096];

  static tflite::MicroInterpreter static_interpreter(
    model, resolver, tensor_arena, sizeof(tensor_arena), nullptr
  );
  interpreter = &static_interpreter;

  if (interpreter->AllocateTensors() != kTfLiteOk) {
    Serial.println("[TF] ERROR: Failed to allocate tensors!");
    return;
  }

  input = interpreter->input(0);
  output = interpreter->output(0);

  Serial.println("[TF] ✓ TensorFlow Lite initialized!");
}

// =============================
// RGB LED Control Functions
// =============================

void setLEDColor(int ledIndex, uint32_t color) {
  strip.setPixelColor(ledIndex, color);
  strip.show();
}

void allLedsOff() {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, COLOR_OFF);
  }
  strip.show();
}

void blinkRGB(int ledIndex, uint32_t color, int times, int delayMs) {
  for (int i = 0; i < times; i++) {
    setLEDColor(ledIndex, color);
    delay(delayMs);
    setLEDColor(ledIndex, COLOR_OFF);
    delay(delayMs);
  }
}

void pulseRGB(int ledIndex, uint32_t color, int duration) {
  for (int i = 0; i < 255; i += 15) {
    uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8) & 0xFF;
    uint8_t b = color & 0xFF;
    strip.setPixelColor(ledIndex, strip.Color(r * i / 255, g * i / 255, b * i / 255));
    strip.show();
    delay(duration / 17);
  }
  for (int i = 255; i > 0; i -= 15) {
    uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8) & 0xFF;
    uint8_t b = color & 0xFF;
    strip.setPixelColor(ledIndex, strip.Color(r * i / 255, g * i / 255, b * i / 255));
    strip.show();
    delay(duration / 17);
  }
  setLEDColor(ledIndex, COLOR_OFF);
}

void indicateThreatLevel(String level) {
  if (level == "green") {
    // Green pulse
    pulseRGB(STATUS_LED_IDX, COLOR_GREEN, 500);
  }
  else if (level == "yellow") {
    // Yellow double blink
    blinkRGB(STATUS_LED_IDX, COLOR_YELLOW, 2, 300);
  }
  else if (level == "red") {
    // Red rapid blink
    blinkRGB(STATUS_LED_IDX, COLOR_RED, 5, 100);
  }
}

// =============================
// Feature Extraction
// =============================

float extractPortFeature(int port) {
  return (float)port / 65535.0;
}

float extractTimeFeature() {
  time_t now = time(nullptr);
  struct tm* timeinfo = localtime(&now);
  return (float)timeinfo->tm_hour / 24.0;
}

float extractAttackFrequencyFeature() {
  return (float)random(0, 100) / 100.0;
}

float extractIPReputationFeature(String attackerIP) {
  uint32_t hash = 0;
  for (char c : attackerIP) {
    hash = ((hash << 5) + hash) + c;
  }
  return (float)(hash % 100) / 100.0;
}

float extractAttackSpeedFeature() {
  return (float)random(0, 100) / 1000.0;
}

// =============================
// Threat Detection
// =============================

ThreatAnalysis analyzeThreat(int port, String attackerIP) {
  // Extract features
  float port_norm = extractPortFeature(port);
  float time_norm = extractTimeFeature();
  float attack_freq = extractAttackFrequencyFeature();
  float ip_rep = extractIPReputationFeature(attackerIP);
  float attack_speed = extractAttackSpeedFeature();

  Serial.print("[THREAT] Features - Port: ");
  Serial.print(port_norm, 2);
  Serial.print(" | Time: ");
  Serial.print(time_norm, 2);
  Serial.print(" | Freq: ");
  Serial.print(attack_freq, 2);
  Serial.print(" | IP Rep: ");
  Serial.print(ip_rep, 2);
  Serial.print(" | Speed: ");
  Serial.println(attack_speed, 3);

  // Set input tensor
  input->data.f[0] = port_norm;
  input->data.f[1] = time_norm;
  input->data.f[2] = attack_freq;
  input->data.f[3] = ip_rep;
  input->data.f[4] = attack_speed;

  // Run inference
  if (interpreter->Invoke() != kTfLiteOk) {
    Serial.println("[THREAT] ERROR: Inference failed!");
    return {0.5, "unknown"};
  }

  float ml_score = output->data.f[0];

  // Determine threat level
  String threat_level = "green";
  if (ml_score >= YELLOW_THRESHOLD) {
    threat_level = "red";
  } else if (ml_score >= GREEN_THRESHOLD) {
    threat_level = "yellow";
  }

  Serial.print("[THREAT] ML Score: ");
  Serial.print(ml_score, 3);
  Serial.print(" | Level: ");
  Serial.println(threat_level);

  return {ml_score, threat_level};
}

// =============================
// LED Control & Initialization
// =============================

void initializeLEDs() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  // Boot animation: Rainbow pulse
  for (int i = 0; i < NUM_LEDS; i++) {
    setLEDColor(i, COLOR_CYAN);
    delay(100);
  }
  delay(300);
  allLedsOff();

  Serial.println("RGB LEDs initialized!");
}

// =============================
// Time Synchronization
// =============================

void syncTime() {
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  int i = 0;
  while (now < 24 * 3600 && ++i < 20) {
    Serial.print(".");
    delay(500);
    now = time(nullptr);
  }
  Serial.println();
}

String getTimestamp() {
  time_t now = time(nullptr);
  struct tm* timeinfo = localtime(&now);
  char buffer[30];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
  return String(buffer);
}

// =============================
// Setup
// =============================

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n================================");
  Serial.println("   EP3: AI THREAT DETECTION");
  Serial.println("   RGB LED Version (ESP32 S3)");
  Serial.println("================================");

  initializeLEDs();
  delay(500);

  // Connect to WiFi
  Serial.print("Connecting to Wi-Fi: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  Serial.println();
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Wi-Fi connected!");
    Serial.print("ESP32 IP: ");
    Serial.println(WiFi.localIP());

    // WiFi connected animation
    for (int i = 0; i < NUM_LEDS; i++) {
      setLEDColor(i, COLOR_GREEN);
      delay(100);
    }
    delay(300);
    allLedsOff();
  } else {
    Serial.println("Failed to connect!");
    // WiFi failed animation
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < NUM_LEDS; j++) {
        setLEDColor(j, COLOR_RED);
      }
      delay(300);
      allLedsOff();
      delay(300);
    }
  }

  // Sync time
  Serial.println("\nSynchronizing time...");
  syncTime();

  // Initialize TensorFlow Lite
  setupTensorFlow();

  // Start honeypot
  telnetServer.begin();
  httpServer.begin();
  sshServer.begin();
  adminServer.begin();

  Serial.println("\nHoneypot services started (with AI threat detection)");
  Serial.println("✅ System Ready!");
  Serial.println("================================\n");

  // Ready animation
  for (int i = 0; i < NUM_LEDS; i++) {
    setLEDColor(i, COLOR_BLUE);
  }
  delay(500);
  allLedsOff();
}

// =============================
// Connection Handler
// ================================

void checkServer(WiFiServer &server, int port, const char* service, int ledIdx) {
  WiFiClient client = server.available();

  if (client) {
    totalConnections++;

    String attackerIP = client.remoteIP().toString();
    String timestamp = getTimestamp();

    Serial.println("\n╔════════════════════════════════════╗");
    Serial.println("║  !!! CONNECTION DETECTED !!!       ║");
    Serial.println("╚════════════════════════════════════╝");
    Serial.print("Service: ");
    Serial.println(service);
    Serial.print("Port: ");
    Serial.println(port);
    Serial.print("Attacker IP: ");
    Serial.println(attackerIP);
    Serial.print("Timestamp: ");
    Serial.println(timestamp);
    Serial.println("────────────────────────────────────");

    // Service LED blink (Magenta)
    blinkRGB(ledIdx, COLOR_MAGENTA, 3, 200);

    // Analyze threat using ML
    ThreatAnalysis threat = analyzeThreat(port, attackerIP);

    // Visual feedback based on threat level
    indicateThreatLevel(threat.threat_level);

    // Send fake banner
    if (port == 22) {
      client.println("SSH-2.0-OpenSSH_8.2p1 Ubuntu-4ubuntu0.5");
    } else if (port == 80 || port == 8080) {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("Connection: close\n");
      client.println("<html><body><h1>IoT Device</h1></body></html>");
    } else if (port == 23) {
      client.println("Welcome to IoT Device");
      client.println("login:");
    }

    delay(100);
    client.stop();

    Serial.println("Connection closed.\n");
  }
}

// =============================
// Main Loop
// =============================

void loop() {
  checkServer(sshServer, 22, "SSH", SSH_LED_IDX);
  checkServer(telnetServer, 23, "Telnet", TELNET_LED_IDX);
  checkServer(httpServer, 80, "HTTP", HTTP_LED_IDX);
  checkServer(adminServer, 8080, "HTTP Admin", ADMIN_LED_IDX);

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi lost!");
    setLEDColor(STATUS_LED_IDX, COLOR_RED);
    delay(500);
    setLEDColor(STATUS_LED_IDX, COLOR_OFF);
    delay(500);
    WiFi.disconnect();
    WiFi.begin(ssid, password);
    delay(1000);
  }

  delay(100);
}
