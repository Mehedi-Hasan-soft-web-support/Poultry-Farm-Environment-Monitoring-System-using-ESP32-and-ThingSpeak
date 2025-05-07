# Poultry-Farm-Environment-Monitoring-System-using-ESP32-and-ThingSpeak
An IoT-based smart poultry monitoring system using ESP32, DHT22, MQ7, and MQ135 sensors to collect and visualize temperature, humidity, and gas levels on ThingSpeak with LCD display and relay automation.

# Poultry Farm Environment Monitoring System 🐔🌡️

This project implements an IoT-based environment monitoring system for poultry farms using an ESP32 microcontroller. The system measures **temperature**, **humidity**, **carbon monoxide (CO)**, and **air quality (toxic gases)** using DHT22, MQ7, and MQ135 sensors, respectively. Data is displayed on an **I2C LCD** and uploaded to **ThingSpeak** for real-time cloud monitoring and analysis.

---

## 🚀 Features

- 📶 Wi-Fi-based remote monitoring
- 🌡️ Temperature and humidity sensing with DHT22
- 🛢️ Gas detection using MQ7 (CO) and MQ135 (air quality)
- 📊 Real-time data logging to ThingSpeak
- 🖥️ LCD display with sensor readings
- ⚙️ Relay-based actuator triggered by air quality threshold
- 🔄 Periodic sensor averaging for stability

---

## 🛠️ Components Used

| Component       | Description                    |
|----------------|--------------------------------|
| ESP32 Dev Board| Wi-Fi enabled microcontroller  |
| DHT22          | Temperature & humidity sensor  |
| MQ7            | Carbon monoxide sensor         |
| MQ135          | Air quality sensor             |
| LCD 16x2 (I2C) | For displaying sensor data     |
| Relay Module   | To trigger external systems    |
| LED            | Wi-Fi/Status indicator         |

---

## 🌐 ThingSpeak Configuration

- Create a new ThingSpeak channel with **4 fields**:
  1. Temperature (°C)
  2. Humidity (%)
  3. MQ7 Sensor Value
  4. MQ135 Sensor Value
- Copy your **Write API Key** and **Channel ID** and update them in the code:
```cpp
const char* thingSpeakApiKey = "YOUR_API_KEY";
unsigned long channelNumber = YOUR_CHANNEL_ID;


## 🔧 How to Use

1. Upload the code to ESP32 using the Arduino IDE.
2. Install required libraries:

   * `WiFi.h`
   * `ThingSpeak.h`
   * `DHT.h`
   * `LiquidCrystal_I2C.h` (by Frank de Brabander)
3. Connect sensors as per the defined pins in code.
4. Power up the ESP32 and check serial monitor for logs.
5. View live data on your ThingSpeak channel!

---


