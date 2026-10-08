#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <time.h>

// ============================================================
// ESP32 HONEYPOT + GMAIL + TELEGRAM + FIREBASE
// ============================================================

// =============================
// Wi-Fi Configuration
// =============================

const char* ssid = "WIFINAME";
const char* password = "YOUR_WIFI_PASSWORD";

// =============================
// Gmail SMTP Configuration
// Port 465 = Implicit TLS
// =============================

const char* smtp_server = "smtp.gmail.com";
const int smtp_port = 465;

const char* email_sender = "youremail";
const char* email_password = "YOUR_NEW_GMAIL_APP_PASSWORD";
const char* email_recipient = "youremail";

// =============================
// Telegram Configuration
// =============================

const char* telegram_bot_token = "YOUR_NEW_TELEGRAM_BOT_TOKEN";
const char* telegram_chat_id = "yourid";
const char* telegram_api = "https://api.telegram.org/bot";

// =============================
// Firebase Configuration
// =============================

const char* firebase_host =
  "your*default-rtdb.firebaseio.com";

const char* firebase_auth = "testing123";

// =============================
// LED Configuration
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
// Statistics
// =============================

unsigned long totalConnections = 0;
unsigned long lastAlertTime = 0;

const unsigned long ALERT_COOLDOWN = 5000;

// =============================
// Attack Record
// =============================

struct AttackRecord {
  String service;
  String port;
  String attackerIP;
  String timestamp;
  long unixTime;
};

AttackRecord lastAttack;

// ============================================================
// LED FUNCTIONS
// ============================================================

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

// ============================================================
// TIME SYNCHRONIZATION
// ============================================================

void syncTime() {

  configTime(
    0,
    0,
    "pool.ntp.org",
    "time.nist.gov"
  );

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

  strftime(
    buffer,
    sizeof(buffer),
    "%Y-%m-%d %H:%M:%S",
    timeinfo
  );

  return String(buffer);
}

// ============================================================
// BASE64
// ============================================================

String base64Encode(String input) {

  static const char base64_chars[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

  String output = "";

  int i = 0;

  unsigned char char_array_3[3];
  unsigned char char_array_4[4];

  for (int index = 0; index < input.length(); index++) {

    char_array_3[i % 3] = input[index];

    if ((index + 1) % 3 == 0) {

      char_array_4[0] =
        (char_array_3[0] & 0xfc) >> 2;

      char_array_4[1] =
        ((char_array_3[0] & 0x03) << 4) +
        ((char_array_3[1] & 0xf0) >> 4);

      char_array_4[2] =
        ((char_array_3[1] & 0x0f) << 2) +
        ((char_array_3[2] & 0xc0) >> 6);

      char_array_4[3] =
        char_array_3[2] & 0x3f;

      for (int j = 0; j < 4; j++) {
        output += base64_chars[char_array_4[j]];
      }
    }

    i++;
  }

  int remaining = input.length() % 3;

  if (remaining == 1) {

    int pos = input.length() - 1;

    unsigned char c1 = input[pos];

    char_array_4[0] =
      (c1 & 0xfc) >> 2;

    char_array_4[1] =
      (c1 & 0x03) << 4;

    output += base64_chars[char_array_4[0]];
    output += base64_chars[char_array_4[1]];
    output += "==";

  } else if (remaining == 2) {

    int pos = input.length() - 2;

    unsigned char c1 = input[pos];
    unsigned char c2 = input[pos + 1];

    char_array_4[0] =
      (c1 & 0xfc) >> 2;

    char_array_4[1] =
      ((c1 & 0x03) << 4) +
      ((c2 & 0xf0) >> 4);

    char_array_4[2] =
      (c2 & 0x0f) << 2;

    output += base64_chars[char_array_4[0]];
    output += base64_chars[char_array_4[1]];
    output += base64_chars[char_array_4[2]];
    output += "=";
  }

  return output;
}

// ============================================================
// SMTP RESPONSE READER
// ============================================================

String readSMTPResponse(
  WiFiClientSecure& client,
  unsigned long timeout = 10000
) {

  String lastLine = "";

  unsigned long startTime = millis();

  while (millis() - startTime < timeout) {

    if (client.available()) {

      String line = client.readStringUntil('\n');

      line.trim();

      if (line.length() == 0) {
        continue;
      }

      Serial.println("[SMTP] " + line);

      lastLine = line;

      // SMTP:
      //
      // 250-xxxxxxxx
      // 250-xxxxxxxx
      // 250 xxxxxxxx
      //
      // '-' means more lines
      // ' ' means final line

      if (
        line.length() >= 4 &&
        line[0] >= '0' &&
        line[0] <= '9' &&
        line[1] >= '0' &&
        line[1] <= '9' &&
        line[2] >= '0' &&
        line[2] <= '9' &&
        line[3] == ' '
      ) {

        return line;
      }

    } else {

      delay(20);
    }
  }

  return lastLine;
}

// ============================================================
// SMTP EXPECT
// ============================================================

bool smtpExpect(
  WiFiClientSecure& client,
  int expectedCode,
  unsigned long timeout = 10000
) {

  String response =
    readSMTPResponse(
      client,
      timeout
    );

  if (response.length() < 3) {

    Serial.println(
      "[SMTP] No valid response received!"
    );

    return false;
  }

  int receivedCode =
    response.substring(0, 3).toInt();

  Serial.print("[SMTP] Expected: ");
  Serial.print(expectedCode);

  Serial.print(" | Received: ");
  Serial.println(receivedCode);

  return receivedCode == expectedCode;
}

// ============================================================
// GMAIL SMTP
// ============================================================
// Port 465 uses implicit TLS.
// There is NO STARTTLS command.
// ============================================================

bool sendEmailAlertGmail(
  String service,
  String port,
  String attackerIP,
  String timestamp
) {

  Serial.println();
  Serial.println(
    "[EMAIL] Attempting Gmail SMTP on port 465..."
  );

  if (!WiFi.isConnected()) {

    Serial.println(
      "[EMAIL] Wi-Fi not connected!"
    );

    return false;
  }

  WiFiClientSecure secureClient;

  // For testing.
  // Production should use certificate verification.
  secureClient.setInsecure();

  secureClient.setTimeout(15);

  Serial.println(
    "[SMTP] Connecting to smtp.gmail.com:465..."
  );

  if (
    !secureClient.connect(
      smtp_server,
      smtp_port
    )
  ) {

    Serial.println(
      "[EMAIL] TLS connection failed!"
    );

    return false;
  }

  Serial.println(
    "[SMTP] TLS connection established!"
  );

  // -------------------------
  // Gmail Greeting
  // -------------------------

  if (
    !smtpExpect(
      secureClient,
      220
    )
  ) {

    Serial.println(
      "[EMAIL] Invalid Gmail greeting!"
    );

    secureClient.stop();

    return false;
  }

  // -------------------------
  // EHLO
  // -------------------------

  Serial.println(
    "[SMTP] Sending EHLO..."
  );

  secureClient.println(
    "EHLO ESP32Honeypot"
  );

  if (
    !smtpExpect(
      secureClient,
      250
    )
  ) {

    Serial.println(
      "[EMAIL] EHLO failed!"
    );

    secureClient.stop();

    return false;
  }

  // -------------------------
  // AUTH LOGIN
  // -------------------------

  Serial.println(
    "[SMTP] Sending AUTH LOGIN..."
  );

  secureClient.println(
    "AUTH LOGIN"
  );

  if (
    !smtpExpect(
      secureClient,
      334
    )
  ) {

    Serial.println(
      "[EMAIL] AUTH LOGIN failed!"
    );

    secureClient.stop();

    return false;
  }

  // -------------------------
  // Username
  // -------------------------

  Serial.println(
    "[SMTP] Sending Gmail username..."
  );

  secureClient.println(
    base64Encode(
      String(email_sender)
    )
  );

  if (
    !smtpExpect(
      secureClient,
      334
    )
  ) {

    Serial.println(
      "[EMAIL] Gmail username rejected!"
    );

    secureClient.stop();

    return false;
  }

  // -------------------------
  // App Password
  // -------------------------

  Serial.println(
    "[SMTP] Sending Gmail App Password..."
  );

  secureClient.println(
    base64Encode(
      String(email_password)
    )
  );

  if (
    !smtpExpect(
      secureClient,
      235
    )
  ) {

    Serial.println(
      "[EMAIL] Gmail authentication failed!"
    );

    Serial.println(
      "[EMAIL] Check your NEW Gmail App Password."
    );

    secureClient.stop();

    return false;
  }

  Serial.println(
    "[EMAIL] Gmail authentication successful!"
  );

  // -------------------------
  // MAIL FROM
  // -------------------------

  Serial.println(
    "[SMTP] MAIL FROM..."
  );

  secureClient.print(
    "MAIL FROM: <"
  );

  secureClient.print(
    email_sender
  );

  secureClient.println(
    ">"
  );

  if (
    !smtpExpect(
      secureClient,
      250
    )
  ) {

    Serial.println(
      "[EMAIL] MAIL FROM failed!"
    );

    secureClient.stop();

    return false;
  }

  // -------------------------
  // RCPT TO
  // -------------------------

  Serial.println(
    "[SMTP] RCPT TO..."
  );

  secureClient.print(
    "RCPT TO: <"
  );

  secureClient.print(
    email_recipient
  );

  secureClient.println(
    ">"
  );

  if (
    !smtpExpect(
      secureClient,
      250
    )
  ) {

    Serial.println(
      "[EMAIL] RCPT TO failed!"
    );

    secureClient.stop();

    return false;
  }

  // -------------------------
  // DATA
  // -------------------------

  Serial.println(
    "[SMTP] DATA..."
  );

  secureClient.println(
    "DATA"
  );

  if (
    !smtpExpect(
      secureClient,
      354
    )
  ) {

    Serial.println(
      "[EMAIL] DATA command failed!"
    );

    secureClient.stop();

    return false;
  }

  // -------------------------
  // Headers
  // -------------------------

  secureClient.print(
    "From: "
  );

  secureClient.println(
    email_sender
  );

  secureClient.print(
    "To: "
  );

  secureClient.println(
    email_recipient
  );

  secureClient.print(
    "Subject: [ESP32 ALERT] "
  );

  secureClient.print(
    service
  );

  secureClient.print(
    " Attack Detected on Port "
  );

  secureClient.println(
    port
  );

  secureClient.println(
    "Content-Type: text/plain; charset=UTF-8"
  );

  secureClient.println();

  // -------------------------
  // Body
  // -------------------------

  secureClient.println(
    "ATTACK DETECTED ON YOUR ESP32 HONEYPOT"
  );

  secureClient.println();

  secureClient.println(
    "================================"
  );

  secureClient.println(
    "ATTACK DETAILS"
  );

  secureClient.println(
    "================================"
  );

  secureClient.println();

  secureClient.print(
    "Service: "
  );

  secureClient.println(
    service
  );

  secureClient.print(
    "Port: "
  );

  secureClient.println(
    port
  );

  secureClient.print(
    "Attacker IP: "
  );

  secureClient.println(
    attackerIP
  );

  secureClient.print(
    "Timestamp: "
  );

  secureClient.println(
    timestamp
  );

  secureClient.print(
    "Total Attacks: "
  );

  secureClient.println(
    totalConnections
  );

  secureClient.println();

  secureClient.println(
    "This is an automated alert from your ESP32 Honeypot."
  );

  // -------------------------
  // End DATA
  // -------------------------

  secureClient.println(
    "."
  );

  if (
    !smtpExpect(
      secureClient,
      250
    )
  ) {

    Serial.println(
      "[EMAIL] Gmail rejected the message!"
    );

    secureClient.stop();

    return false;
  }

  // -------------------------
  // QUIT
  // -------------------------

  secureClient.println(
    "QUIT"
  );

  smtpExpect(
    secureClient,
    221,
    5000
  );

  secureClient.stop();

  Serial.println(
    "[EMAIL] Email sent successfully!"
  );

  return true;
}

// ============================================================
// TELEGRAM
// ============================================================

bool sendTelegramAlert(
  String service,
  String port,
  String attackerIP,
  String timestamp
) {

  Serial.println();
  Serial.println(
    "[TELEGRAM] Attempting to send Telegram alert..."
  );

  if (!WiFi.isConnected()) {

    Serial.println(
      "[TELEGRAM] Wi-Fi not connected!"
    );

    return false;
  }

  WiFiClientSecure telegramClient;

  telegramClient.setInsecure();

  HTTPClient http;

  String message =
    "🚨 *ATTACK DETECTED!*\n\n";

  message +=
    "🎯 *Service:* " +
    service +
    "\n";

  message +=
    "📍 *Port:* " +
    port +
    "\n";

  message +=
    "🔴 *Attacker IP:* `" +
    attackerIP +
    "`\n";

  message +=
    "⏰ *Time:* " +
    timestamp +
    "\n";

  message +=
    "📊 *Total Attacks:* " +
    String(totalConnections);

  String url =
    String(telegram_api) +
    telegram_bot_token +
    "/sendMessage";

  StaticJsonDocument<768> doc;

  doc["chat_id"] =
    telegram_chat_id;

  doc["text"] =
    message;

  doc["parse_mode"] =
    "Markdown";

  String jsonBody;

  serializeJson(
    doc,
    jsonBody
  );

  if (
    !http.begin(
      telegramClient,
      url
    )
  ) {

    Serial.println(
      "[TELEGRAM] HTTPS initialization failed!"
    );

    return false;
  }

  http.addHeader(
    "Content-Type",
    "application/json"
  );

  int httpResponseCode =
    http.POST(
      jsonBody
    );

  if (
    httpResponseCode == 200
  ) {

    Serial.println(
      "[TELEGRAM] Telegram alert sent successfully!"
    );

    http.end();

    return true;
  }

  Serial.print(
    "[TELEGRAM] Failed. HTTP code: "
  );

  Serial.println(
    httpResponseCode
  );

  http.end();

  return false;
}

// ============================================================
// FIREBASE
// ============================================================

bool sendToFirebase(
  String service,
  String port,
  String attackerIP,
  String timestamp
) {

  Serial.println();
  Serial.println(
    "[FIREBASE] Attempting to send data to Firebase..."
  );

  if (!WiFi.isConnected()) {

    Serial.println(
      "[FIREBASE] Wi-Fi not connected!"
    );

    return false;
  }

  WiFiClientSecure firebaseClient;

  firebaseClient.setInsecure();

  HTTPClient http;

  String firebaseUrl =
    "https://" +
    String(firebase_host) +
    "/attacks/" +
    String(totalConnections) +
    ".json?auth=" +
    String(firebase_auth);

  StaticJsonDocument<512> doc;

  doc["service"] =
    service;

  doc["port"] =
    port;

  doc["attacker_ip"] =
    attackerIP;

  doc["timestamp"] =
    timestamp;

  doc["unix_time"] =
    time(nullptr);

  doc["attack_number"] =
    totalConnections;

  String jsonBody;

  serializeJson(
    doc,
    jsonBody
  );

  if (
    !http.begin(
      firebaseClient,
      firebaseUrl
    )
  ) {

    Serial.println(
      "[FIREBASE] HTTPS initialization failed!"
    );

    return false;
  }

  http.addHeader(
    "Content-Type",
    "application/json"
  );

  int httpResponseCode =
    http.PUT(
      jsonBody
    );

  if (
    httpResponseCode == 200
  ) {

    Serial.println(
      "[FIREBASE] Data sent successfully!"
    );

    http.end();

    return true;
  }

  Serial.print(
    "[FIREBASE] Failed. HTTP code: "
  );

  Serial.println(
    httpResponseCode
  );

  http.end();

  return false;
}

// ============================================================
// SETUP
// ============================================================

void setup() {

  Serial.begin(115200);

  delay(1000);

  Serial.println();

  Serial.println(
    "================================"
  );

  Serial.println(
    "      ESP32 HONEYPOT"
  );

  Serial.println(
    "   Gmail | Telegram | Firebase"
  );

  Serial.println(
    "================================"
  );

  initializeLEDs();

  delay(500);

  blinkLED(
    STATUS_LED,
    2,
    300
  );

  // -------------------------
  // Wi-Fi
  // -------------------------

  Serial.print(
    "Connecting to Wi-Fi: "
  );

  Serial.println(
    ssid
  );

  WiFi.mode(
    WIFI_STA
  );

  WiFi.begin(
    ssid,
    password
  );

  int attempts = 0;

  while (
    WiFi.status() != WL_CONNECTED &&
    attempts < 30
  ) {

    delay(500);

    Serial.print(".");

    attempts++;
  }

  Serial.println();

  if (
    WiFi.status() == WL_CONNECTED
  ) {

    Serial.println(
      "Wi-Fi connected!"
    );

    Serial.print(
      "ESP32 IP Address: "
    );

    Serial.println(
      WiFi.localIP()
    );

    blinkLED(
      STATUS_LED,
      5,
      100
    );

  } else {

    Serial.println(
      "Failed to connect to Wi-Fi!"
    );

    blinkLED(
      STATUS_LED,
      3,
      500
    );
  }

  // -------------------------
  // NTP
  // -------------------------

  Serial.println();

  Serial.println(
    "Synchronizing time..."
  );

  syncTime();

  // -------------------------
  // Start Honeypot
  // -------------------------

  telnetServer.begin();
  httpServer.begin();
  sshServer.begin();
  adminServer.begin();

  Serial.println();

  Serial.println(
    "Honeypot services started:"
  );

  Serial.println(
    "TCP 22   - SSH       (GPIO 5)"
  );

  Serial.println(
    "TCP 23   - Telnet    (GPIO 18)"
  );

  Serial.println(
    "TCP 80   - HTTP      (GPIO 19)"
  );

  Serial.println(
    "TCP 8080 - Admin     (GPIO 21)"
  );

  Serial.println();

  Serial.println(
    "System Ready!"
  );

  Serial.println(
    "Gmail SMTP 465: ENABLED"
  );

  Serial.println(
    "Telegram: ENABLED"
  );

  Serial.println(
    "Firebase: ENABLED"
  );

  Serial.println();

  Serial.println(
    "Waiting for connections..."
  );

  Serial.println(
    "================================"
  );

  Serial.println();
}

// ============================================================
// CONNECTION HANDLER
// ============================================================

void checkServer(
  WiFiServer &server,
  int port,
  const char* service,
  int ledPin
) {

  WiFiClient client =
    server.available();

  if (!client) {
    return;
  }

  totalConnections++;

  unsigned long currentTime =
    millis();

  String attackerIP =
    client.remoteIP().toString();

  String timestamp =
    getTimestamp();

  String portStr =
    String(port);

  String serviceStr =
    String(service);

  // -------------------------
  // Save Attack
  // -------------------------

  lastAttack.service =
    serviceStr;

  lastAttack.port =
    portStr;

  lastAttack.attackerIP =
    attackerIP;

  lastAttack.timestamp =
    timestamp;

  lastAttack.unixTime =
    time(nullptr);

  // -------------------------
  // Serial Output
  // -------------------------

  Serial.println();

  Serial.println(
    "╔════════════════════════════════════╗"
  );

  Serial.println(
    "║     CONNECTION DETECTED            ║"
  );

  Serial.println(
    "╚════════════════════════════════════╝"
  );

  Serial.print(
    "Service: "
  );

  Serial.println(
    service
  );

  Serial.print(
    "Port: "
  );

  Serial.println(
    port
  );

  Serial.print(
    "Attacker IP: "
  );

  Serial.println(
    attackerIP
  );

  Serial.print(
    "Timestamp: "
  );

  Serial.println(
    timestamp
  );

  Serial.println(
    "────────────────────────────────────"
  );

  // -------------------------
  // LED
  // -------------------------

  indicateConnection(
    ledPin,
    service
  );

  // -------------------------
  // SSH
  // -------------------------

  if (
    port == 22
  ) {

    client.println(
      "SSH-2.0-OpenSSH_8.2p1 Ubuntu-4ubuntu0.5"
    );
  }

  // -------------------------
  // HTTP
  // -------------------------

  else if (
    port == 80 ||
    port == 8080
  ) {

    client.println(
      "HTTP/1.1 200 OK"
    );

    client.println(
      "Content-Type: text/html"
    );

    client.println(
      "Connection: close"
    );

    client.println();

    client.println(
      "<html>"
      "<body>"
      "<h1>IoT Device</h1>"
      "</body>"
      "</html>"
    );
  }

  // -------------------------
  // Telnet
  // -------------------------

  else if (
    port == 23
  ) {

    client.println(
      "Welcome to IoT Device"
    );

    client.println(
      "login:"
    );
  }

  delay(100);

  client.stop();

  // -------------------------
  // Alerts
  // -------------------------

  if (
    currentTime - lastAlertTime >
    ALERT_COOLDOWN
  ) {

    Serial.println();

    Serial.println(
      "[ALERTS] Sending notifications..."
    );

    bool emailResult =
      sendEmailAlertGmail(
        serviceStr,
        portStr,
        attackerIP,
        timestamp
      );

    bool telegramResult =
      sendTelegramAlert(
        serviceStr,
        portStr,
        attackerIP,
        timestamp
      );

    bool firebaseResult =
      sendToFirebase(
        serviceStr,
        portStr,
        attackerIP,
        timestamp
      );

    Serial.println();

    Serial.println(
      "========== ALERT SUMMARY =========="
    );

    Serial.print(
      "Email: "
    );

    Serial.println(
      emailResult
        ? "SUCCESS"
        : "FAILED"
    );

    Serial.print(
      "Telegram: "
    );

    Serial.println(
      telegramResult
        ? "SUCCESS"
        : "FAILED"
    );

    Serial.print(
      "Firebase: "
    );

    Serial.println(
      firebaseResult
        ? "SUCCESS"
        : "FAILED"
    );

    Serial.println(
      "==================================="
    );

    lastAlertTime =
      currentTime;

  } else {

    Serial.println(
      "[ALERTS] Cooldown active - skipping alerts"
    );
  }
}

// ============================================================
// MAIN LOOP
// ============================================================

void loop() {

  checkServer(
    sshServer,
    22,
    "SSH",
    SSH_LED
  );

  checkServer(
    telnetServer,
    23,
    "Telnet",
    TELNET_LED
  );

  checkServer(
    httpServer,
    80,
    "HTTP",
    HTTP_LED
  );

  checkServer(
    adminServer,
    8080,
    "HTTP Admin",
    ADMIN_LED
  );

  // -------------------------
  // Wi-Fi Reconnect
  // -------------------------

  if (
    WiFi.status() != WL_CONNECTED
  ) {

    Serial.println(
      "Wi-Fi connection lost!"
    );

    blinkLED(
      STATUS_LED,
      2,
      500
    );

    WiFi.disconnect();

    WiFi.begin(
      ssid,
      password
    );

    delay(1000);
  }

  delay(100);
}
