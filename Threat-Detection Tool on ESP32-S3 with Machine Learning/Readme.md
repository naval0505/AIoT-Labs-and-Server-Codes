# ESP32 AI-Powered Threat Detection Honeypot

## 🎯 Project Overview

A sophisticated DIY cybersecurity system that uses **Machine Learning** on an ESP32 microcontroller to detect and classify network attacks in real-time. This project demonstrates how enterprise security systems work by combining honeypot technology with embedded AI.

**Perfect for:** Learning cybersecurity, IoT security, embedded machine learning, threat intelligence

---

## ✨ Features

- ✅ **Real-time Attack Detection** - Monitors SSH (22), Telnet (23), HTTP (80), Admin (8080) ports
- ✅ **Machine Learning Inference** - TensorFlow Lite neural network running on ESP32
- ✅ **Intelligent Threat Scoring** - AI analyzes attack patterns and classifies threat level
- ✅ **RGB LED Visualization** - Beautiful color-coded threat indicators
- ✅ **5-Feature Analysis** - Port, Time, Frequency, IP Reputation, Attack Speed
- ✅ **Real-time Threat Classification** - GREEN/YELLOW/RED threat levels
- ✅ **Serial Monitor Logging** - Detailed attack analytics and ML scores

---

## 🔴 What It Does

### Attack Detection Flow

```
1. Network Scan Detected
   ↓
2. Extract 5 Features
   - Port (which port?)
   - Time of Day (3am vs 2pm?)
   - Attack Frequency (how many attacks?)
   - IP Reputation (is this IP malicious?)
   - Attack Speed (how fast?)
   ↓
3. ML Model Inference
   TensorFlow Lite neural network processes features
   ↓
4. Threat Score Calculation
   Returns: 0.0 (normal) to 1.0 (anomalous)
   ↓
5. Threat Level Classification
   - 🟢 GREEN (0.0-0.4): Normal traffic
   - 🟡 YELLOW (0.4-0.7): Suspicious
   - 🔴 RED (0.7-1.0): High threat
   ↓
6. RGB LED Feedback
   Visual indication of threat level
```

---

## 🌈 RGB LED Color Guide

| Color | Blink Pattern | Threat Level | Meaning |
|-------|---------------|--------------|---------|
| 🟢 **GREEN** | Pulse | Low (0-0.4) | Normal attack pattern |
| 🟡 **YELLOW** | Double blink | Medium (0.4-0.7) | Slightly suspicious |
| 🔴 **RED** | Rapid blink | High (0.7-1.0) | Possible advanced threat |
| 🔵 **BLUE** | Steady | Boot | System initialized |
| 🩷 **MAGENTA** | Single blink | Detection | Connection detected |
| ⚫ **OFF** | Off | Idle | Waiting for connections |

---

## 🛠️ Hardware Requirements

### Components
- **ESP32 S3 DevKit** (or compatible ESP32)
- **5× NeoPixel/WS2812B RGB LEDs** (addressable)
- **Breadboard & Jumper Wires**
- **USB Cable** (for programming)
- **WiFi Network** (2.4GHz)

### Wiring
```
ESP32 GPIO 38 → NeoPixel Data Pin
ESP32 GND → NeoPixel GND
ESP32 5V → NeoPixel 5V

LED Index Mapping:
- LED 0: SSH (Port 22)
- LED 1: Telnet (Port 23)
- LED 2: HTTP (Port 80)
- LED 3: Admin (Port 8080)
- LED 4: Status/Threat Level
```

---

## 💻 Software Requirements

### Arduino IDE Libraries
Install via **Sketch → Include Library → Manage Libraries**:
- ✅ `Adafruit NeoPixel` - RGB LED control
- ✅ `Chirale_TensorFlowLite` - ML inference on ESP32
- ✅ Built-in: WiFi, HTTPClient, ArduinoJson, time.h

### Python (For Training)
- TensorFlow 2.x
- NumPy
- Keras (included with TensorFlow)

---

## 📋 Setup Instructions

### Step 1: Generate ML Model (On Your Laptop)

```bash
# Install Python dependencies
pip install tensorflow numpy

# Run the training script
python test.py
```

**Output files:**
- `threat_model.h` - Trained neural network (copy to Arduino folder)
- `threat_rules.json` - Threat scoring configuration
- `threat_model_info.json` - Model metadata

### Step 2: Configure ESP32 Code

Edit `EP3_RGB_VERSION.cpp`:

```cpp
// Line 14-15: WiFi credentials
const char* ssid = "Your_WiFi_Name";
const char* password = "Your_WiFi_Password";

// Line 28: NeoPixel GPIO pin (change if different)
#define RGB_PIN 38

// Line 29: Number of RGB LEDs
#define NUM_LEDS 5
```

### Step 3: Upload to ESP32

1. Open Arduino IDE
2. **Tools → Board → ESP32 S3**
3. **Tools → Port → Select your ESP32 port**
4. Paste code from `EP3_RGB_VERSION.cpp`
5. **Sketch → Upload**

### Step 4: Test the System

```bash
# From another computer on same WiFi
nmap -p 22 192.168.x.x
```

**Expected output:**
- Serial Monitor shows threat analysis
- LED blinks with color based on threat level
- If GREEN: "Normal traffic"
- If RED: "Suspicious activity detected"

---

## 📊 Example Output

### Normal HTTP Connection (GREEN)

```
╔════════════════════════════════╗
║  !!! CONNECTION DETECTED !!!   ║
╚════════════════════════════════╝
Service: HTTP
Port: 80
Attacker IP: 10.211.81.46
Timestamp: 2026-10-24 03:36:16

[THREAT] Features - Port: 0.00 | Time: 0.13 | Freq: 0.37 | IP Rep: 0.10 | Speed: 0.076
[THREAT] ML Score: 0.015 | Level: green

✅ GREEN LED pulses
→ "This is normal traffic"
```

### Suspicious Port Scan (RED)

```
╔════════════════════════════════╗
║  !!! CONNECTION DETECTED !!!   ║
╚════════════════════════════════╝
Service: SSH
Port: 22
Attacker IP: 192.168.1.105
Timestamp: 2026-10-24 03:15:42

[THREAT] Features - Port: 0.00 | Time: 0.125 | Freq: 0.95 | IP Rep: 0.89 | Speed: 0.85
[THREAT] ML Score: 0.842 | Level: red

🔴 RED LED blinks rapidly
→ "ALERT! Suspicious pattern detected"
```

---

## 🧠 How the Machine Learning Works

### Neural Network Architecture

```
Input Layer (5 features)
    ↓
Dense Layer (16 neurons) + ReLU activation
    ↓
Dropout (20% - prevents overfitting)
    ↓
Dense Layer (8 neurons) + ReLU activation
    ↓
Dropout (20%)
    ↓
Output Layer (1 neuron) + Sigmoid (0-1 probability)
```

### Training Results

- **Training Accuracy:** 99.5%+
- **Validation Accuracy:** 100%
- **Model Size:** ~770KB (optimized for ESP32)
- **Inference Time:** 50-100ms per prediction

### What the Model Learns

**Normal Patterns:**
- SSH on port 22 during business hours
- Slow connection speed
- Single connection attempt

**Anomalous Patterns:**
- Random high ports (65432, etc.)
- Connections at 3am
- High attack frequency (botnet behavior)
- Known malicious IPs
- Very fast request rates

---

## 🎓 Learning Outcomes

By building this project, you'll understand:

- ✅ **Embedded Machine Learning** - Running neural networks on microcontrollers
- ✅ **Threat Intelligence** - What makes an attack suspicious
- ✅ **Honeypot Technology** - How security systems trap and analyze attackers
- ✅ **Real-time Processing** - AI decision-making in milliseconds
- ✅ **Network Security** - Port monitoring and attack detection
- ✅ **IoT Security** - Bringing AI to constrained devices
- ✅ **Feature Engineering** - Extracting meaningful data for ML

---

## 🧪 Testing Guide

### Test 1: Normal HTTP Traffic
```bash
nmap -p 80 192.168.x.x
# Expected: 🟢 GREEN
```

### Test 2: SSH Scan
```bash
nmap -p 22 192.168.x.x
# Expected: 🟢 GREEN (daytime) or 🟡 YELLOW (3am)
```

### Test 3: Multiple Port Scan (Suspicious)
```bash
nmap -p 22,23,80,8080,65432 192.168.x.x
# Expected: 🟡 YELLOW or 🔴 RED
```

### Test 4: Full Port Range (Very Suspicious)
```bash
nmap -p 1-1000 192.168.x.x
# Expected: 🔴 RED (fast scanning pattern)
```

---

## 📁 Project Structure

```
ESP32_AI_Threat_Detection/
├── test.py                        # ML training script
├── EP3_RGB_VERSION.cpp            # Arduino code with RGB LEDs
├── threat_model.h                 # Generated ML model
├── threat_rules.json              # Threat scoring config
├── threat_model_info.json         # Model metadata
└── README.md                      # This file
```

---

## 🚀 Episode Series

This is **Episode 3** of a DIY Cybersecurity YouTube series:

- **Episode 1:** Basic Honeypot with LEDs (port monitoring)
- **Episode 2:** Multi-channel Alerts (Email, Telegram, Firebase)
- **Episode 3:** AI Threat Detection (TensorFlow Lite + ML) ← **You are here**
- **Episode 4+:** Advanced analytics, cloud logging, forensics

---

## 🔐 Security Notes

- ⚠️ **For Learning Only** - This is an educational project
- ⚠️ **Test in Safe Environment** - Use in isolated networks or lab setups
- ⚠️ **No Credentials in Code** - Never hardcode WiFi passwords in production
- ⚠️ **Model Training** - Synthetic data used; in production, train on real attacks

---

## 💡 Future Enhancements

- [ ] Real-time attack logging to cloud (Firebase)
- [ ] Email/SMS alerts for RED threats
- [ ] Dashboard for analyzing attack patterns
- [ ] Retrain model with real attack data
- [ ] Export logs for SIEM integration
- [ ] Multiple honeypot instances in network
- [ ] GeoIP blocking for known malicious regions

---

## 🎯 Why This Matters

This project bridges **cybersecurity** and **machine learning**:

- Enterprises deploy honeypots to understand attacker behavior
- ML models help distinguish real threats from noise
- Embedded AI brings security closer to the network edge
- Understanding these concepts is crucial for security careers

**This is production-grade security methodology, scaled down for learning!**

---

## 📸 Photos & Demo

[Add screenshots of RGB LEDs lighting up]
[Add Serial Monitor output examples]
[Add network scan demonstration]

---

## 📚 References

- TensorFlow Lite: https://www.tensorflow.org/lite/microcontrollers
- ESP32 Arduino: https://github.com/espressif/arduino-esp32
- Adafruit NeoPixel: https://github.com/adafruit/Adafruit_NeoPixel
- Honeypot Concepts: https://en.wikipedia.org/wiki/Honeypot_(computing)

---

## 🤝 Contributing

Found a bug? Have improvements? Open an issue or submit a pull request!

---

## 📝 License

This project is open source and available under the MIT License.

---

**Naval Koshti**

---

## ⭐ Support

If this project helps you learn cybersecurity and machine learning:
- ⭐ **Star this repo**
- 📺 **Subscribe to the YouTube channel**
- 💬 **Share with your network**
- 🐛 **Report issues you find**

---

**Happy hacking! 🚀🔒**
