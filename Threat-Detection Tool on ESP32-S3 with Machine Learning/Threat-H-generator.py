#!/usr/bin/env python3
"""
Episode 3: AI-Powered Threat Detection
ML Training Script - Trains anomaly detection model on attack patterns

Run this on your laptop to generate a TensorFlow Lite model
"""

import numpy as np
import json
from datetime import datetime
import tensorflow as tf
from tensorflow import keras
import pickle

# ============================================================
# STEP 1: Generate synthetic training data (or load from Firebase)
# ============================================================

def generate_training_data(num_samples=500):
    """
    Generate training data representing normal attack patterns.
    In production, this would come from your Firebase logs.

    Features:
    - port (normalized 0-1)
    - time_of_day (0-23 hours, normalized)
    - attack_frequency (how many attacks in last hour)
    - ip_reputation (0-1, where 1 = highly suspicious)
    - attack_speed (requests per second)
    """

    X_train = []
    y_train = []  # 0 = normal, 1 = anomaly

    print("[ML] Generating training data...")

    # Normal attack patterns (label = 0)
    # Typical: SSH (port 22), Telnet (23), HTTP (80) during business hours
    for i in range(int(num_samples * 0.7)):
        port = np.random.choice([22, 23, 80, 8080]) / 65535  # normalize
        time_of_day = np.random.randint(8, 18) / 24  # 8am-6pm
        attack_freq = np.random.randint(1, 10) / 100
        ip_rep = np.random.uniform(0, 0.3)  # low reputation
        attack_speed = np.random.uniform(0.1, 2) / 10  # slow

        X_train.append([port, time_of_day, attack_freq, ip_rep, attack_speed])
        y_train.append(0)  # normal

    # Anomalous patterns (label = 1)
    # Unusual: All ports being scanned, 3am attack, very fast, known bad IP
    for i in range(int(num_samples * 0.3)):
        port = np.random.randint(1, 65535) / 65535  # random port (scan pattern)
        time_of_day = np.random.choice([0, 3, 4]) / 24  # weird hours
        attack_freq = np.random.randint(50, 200) / 100  # very frequent
        ip_rep = np.random.uniform(0.7, 1.0)  # high reputation (known bad)
        attack_speed = np.random.uniform(5, 20) / 10  # very fast

        X_train.append([port, time_of_day, attack_freq, ip_rep, attack_speed])
        y_train.append(1)  # anomaly

    return np.array(X_train), np.array(y_train)

# ============================================================
# STEP 2: Build and train the model
# ============================================================

def build_model():
    """
    Build a lightweight neural network for anomaly detection.
    Must be small enough to fit on ESP32 (< 100KB).
    """
    model = keras.Sequential([
        keras.layers.Dense(16, activation='relu', input_shape=(5,)),
        keras.layers.Dropout(0.2),
        keras.layers.Dense(8, activation='relu'),
        keras.layers.Dropout(0.2),
        keras.layers.Dense(1, activation='sigmoid')  # Binary: normal (0) vs anomaly (1)
    ])

    model.compile(
        optimizer='adam',
        loss='binary_crossentropy',
        metrics=['accuracy']
    )

    return model

def train_model(X_train, y_train):
    """Train the model"""
    print("[ML] Building model...")
    model = build_model()

    print("[ML] Training model...")
    history = model.fit(
        X_train, y_train,
        epochs=50,
        batch_size=16,
        validation_split=0.2,
        verbose=1
    )

    return model, history

# ============================================================
# STEP 3: Convert to TensorFlow Lite
# ============================================================

def convert_to_tflite(model):
    """Convert Keras model to TensorFlow Lite format for ESP32"""
    print("[ML] Converting to TensorFlow Lite...")

    converter = tf.lite.TFLiteConverter.from_keras_model(model)
    converter.target_spec.supported_ops = [
        tf.lite.OpsSet.TFLITE_BUILTINS
    ]
    converter.optimizations = [tf.lite.Optimize.DEFAULT]

    tflite_model = converter.convert()

    return tflite_model

# ============================================================
# STEP 4: Save model as C++ array for ESP32
# ============================================================

def save_model_as_cpp_array(tflite_model, filename):
    """
    Save TFlite model as C++ byte array.
    This can be embedded directly in ESP32 code.
    """
    print(f"[ML] Saving model as C++ array to {filename}...")

    # Convert to hex string
    hex_data = ', '.join(f'0x{byte:02x}' for byte in tflite_model)

    cpp_code = f"""
// Auto-generated TensorFlow Lite Model
// Size: {len(tflite_model)} bytes
// Generated: {datetime.now().isoformat()}

const unsigned char threat_detection_model[] = {{
{hex_data}
}};
const int threat_detection_model_size = {len(tflite_model)};
"""

    with open(filename, 'w') as f:
        f.write(cpp_code)

    print(f"[ML] Model saved! Size: {len(tflite_model)} bytes")
    print(f"[ML] File: {filename}")

# ============================================================
# STEP 5: Create threat scoring system
# ============================================================

def generate_threat_scoring_rules():
    """
    Generate JSON with threat scoring rules.
    This helps ESP32 decide what's low/medium/high risk.
    """
    rules = {
        "anomaly_threshold": 0.6,  # ML score > 0.6 = anomaly
        "threat_levels": {
            "green": {
                "min_score": 0,
                "max_score": 0.4,
                "description": "Normal attack pattern",
                "action": "log_only"
            },
            "yellow": {
                "min_score": 0.4,
                "max_score": 0.7,
                "description": "Slightly unusual pattern",
                "action": "alert_telegram"
            },
            "red": {
                "min_score": 0.7,
                "max_score": 1.0,
                "description": "Highly anomalous - possible advanced threat",
                "action": "alert_all"  # Email + Telegram + Firebase
            }
        },
        "port_risk_scores": {
            "22": 0.8,    # SSH - high value target
            "23": 0.7,    # Telnet - common target
            "80": 0.5,    # HTTP - expected traffic
            "443": 0.5,   # HTTPS - expected traffic
            "3306": 0.9,  # MySQL - critical
            "5432": 0.9,  # PostgreSQL - critical
            "27017": 0.95, # MongoDB - critical
        },
        "ip_reputation_sources": [
            "abuseipdb",  # Free API
            "local_blocklist"  # Your own list
        ]
    }

    return rules

# ============================================================
# STEP 6: Save Python model info for later use
# ============================================================

def save_model_info(model, X_train, y_train, filename="threat_model_info.json"):
    """Save model metadata for verification"""

    # Evaluate on training data
    loss, accuracy = model.evaluate(X_train, y_train, verbose=0)

    info = {
        "model_type": "anomaly_detection",
        "input_features": 5,
        "input_feature_names": [
            "port_normalized",
            "time_of_day_normalized",
            "attack_frequency",
            "ip_reputation",
            "attack_speed"
        ],
        "training_accuracy": float(accuracy),
        "training_loss": float(loss),
        "model_size_bytes": len(model.get_weights()),
        "timestamp": datetime.now().isoformat()
    }

    with open(filename, 'w') as f:
        json.dump(info, f, indent=2)

    print(f"[ML] Model info saved to {filename}")
    print(f"[ML] Training accuracy: {accuracy:.2%}")

# ============================================================
# MAIN
# ============================================================

if __name__ == "__main__":
    print("=" * 70)
    print("  EP3: AI-Powered Threat Detection - ML Training")
    print("=" * 70)

    # Generate training data
    X_train, y_train = generate_training_data(num_samples=500)
    print(f"[ML] Generated {len(X_train)} training samples")
    print(f"[ML] Normal samples: {sum(y_train == 0)}")
    print(f"[ML] Anomaly samples: {sum(y_train == 1)}")

    # Build and train model
    model, history = train_model(X_train, y_train)

    # Convert to TensorFlow Lite
    tflite_model = convert_to_tflite(model)

    # Save as C++ array (to current directory)
    save_model_as_cpp_array(tflite_model, "threat_model.h")

    # Save model info
    save_model_info(model, X_train, y_train)

    # Save threat scoring rules
    threat_rules = generate_threat_scoring_rules()
    with open("threat_rules.json", 'w') as f:
        json.dump(threat_rules, f, indent=2)
    print("[ML] Threat scoring rules saved to threat_rules.json")

    print("\n" + "=" * 70)
    print("  ✅ Training complete!")
    print("=" * 70)
    print("\nNext steps:")
    print("1. You will see these files in the same folder as test.py:")
    print("   - threat_model.h")
    print("   - threat_rules.json")
    print("   - threat_model_info.json")
    print("2. Copy threat_model.h to your Arduino folder")
    print("3. Upload the .ino to ESP32")
    print("4. Run: nmap -p 22 <ESP32-IP>")
    print("5. Watch the LEDs blink based on threat scores!")
