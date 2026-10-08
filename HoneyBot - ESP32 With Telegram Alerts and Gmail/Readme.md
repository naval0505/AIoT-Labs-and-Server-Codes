# ESP32 Honeypot with Multi-Channel Alerts

A DIY cybersecurity project that turns an ESP32 microcontroller into an intelligent honeypot with real-time alerts via **Gmail SMTP**, **Telegram**, and **Firebase Cloud Logging**.

## 🎯 Project Overview

This is **Episode 2** of a YouTube series on DIY cybersecurity. It demonstrates how to:

- Monitor multiple ports (SSH, Telnet, HTTP, Admin) simultaneously
- Detect incoming connection attempts automatically
- Send **instant email alerts** via Gmail SMTP
- Send **real-time push notifications** via Telegram Bot API
- **Log all attacks** to Firebase Realtime Database for cloud analytics
- Control **5 indicator LEDs** for visual feedback

All three alert systems fire simultaneously when an attack is detected.

## 🔧 Hardware Requirements

- **ESP32 DevKit V1** (or compatible)
- **5 × LEDs** (any color, RGB optional)
- **5 × 220Ω resistors** (for LED current limiting)
- **Breadboard & jumper wires**
- **USB cable** (for programming)
- **WiFi network** (2.4GHz)

### Wiring

```
ESP32 GPIO Pins → LED Pins:
- GPIO 5   → SSH LED (Port 22)
- GPIO 18  → Telnet LED (Port 23)
- GPIO 19  → HTTP LED (Port 80)
- GPIO 21  → Admin LED (Port 8080)
- GPIO 2   → Status LED

Each LED: Anode → GPIO (via 220Ω resistor), Cathode → GND
```

## 📋 Software Requirements

### Arduino IDE Libraries

Install these via **Sketch → Include Library → Manage Libraries**:

- **ArduinoJson** (by Benoit Blanchon) - JSON handling
- **WiFi** (built-in with ESP32 board support)
- **HTTPClient** (built-in)
- **time.h** (built-in - for NTP sync)

### Board Setup

```
Tools → Board → ESP32 → ESP32 Dev Module
Tools → Port → (select your COM port)
Tools → Upload Speed → 921600
```

## ⚙️ Configuration

Before uploading, update these credentials in the code:

```cpp
// WiFi
const char* ssid = "Your_WiFi_Name";
const char* password = "Your_WiFi_Password";

// Gmail SMTP (Port 587 with STARTTLS)
const char* email_sender = "your_email@gmail.com";
const char* email_password = "xxxx xxxx xxxx xxxx";  // App Password, not regular password
const char* email_recipient = "recipient@gmail.com";

// Telegram Bot
const char* telegram_bot_token = "YOUR_BOT_TOKEN";
const char* telegram_chat_id = "YOUR_CHAT_ID";

// Firebase Realtime Database
const char* firebase_host = "your-project.firebaseio.com";
const char* firebase_auth = "YOUR_AUTH_TOKEN";
```

### Step-by-Step Setup

#### 1️⃣ Gmail App Password (5 min)

1. Go to **https://myaccount.google.com/**
2. Click **Security** (left sidebar)
3. Enable **2-Step Verification** if not already enabled
4. Click **App passwords**
5. Select "Mail" and "Windows Computer"
6. Google generates a 16-character password (e.g., `uhmw vpff lpds cjuh`)
7. Copy this password **WITH SPACES** into `email_password`

> ⚠️ Use the **App Password**, NOT your regular Gmail password!

#### 2️⃣ Telegram Bot Token (10 min)

1. Open **Telegram**
2. Search for **@BotFather**
3. Type **/newbot**
4. Follow prompts to create your bot
5. Copy the token (looks like `123456:ABC-DEF...`)
6. Paste into `telegram_bot_token`

**Get Chat ID:**
1. Start your bot (send any message to @YourBotName)
2. Go to: `https://api.telegram.org/botYOUR_TOKEN/getUpdates`
3. Find `"chat":{"id": YOUR_CHAT_ID`
4. Paste that number into `telegram_chat_id`

#### 3️⃣ Firebase Realtime Database (15 min)

1. Go to **https://console.firebase.google.com/**
2. Create new project: "esp32-honeypot"
3. Add **Realtime Database**
4. Copy database URL: `esp32-honeypot.firebaseio.com`
5. Paste into `firebase_host`
6. For testing, set `firebase_auth = "testing123"`
7. Set database rules to allow writes:
   ```json
   {
     "rules": {
       ".read": true,
       ".write": true
     }
   }
   ```

## 🚀 Upload & Test

### Upload Code
```bash
Arduino IDE → Sketch → Upload
```

### Test the System

Open **Serial Monitor** at **115200 baud**. You should see:
```
================================
   ESP32 HONEYPOT + ALERTS
   Gmail | Telegram | Firebase
================================
LEDs initialized!
Connecting to Wi-Fi: Naval
Wi-Fi connected!
ESP32 IP Address: 192.168.x.x
✅ System Ready!
📧 Email alerts (Gmail SMTP): ENABLED
📱 Telegram alerts: ENABLED
☁️ Firebase logging: ENABLED
```

### Trigger an Alert

From another computer on your network:
```bash
nmap -p 22 192.168.x.x
```

Watch:
- ✅ **LEDs** blink on ESP32
- ✅ **Serial Monitor** shows connection detected
- ✅ **Telegram** notification arrives on phone
- ✅ **Gmail** inbox receives alert email
- ✅ **Firebase** console shows new entry

## 📊 Alert Format

### Email
```
From: your@gmail.com
Subject: [ESP32 ALERT] SSH Attack Detected on Port 22

ATTACK DETECTED ON YOUR ESP32 HONEYPOT

ATTACK DETAILS:
Service: SSH
Port: 22
Attacker IP: 192.168.x.y
Timestamp: 2026-10-08 11:05:30
```

### Telegram
```
🚨 ATTACK DETECTED!

🎯 Service: SSH
📍 Port: 22
🔴 Attacker IP: 192.168.x.y
⏰ Time: 2026-10-08 11:05:30
📊 Total Attacks: 1
```

### Firebase
```json
{
  "attacks": {
    "1": {
      "service": "SSH",
      "port": "22",
      "attacker_ip": "192.168.x.y",
      "timestamp": "2026-10-08 11:05:30",
      "unix_time": 1728379530,
      "attack_number": 1
    }
  }
}
```

## 🔐 Security Notes

- **App Passwords** are safer than regular Gmail passwords
- **setInsecure()** is used for testing only - in production, implement proper certificate validation
- **Credentials should never be hardcoded** in production - use secure storage or environment variables
- The 5-second **alert cooldown** prevents spam from rapid port scans
- Database is set to **test mode** - restrict `.write` rules in production

## 🐛 Troubleshooting

| Problem | Solution |
|---------|----------|
| Wi-Fi won't connect | Check SSID/password (case-sensitive). Verify ESP32 is in WiFi range |
| Email not sending | Verify App Password (with spaces). Check 2FA is enabled. Confirm email_sender matches Gmail account |
| Telegram silent | Check bot token & chat ID. Verify Telegram notifications enabled |
| Firebase not updating | Check firebase_host has no "https://". Verify rules allow ".write": true |
| Gibberish in Serial | Change Serial Monitor baud to **115200** |

## 📚 Project Structure

```
ESP32-Honeypot/
├── EP2_GMAIL_SMTP_FINAL_FIX.cpp     (Main code - ready to upload)
├── README.md                        (This file)
├── EP2_GMAIL_SMTP_SETUP_GUIDE.txt   (Detailed setup instructions)
├── EP2_VIDEO_SCRIPT_AND_GUIDE.txt   (YouTube video script)
└── CIRCUIT_DIAGRAM.txt              (Wiring diagram & calculations)
```

## 🎥 Video Series

This project is part of a **YouTube series on DIY Cybersecurity**:

- **Episode 1:** Basic honeypot with LEDs
- **Episode 2:** Multi-channel alerts (Gmail, Telegram, Firebase) 

## 📄 License

Open source - feel free to modify and use for educational purposes.

## ⭐ Support

If this project helps you learn cybersecurity, consider:
- ⭐ **Star this repo**
- 📺 **Subscribe** to the YouTube channel
- 💬 **Share** with your network
- 🐛 **Report issues** or suggest improvements


**Jai Shri Ram, Happy hacking! 🚀**
