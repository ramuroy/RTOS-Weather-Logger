# 🌦️ ESP32 FreeRTOS Weather Logger 🌡️📊

![ESP32](https://img.shields.io/badge/ESP32-E7352C?style=flat-square&logo=espressif&logoColor=white)
![Arduino](https://img.shields.io/badge/Arduino--ESP32-00979D?style=flat-square&logo=arduino&logoColor=white)
![FreeRTOS](https://img.shields.io/badge/FreeRTOS-task%20%2B%20queue-4FA94D?style=flat-square)
![Blynk](https://img.shields.io/badge/IoT-Blynk-2E6BA8?style=flat-square)
![License](https://img.shields.io/badge/License-MIT-3da639?style=flat-square)

A simple ESP32 project that uses **FreeRTOS** to log temperature and humidity from a **DHT11** sensor and display it on the **Blynk IoT platform** with live graphs.

A dedicated FreeRTOS task samples the sensor and pushes the latest reading into a length-1 FreeRTOS queue; the main task runs the Blynk client and uploads that reading on a timer. Keeping every Blynk call on a single task makes the cross-task hand-off thread-safe.

## 📌 Required Components & Software
- ESP32 (38-pin) NodeMCU Development Board
- DHT11 Temperature & Humidity Sensor
- Jumper wires + breadboard
- **Arduino IDE** (or `arduino-cli`) with:
  - the **ESP32 board package** (`esp32` by Espressif)
  - the **Blynk** library
  - the **DHT sensor library** (Adafruit) + its **Adafruit Unified Sensor** dependency

## 🔧 Wiring Diagram
| ESP32 GPIO Pin | Component |
|--------------|-----------|
| GPIO 4 | DHT11 Data |
| 3.3V   | DHT11 VCC  |
| GND    | DHT11 GND  |

## 📝 Code
The full sketch lives in [`RTOS-Weather-Logger.ino`](RTOS-Weather-Logger.ino). The core of it: a FreeRTOS task reads the DHT11 (skipping failed `NaN` reads) and publishes to a length-1 queue, while a `BlynkTimer` on the main task uploads the freshest sample.

```cpp
// FreeRTOS task: sample the DHT11 and publish the latest valid reading.
void sensorTask(void *pvParameters) {
  for (;;) {
    Reading r;
    r.temperature = dht.readTemperature();   // Celsius
    r.humidity    = dht.readHumidity();

    if (isnan(r.temperature) || isnan(r.humidity)) {
      Serial.println("DHT read failed; skipping sample");
    } else {
      xQueueOverwrite(readingQueue, &r);     // keep only the freshest sample
    }
    vTaskDelay(pdMS_TO_TICKS(SAMPLE_PERIOD_MS));
  }
}

// Runs on the main (Blynk-safe) task: push the most recent sample.
void uploadReading() {
  Reading r;
  if (xQueuePeek(readingQueue, &r, 0) == pdTRUE) {
    Blynk.virtualWrite(V5, r.temperature);
    Blynk.virtualWrite(V6, r.humidity);
  }
}

void loop() {
  Blynk.run();
  timer.run();
}
```

## 🚀 How to Run
1. Install the **ESP32 board package** in the Arduino IDE (Boards Manager → "esp32"), plus the **Blynk** and **DHT sensor library** packages (Library Manager).
2. Open `RTOS-Weather-Logger.ino` and fill in your `BLYNK_TEMPLATE_ID`, `BLYNK_TEMPLATE_NAME`, `BLYNK_AUTH_TOKEN`, Wi-Fi `ssid`, and `pass`.
3. Select **Tools → Board → ESP32 Dev Module**, pick the serial port, and click **Upload**.

   Or from the command line with `arduino-cli`:
   ```sh
   arduino-cli compile --fqbn esp32:esp32:esp32 .
   arduino-cli upload  --fqbn esp32:esp32:esp32 -p /dev/ttyUSB0 .
   arduino-cli monitor -p /dev/ttyUSB0 -c baudrate=115200
   ```
4. In the **Blynk app**, add a widget bound to **V5** (temperature) and another to **V6** (humidity).

## 📽️ Demo Video
🔗 **[Click here to watch the demonstration on LinkedIn](https://www.linkedin.com/posts/ramu-roy-b780382b7_%F0%9D%97%A5%F0%9D%97%A7%F0%9D%97%A2%F0%9D%97%A6-%F0%9D%97%AA%F0%9D%97%B2%F0%9D%97%AE%F0%9D%98%81%F0%9D%97%B5%F0%9D%97%B2%F0%9D%97%BF-%F0%9D%97%9F%F0%9D%97%BC%F0%9D%97%B4%F0%9D%97%B4%F0%9D%97%B2%F0%9D%97%BF-activity-7298553424588029953-Kclr?utm_source=social_share_send&utm_medium=member_desktop_web&rcm=ACoAAEwAX4wBY70YZ3l58lvkiXtyCZcnWWrfJAA)**

## 🛠️ License
This project is open-source and available under the MIT License.

---
✨ Happy coding with ESP32 & FreeRTOS! 🚀
