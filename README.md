# RTOS Weather Logger 🌦️  

## Overview  
RTOS Weather Logger is a temperature and humidity monitoring system built using **ESP32 Dev Board**, **DHT11 sensor**, and **FreeRTOS**. The system reads environmental data and sends it to the **Blynk IoT platform**, allowing real-time remote monitoring with graphical representation.  

## Features  
- Real-time **temperature and humidity** monitoring 📊  
- **Concurrent task execution** using **FreeRTOS** 🔄  
- **Wi-Fi-enabled** remote monitoring via **Blynk IoT** 🌍  
- Efficient **data logging with timestamps** ⏳  

## Components Used  
- **ESP32 Dev Board** – Microcontroller with Wi-Fi support  
- **DHT11 Sensor** – Temperature & Humidity sensor  
- **Blynk IoT Platform** – For real-time data visualization  
- **FreeRTOS (ESP-IDF)** – For multitasking and scheduling  

## How It Works  
- **Task 1:** Connects ESP32 to Wi-Fi and establishes Blynk communication.  
- **Task 2:** Reads **temperature & humidity** from the **DHT11** sensor and logs the data.  
- **Both tasks run concurrently** using **FreeRTOS** for efficient multitasking.  
- The data is displayed on the **Blynk app** with real-time graphical representation.  

## License  
This project is **open-source** and available under the **MIT License**.  

---

💡 *Feel free to contribute and improve this project!* 😊  
