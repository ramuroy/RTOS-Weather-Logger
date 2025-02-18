# RTOS Weather Logger 🌦️  

## Overview  
RTOS Weather Logger is a real-time temperature and humidity monitoring system built using **ESP32 Dev Board**, **DHT11 sensor**, and **FreeRTOS**. The system logs environmental data and sends it to the **Blynk IoT platform**, where users can monitor live readings and graphical representations of temperature and humidity.  

## Features  
✅ Real-time temperature and humidity monitoring 📊  
✅ Runs tasks concurrently using **FreeRTOS** 🔄  
✅ Remote monitoring via **Blynk IoT** 🌍  
✅ Efficient and accurate data logging with timestamps ⏳  
✅ Wi-Fi connectivity for seamless data transmission 📡  

## Components Used  
### **Hardware**  
- **ESP32 Dev Board** – Microcontroller with Wi-Fi support  
- **DHT11 Sensor** – Temperature & Humidity sensor  
- **Power Supply** – USB/Adapter for ESP32  
- **Jumper Wires** – For circuit connections  
- **Breadboard** *(optional)* – For prototyping  

### **Software & Tools**  
- **ESP-IDF (FreeRTOS)** – Development framework for ESP32  
- **Blynk IoT Platform** – Cloud-based data visualization  
- **Arduino IDE / VS Code (ESP-IDF Extension)** – For coding & debugging  

## How It Works  
- **Task 1**: Connects ESP32 to Wi-Fi and establishes Blynk communication.  
- **Task 2**: Reads **temperature & humidity** data from **DHT11** sensor and logs it.  
- **Both tasks run concurrently using FreeRTOS**, ensuring efficient multitasking.  
- Data is **sent to Blynk** for real-time monitoring with graphical representation.  

## Installation & Setup  
1. **Clone this repository**  
   ```sh
   git clone https://github.com/YourUsername/RTOS-Weather-Logger.git  
   cd RTOS-Weather-Logger  
