#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "BlynkSimpleEsp32.h"
#include "DHT.h"

// Wi-Fi credentials and Blynk authentication token
#define WIFI_SSID "YourWiFiSSID"
#define WIFI_PASSWORD "YourWiFiPassword"
#define BLYNK_AUTH_TOKEN "YourAuthToken"

// DHT11 sensor pin and type
#define DHT_PIN GPIO_NUM_4
#define DHT_TYPE DHT11

// Blynk virtual pins
#define VIRTUAL_PIN_TEMP V5
#define VIRTUAL_PIN_HUM V6

// DHT11 object initialization
DHT dht(DHT_PIN, DHT_TYPE);

// Task for reading and sending sensor data
void weather_task(void *pvParameter)
{
    float temperature, humidity;

    while (1) {
        // Read temperature and humidity
        temperature = dht.readTemperature();
        humidity = dht.readHumidity();

        // Print the values to the serial monitor
        ESP_LOGI("WeatherTask", "Temp: %.2f °C, Hum: %.2f %%", temperature, humidity);

        // Send the values to Blynk app using virtual pins
        Blynk.virtualWrite(VIRTUAL_PIN_TEMP, temperature);
        Blynk.virtualWrite(VIRTUAL_PIN_HUM, humidity);

        // Delay for 2 seconds
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

// Wi-Fi initialization function
esp_err_t wifi_init_sta(void)
{
    esp_wifi_init(NULL);
    esp_wifi_set_storage(WIFI_STORAGE_FLASH);
    esp_wifi_set_mode(WIFI_MODE_STA);

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASSWORD,
        },
    };

    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
    esp_wifi_start();

    return ESP_OK;
}

void app_main(void)
{
    // Initialize NVS (Non-Volatile Storage) for Wi-Fi
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Initialize Wi-Fi
    ESP_ERROR_CHECK(wifi_init_sta());

    // Initialize Blynk
    Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASSWORD);

    // Initialize DHT sensor
    dht.begin();

    // Create FreeRTOS task to read and send data
    xTaskCreate(weather_task, "weather_task", 2048, NULL, 5, NULL);

    // Run Blynk event loop
    while (1) {
        Blynk.run();
    }
}
