# IoT-Based Forest Fire Detection Drone (ESP32 Project)
## Project Overview

This project detects early signs of forest fire using an ESP32-based drone system. It uses flame and smoke sensors along with GPS to identify fire
and send location-based alerts to ThingSpeak. The drone continuously monitors the environment and triggers alerts during fire or smoke events.

## Components

- ESP32
- Flame Sensor (Analog)
- Smoke Sensor MQ-2 (Analog)
- GPS Module (NEO-6M)
- ThingSpeak IoT Platform
- Drone Frame / Prototype
- Jumper Wires & Power Supply

## Connections

| Component      |	ESP32 Pin   |
| ----------------------------- |
| Flame Sensor AO |	GPIO 34     |
| Smoke Sensor AO |  GPIO 35    |
| GPS TX	        |  GPIO 16    |
| GPS RX	        |  GPIO 17    |
| 3.3V / 5V	      |  3.3V / 5V  |
| GND	            |  GND        |


## How It Works

- The flame sensor detects fire intensity using analog values.
- The smoke sensor (MQ-2) detects smoke concentration.
- The GPS module fetches latitude and longitude.
  
- ESP32 checks:
- If flame value < threshold → FIRE detected
- If smoke value > threshold → SMOKE detected
- When fire/smoke is detected:
- Coordinates + sensor values are sent to ThingSpeak as an alert.
- All real-time readings are also printed on the Serial Monitor.
