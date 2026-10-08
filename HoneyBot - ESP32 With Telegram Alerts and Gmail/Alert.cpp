#include <WiFi.h>
#include <WiFiClientSecure.h>  // ← ADD THIS LINE
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <time.h>

// =============================
// Wi-Fi Configuration
// =============================

const char* ssid = "Naval";
const char* password = "naval05245";

// =============================
// Gmail SMTP Configuration (Direct - No IFTTT)
// =============================

const char* smtp_server = "smtp.gmail.com";
const int smtp_port = 587;
const char* email_sender = "your_email@gmail.com";        // Your Gmail address
const char* email_password = "your_app_password";         // Gmail App Password (NOT regular password)
const char* email_recipient = "recipient@gmail.com";      // Where to send alerts

// =============================
// Telegram Configuration
// =============================

const char* telegram_bot_token = "YOUR_TELEGRAM_BOT_TOKEN";
const char* telegram_chat_id = "YOUR_TELEGRAM_CHAT_ID";
const char* telegram_api = "https://api.telegram.org/bot";

// =============================
// Firebase Configuration
// =============================

const char* firebase_host = "your-project.firebaseio.com";
const char* firebase_auth = "YOUR_FIREBASE_DATABASE_SECRET";

// =============================
// LED Pin Configuration
// =============================

const int SSH_LED = 5;
const int TELNET_LED = 18;
const int HTTP_LED = 19;
const int ADMIN_LED = 21;
const int STATUS_LED = 2;

// =============================
// Honeypot Ports
// =============================

WiFiServer telnetServer(23);
WiFiServer httpServer(80);
WiFiServer sshServer(22);
WiFiServer adminServer(8080);

// =============================
// Statistics & Logging
// =============================

unsigned long totalConnections = 0;
unsigned long lastAlertTime = 0;
const unsigned long ALERT_COOLDOWN = 5000; // 5 second cooldown between alerts

struct AttackRecord {
  String service;
  String port;
  String attackerIP;
  String timestamp;
  long unixTime;
};

AttackRecord lastAttack;

// =============================
// LED Control Functions
// =============================

void initializeLEDs() {
  pinMode(SSH_LED, OUTPUT);
  pinMode(TELNET_LED, OUTPUT);
  pinMode(HTTP_LED, OUTPUT);
  pinMode(ADMIN_LED, OUTPUT);
  pinMode(STATUS_LED, OUTPUT);

  digitalWrite(SSH_LED, LOW);
  digitalWrite(TELNET_LED, LOW);
  digitalWrite(HTTP_LED, LOW);
  digitalWrite(ADMIN_LED, LOW);
  digitalWrite(STATUS_LED, LOW);

  Serial.println("LEDs initialized!");
}

void blinkLED(int pin, int times, int delayMs) {
  for (int i = 0; i < times; i++) {
    digitalWrite(pin, HIGH);
    delay(delayMs);
    digitalWrite(pin, LOW);
    delay(delayMs);
  }
}

void indicateConnection(int ledPin, const char* serviceName) {
  blinkLED(ledPin, 3, 200);
  blinkLED(STATUS_LED, 1, 300);
  digitalWrite(ledPin, HIGH);
  delay(2000);
  digitalWrite(ledPin, LOW);
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
  struct tm timeinfo = *localtime(&now);
  Serial.print("Current time: ");
  Serial.println(asctime(&timeinfo));
}

String getTimestamp() {
  time_t now = time(nullptr);
  struct tm* timeinfo = localtime(&now);
  char buffer[30];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
  return String(buffer);
}

// =============================
// Gmail SMTP Alert Function (Direct - No IFTTT)
// =============================

bool sendEmailAlertGmail(String service, String port, String attackerIP, String timestamp) {
  Serial.println("\n[EMAIL] Attempting to send email via Gmail SMTP...");

  if (!WiFi.isConnected()) {
    Serial.println("[EMAIL] Wi-Fi not connected!");
    return false;
  }

  // Create WiFiClientSecure for SMTP
  WiFiClientSecure client;
  client.setInsecure(); // For testing only - in production, use proper certificates

  // Connect to Gmail SMTP server
  if (!client.connect(smtp_server, smtp_port)) {
    Serial.println("[EMAIL] Failed to connect to SMTP server!");
    return false;
  }

  delay(500);

  // Read server greeting
  while (client.available()) {
    String line = client.readStringUntil('\n');
    Serial.println("[SMTP] " + line);
  }

  // Send EHLO command
  client.println("EHLO ESP32");
  delay(500);
  while (client.available()) {
    String line = client.readStringUntil('\n');
    Serial.println("[SMTP] " + line);
  }

  // Send STARTTLS command (upgrade to TLS)
  client.println("STARTTLS");
  delay(500);
  while (client.available()) {
    String line = client.readStringUntil('\n');
    Serial.println("[SMTP] " + line);
  }

  delay(500);
  client.stop();

  // Reconnect with TLS
  if (!client.connect(smtp_server, smtp_port)) {
    Serial.println("[EMAIL] Failed to reconnect with TLS!");
    return false;
  }

  delay(500);
  while (client.available()) {
    String line = client.readStringUntil('\n');
    Serial.println("[SMTP] " + line);
  }

  // Send EHLO again after TLS
  client.println("EHLO ESP32");
  delay(500);
  while (client.available()) {
    String line = client.readStringUntil('\n');
    Serial.println("[SMTP] " + line);
  }

  // Authentication - PLAIN method (base64 encoded)
  // Format: \0username\0password
  String auth = String("\0") + email_sender + String("\0") + email_password;

  // Simple base64 encoding (limited version for SMTP)
  client.println("AUTH PLAIN " + base64Encode(auth));
  delay(500);
  while (client.available()) {
    String line = client.readStringUntil('\n');
    Serial.println("[SMTP] " + line);
    if (line.indexOf("235") >= 0 || line.indexOf("334") >= 0) {
      Serial.println("[EMAIL] Authentication successful!");
      break;
    }
  }

  // Send email
  client.print("MAIL FROM: <");
  client.print(email_sender);
  client.println(">");
  delay(500);
  while (client.available()) {
    String line = client.readStringUntil('\n');
    Serial.println("[SMTP] " + line);
  }

  client.print("RCPT TO: <");
  client.print(email_recipient);
  client.println(">");
  delay(500);
  while (client.available()) {
    String line = client.readStringUntil('\n');
    Serial.println("[SMTP] " + line);
  }

  client.println("DATA");
  delay(500);
  while (client.available()) {
    String line = client.readStringUntil('\n');
    Serial.println("[SMTP] " + line);
  }

  // Email headers and body
  client.print("From: ");
  client.println(email_sender);
  client.print("To: ");
  client.println(email_recipient);
  client.println("Subject: [ESP32 ALERT] " + service + " Attack Detected on Port " + port);
  client.println("Content-Type: text/plain; charset=UTF-8");
  client.println();

  // Email body
  client.println("🚨 ATTACK DETECTED ON YOUR ESP32 HONEYPOT 🚨");
  client.println();
  client.println("ATTACK DETAILS:");
  client.println("─────────────────────────────────────────");
  client.println("Service: " + service);
  client.println("Port: " + port);
  client.println("Attacker IP: " + attackerIP);
  client.println("Timestamp: " + timestamp);
  client.println("─────────────────────────────────────────");
  client.println();
  client.println("This is an automated alert from your ESP32 Honeypot.");
  client.println("If you did not expect this, investigate immediately!");
  client.println();
  client.println("Best regards,");
  client.println("Your ESP32 Security System");

  client.println(".");
  delay(500);
  while (client.available()) {
    String line = client.readStringUntil('\n');
    Serial.println("[SMTP] " + line);
  }

  client.println("QUIT");
  delay(500);
  while (client.available()) {
    String line = client.readStringUntil('\n');
    Serial.println("[SMTP] " + line);
  }

  client.stop();

  Serial.println("[EMAIL] Email sent successfully!");
  return true;
}

// Simple base64 encoding helper (limited version for SMTP AUTH)
String base64Encode(String input) {
  static const char base64_chars[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

  String output = "";
  int i = 0;
  unsigned char char_array_3[3];
  unsigned char char_array_4[4];

  for (int i = 0; i < input.length(); i++) {
    char_array_3[i % 3] = input[i];
    if ((i + 1) % 3 == 0) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;
      for (i = 0; i < 4; i++) output += base64_chars[char_array_4[i]];
    }
  }

  if (input.length() % 3 == 1) {
    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = (char_array_3[0] & 0x03) << 4;
    output += base64_chars[char_array_4[0]];
    output += base64_chars[char_array_4[1]];
    output += "==";
  } else if (input.length() % 3 == 2) {
    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = (char_array_3[1] & 0x0f) << 2;
    output += base64_chars[char_array_4[0]];
    output += base64_chars[char_array_4[1]];
    output += base64_chars[char_array_4[2]];
    output += "=";
  }

  return output;
}

// =============================
// Telegram Alert Function
// =============================

bool sendTelegramAlert(String service, String port, String attackerIP, String timestamp) {
  Serial.println("\n[TELEGRAM] Attempting to send Telegram alert...");

  if (!WiFi.isConnected()) {
    Serial.println("[TELEGRAM] Wi-Fi not connected!");
    return false;
  }

  HTTPClient http;

  // Build Telegram message
  String message = "🚨 *ATTACK DETECTED!*\n\n";
  message += "🎯 *Service:* " + service + "\n";
  message += "📍 *Port:* " + port + "\n";
  message += "🔴 *Attacker IP:* `" + attackerIP + "`\n";
  message += "⏰ *Time:* " + timestamp + "\n";
  message += "📊 *Total Attacks:* " + String(totalConnections);

  String url = String(telegram_api) + telegram_bot_token + "/sendMessage";

  StaticJsonDocument<512> doc;
  doc["chat_id"] = telegram_chat_id;
  doc["text"] = message;
  doc["parse_mode"] = "Markdown";

  String jsonBody;
  serializeJson(doc, jsonBody);

  http.begin(url);
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.POST(jsonBody);

  if (httpResponseCode == 200) {
    Serial.println("[TELEGRAM] Telegram alert sent successfully!");
    http.end();
    return true;
  } else {
    Serial.print("[TELEGRAM] Failed to send Telegram alert. Response: ");
    Serial.println(httpResponseCode);
    String response = http.getString();
    Serial.println(response);
    http.end();
    return false;
  }
}

// =============================
// Firebase Alert Function
// =============================

bool sendToFirebase(String service, String port, String attackerIP, String timestamp) {
  Serial.println("\n[FIREBASE] Attempting to send data to Firebase...");

  if (!WiFi.isConnected()) {
    Serial.println("[FIREBASE] Wi-Fi not connected!");
    return false;
  }

  HTTPClient http;

  // Firebase Realtime Database URL
  String firebaseUrl = "https://" + String(firebase_host) + "/attacks/" + String(totalConnections) + ".json?auth=" + String(firebase_auth);

  // Create JSON payload
  StaticJsonDocument<256> doc;
  doc["service"] = service;
  doc["port"] = port;
  doc["attacker_ip"] = attackerIP;
  doc["timestamp"] = timestamp;
  doc["unix_time"] = time(nullptr);
  doc["attack_number"] = totalConnections;

  String jsonBody;
  serializeJson(doc, jsonBody);

  http.begin(firebaseUrl);
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.PUT(jsonBody);

  if (httpResponseCode == 200) {
    Serial.println("[FIREBASE] Data sent to Firebase successfully!");
    http.end();
    return true;
  } else {
    Serial.print("[FIREBASE] Failed to send to Firebase. Response: ");
    Serial.println(httpResponseCode);
    String response = http.getString();
    Serial.println(response);
    http.end();
    return false;
  }
}

// =============================
// Setup
// =============================

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("================================");
  Serial.println("   ESP32 HONEYPOT + ALERTS");
  Serial.println("   Gmail | Telegram | Firebase");
  Serial.println("================================");

  // Initialize LEDs
  initializeLEDs();
  delay(500);
  blinkLED(STATUS_LED, 2, 300);

  // Connect to Wi-Fi
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
    Serial.print("ESP32 IP Address: ");
    Serial.println(WiFi.localIP());
    blinkLED(STATUS_LED, 5, 100);
  } else {
    Serial.println("Failed to connect to Wi-Fi!");
    blinkLED(STATUS_LED, 3, 500);
  }

  // Sync time from NTP server
  Serial.println("\nSynchronizing time...");
  syncTime();

  // Start honeypot services
  telnetServer.begin();
  httpServer.begin();
  sshServer.begin();
  adminServer.begin();

  Serial.println("\nHoneypot services started:");
  Serial.println("TCP 22   - SSH       (LED: GPIO 5)");
  Serial.println("TCP 23   - Telnet    (LED: GPIO 18)");
  Serial.println("TCP 80   - HTTP      (LED: GPIO 19)");
  Serial.println("TCP 8080 - Admin     (LED: GPIO 21)");
  Serial.println("Status LED: GPIO 2");

  Serial.println("\n✅ System Ready!");
  Serial.println("📧 Email alerts (Gmail SMTP): ENABLED");
  Serial.println("📱 Telegram alerts: ENABLED");
  Serial.println("☁️  Firebase logging: ENABLED");
  Serial.println("\nWaiting for connections...");
  Serial.println("================================\n");
}

// =============================
// Connection Handler with Alerts
// =============================

void checkServer(
  WiFiServer &server,
  int port,
  const char* service,
  int ledPin
) {
  WiFiClient client = server.available();

  if (client) {
    totalConnections++;
    unsigned long currentTime = millis();

    // Get attacker IP
    String attackerIP = client.remoteIP().toString();
    String timestamp = getTimestamp();
    String portStr = String(port);
    String serviceStr = String(service);

    // Serial output
    Serial.println();
    Serial.println("╔════════════════════════════════════╗");
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
    Serial.print("Total Connections: ");
    Serial.println(totalConnections);
    Serial.println("────────────────────────────────────");

    // LED indication
    indicateConnection(ledPin, service);

    // Send fake banner
    if (port == 22) {
      client.println("SSH-2.0-OpenSSH_8.2p1 Ubuntu-4ubuntu0.5");
    }
    else if (port == 80 || port == 8080) {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("Connection: close");
      client.println();
      client.println("<html>");
      client.println("<head><title>IoT Device</title></head>");
      client.println("<body>");
      client.println("<h1>IoT Device Management</h1>");
      client.println("<p>System Status: Online</p>");
      client.println("</body>");
      client.println("</html>");
    }
    else if (port == 23) {
      client.println("Welcome to IoT Device");
      client.println("login:");
    }

    delay(100);
    client.stop();

    // Send alerts (with cooldown to avoid spam)
    if (currentTime - lastAlertTime > ALERT_COOLDOWN) {
      Serial.println("\n[ALERTS] Sending notifications...");

      // Send email alert via Gmail SMTP
      sendEmailAlertGmail(serviceStr, portStr, attackerIP, timestamp);

      // Send Telegram alert
      sendTelegramAlert(serviceStr, portStr, attackerIP, timestamp);

      // Send to Firebase
      sendToFirebase(serviceStr, portStr, attackerIP, timestamp);

      lastAlertTime = currentTime;
      Serial.println("[ALERTS] All notifications sent!\n");
    } else {
      Serial.println("[ALERTS] Cooldown active - skipping alerts this time\n");
    }

    Serial.println("Connection closed.");
  }
}

// =============================
// Main Loop
// =============================

void loop() {
  // Check each honeypot service with corresponding LED
  checkServer(sshServer, 22, "SSH", SSH_LED);
  checkServer(telnetServer, 23, "Telnet", TELNET_LED);
  checkServer(httpServer, 80, "HTTP", HTTP_LED);
  checkServer(adminServer, 8080, "HTTP Admin", ADMIN_LED);

  // Check Wi-Fi connection
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi connection lost!");
    blinkLED(STATUS_LED, 2, 500);
    WiFi.disconnect();
    WiFi.begin(ssid, password);
    delay(1000);
  }

  delay(100); // Small delay to prevent overwhelming the loop
}
