#include <WiFi.h>

// =============================
// Wi-Fi Configuration
// =============================

const char* ssid = "Naval";
const char* password = "naval05245";

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

void setup() {

  Serial.begin(115200);

  delay(1000);

  Serial.println();
  Serial.println("================================");
  Serial.println("       ESP32 IoT HONEYPOT");
  Serial.println("================================");

  // Connect to Wi-Fi
  Serial.print("Connecting to Wi-Fi");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);

    Serial.print(".");
  }

  Serial.println();
  Serial.println("Wi-Fi connected!");

  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());

  // Start honeypot services
  telnetServer.begin();
  httpServer.begin();
  sshServer.begin();
  adminServer.begin();

  Serial.println();
  Serial.println("Honeypot services started:");
  Serial.println("TCP 22   - SSH");
  Serial.println("TCP 23   - Telnet");
  Serial.println("TCP 80   - HTTP");
  Serial.println("TCP 8080 - Admin Panel");

  Serial.println();
  Serial.println("Waiting for connections...");
  Serial.println("--------------------------------");
}


void checkServer(
  WiFiServer &server,
  int port,
  const char* service
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


void loop() {

  // Check each honeypot service

  checkServer(
    sshServer,
    22,
    "SSH"
  );

  checkServer(
    telnetServer,
    23,
    "Telnet"
  );

  checkServer(
    httpServer,
    80,
    "HTTP"
  );

  checkServer(
    adminServer,
    8080,
    "HTTP Admin"
  );

  // Check Wi-Fi connection

  if (WiFi.status() != WL_CONNECTED) {

    Serial.println("Wi-Fi connection lost!");

    WiFi.disconnect();
    WiFi.begin(ssid, password);

    delay(1000);
  }
}

