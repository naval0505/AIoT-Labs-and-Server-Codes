#include <WiFi.h>

// =============================
// Wi-Fi Configuration
// =============================

const char* ssid = "Naval";
const char* password = "naval05245";

// =============================
// LED Pin Configuration
// =============================

const int SSH_LED = 5;        // GPIO 5 - SSH (Port 22)
const int TELNET_LED = 18;    // GPIO 18 - Telnet (Port 23)
const int HTTP_LED = 19;      // GPIO 19 - HTTP (Port 80)
const int ADMIN_LED = 21;     // GPIO 21 - Admin (Port 8080)
const int STATUS_LED = 2;     // GPIO 2 - Overall Status

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

// =============================
// LED Control Functions
// =============================

void initializeLEDs() {
  pinMode(SSH_LED, OUTPUT);
  pinMode(TELNET_LED, OUTPUT);
  pinMode(HTTP_LED, OUTPUT);
  pinMode(ADMIN_LED, OUTPUT);
  pinMode(STATUS_LED, OUTPUT);

  // Turn off all LEDs initially
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
  // Blink the specific service LED 3 times
  blinkLED(ledPin, 3, 200);

  // Also blink the status LED
  blinkLED(STATUS_LED, 1, 300);

  // Keep LED on for 2 seconds to show activity
  digitalWrite(ledPin, HIGH);
  delay(2000);
  digitalWrite(ledPin, LOW);
}

// =============================
// Setup
// =============================

void setup() {

  Serial.begin(115200);

  delay(1000);

  Serial.println();
  Serial.println("================================");
  Serial.println("   ESP32 IoT HONEYPOT + LEDs");
  Serial.println("================================");

  // Initialize LEDs
  initializeLEDs();

  delay(500);

  // Status LED blink to indicate startup
  blinkLED(STATUS_LED, 2, 300);

  // Connect to Wi-Fi
  Serial.print("Connecting to Wi-Fi");

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

    // Fast blink status LED to indicate successful connection
    blinkLED(STATUS_LED, 5, 100);

  } else {
    Serial.println("Failed to connect to Wi-Fi!");
    // Slow blink status LED to indicate connection failure
    blinkLED(STATUS_LED, 3, 500);
  }

  // Start honeypot services
  telnetServer.begin();
  httpServer.begin();
  sshServer.begin();
  adminServer.begin();

  Serial.println();
  Serial.println("Honeypot services started:");
  Serial.println("TCP 22   - SSH       (LED: GPIO 5)");
  Serial.println("TCP 23   - Telnet    (LED: GPIO 18)");
  Serial.println("TCP 80   - HTTP      (LED: GPIO 19)");
  Serial.println("TCP 8080 - Admin     (LED: GPIO 21)");
  Serial.println("Status LED: GPIO 2");

  Serial.println();
  Serial.println("Waiting for connections...");
  Serial.println("--------------------------------");
}

// =============================
// Connection Handler
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

    Serial.println();
    Serial.println("!!! CONNECTION DETECTED !!!");

    Serial.print("Service: ");
    Serial.println(service);

    Serial.print("Port: ");
    Serial.println(port);

    Serial.print("Source IP: ");
    Serial.println(client.remoteIP());

    Serial.print("Total Connections: ");
    Serial.println(totalConnections);

    Serial.println("--------------------------------");

    // Trigger LED indication
    indicateConnection(ledPin, service);

    // Send a fake service banner
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

    Serial.println("Connection closed.");
  }
}

// =============================
// Main Loop
// =============================

void loop() {

  // Check each honeypot service with corresponding LED

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

  // Check Wi-Fi connection

  if (WiFi.status() != WL_CONNECTED) {

    Serial.println("Wi-Fi connection lost!");

    // Blink status LED to indicate Wi-Fi issue
    blinkLED(STATUS_LED, 2, 500);

    WiFi.disconnect();
    WiFi.begin(ssid, password);

    delay(1000);
  }
}
