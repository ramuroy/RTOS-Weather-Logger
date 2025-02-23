# 🌦️ ESP32 FreeRTOS Weather Logger 🌡️📊

A simple ESP32 project using FreeRTOS to log temperature and humidity data from a **DHT11** sensor and display it on the **Blynk IoT platform** with graphical representation.

## 📌 Required Components
- ESP32 (38-pin) NodeMCU Development Board
- DHT11 Temperature & Humidity Sensor
- Jumper Wires
- Breadboard
- **ESP-IDF FreeRTOS Software**

## 🔧 Wiring Diagram
| ESP32 GPIO Pin | Component |
|--------------|-----------|
| GPIO 4      | DHT11 Data |
| 3.3V        | DHT11 VCC  |
| GND         | DHT11 GND  |

## 📝 Code
The FreeRTOS-based code creates a task that reads temperature and humidity data from the **DHT11 sensor** and sends it to the **Blynk IoT platform**.

```c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "dht.h"
#include "BlynkSimpleEsp32.h"

#define DHT_PIN 4
#define BLYNK_AUTH_TOKEN "Your_Blynk_Auth_Token"

void dht_task(void *pvParameter) {
    int16_t temperature, humidity;
    while (1) {
        if (dht_read_data(DHT_TYPE_DHT11, DHT_PIN, &humidity, &temperature) == ESP_OK) {
            printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);
            Blynk.virtualWrite(V0, temperature);
            Blynk.virtualWrite(V1, humidity);
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

void app_main() {
    Blynk.begin(BLYNK_AUTH_TOKEN, "Your_WiFi_SSID", "Your_WiFi_Password");
    xTaskCreate(&dht_task, "DHT Sensor Task", 2048, NULL, 5, NULL);
}
```

## 🚀 How to Run
1. Install **ESP-IDF** and set up the development environment.
2. Clone this repository and navigate to the project folder.
3. Replace `Your_Blynk_Auth_Token`, `Your_WiFi_SSID`, and `Your_WiFi_Password` with your actual Blynk credentials.
4. Build and flash the code using:
   ```sh
   idf.py build
   idf.py flash
   idf.py monitor
   ```
5. Open the **Blynk app** and add widgets to display temperature (`V0`) and humidity (`V1`).

## 📽️ Demo Video
🔗 **[Click here to watch the demonstration on LinkedIn](https://www.linkedin.com/posts/ramu-roy-b780382b7_%F0%9D%97%A5%F0%9D%97%A7%F0%9D%97%A2%F0%9D%97%A6-%F0%9D%97%AA%F0%9D%97%B2%F0%9D%97%AE%F0%9D%98%81%F0%9D%97%B5%F0%9D%97%B2%F0%9D%97%BF-%F0%9D%97%9F%F0%9D%97%BC%F0%9D%97%B4%F0%9D%97%B4%F0%9D%97%B2%F0%9D%97%BF-activity-7298553424588029953-Kclr?utm_source=social_share_send&utm_medium=member_desktop_web&rcm=ACoAAEwAX4wBY70YZ3l58lvkiXtyCZcnWWrfJAA)**

## 🛠️ License
This project is open-source and available under the MIT License.

---
✨ Happy coding with ESP32 & FreeRTOS! 🚀
