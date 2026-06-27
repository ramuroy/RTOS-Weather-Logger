/*
 * RTOS Weather Logger — ESP32 + FreeRTOS + Blynk
 * -------------------------------------------------------------
 * A FreeRTOS task samples a DHT11 every few seconds and publishes the
 * latest reading into a length-1 FreeRTOS queue. The main task runs the
 * Blynk client and a BlynkTimer that pushes the most recent sample to the
 * Blynk dashboard. Keeping every Blynk call on the main task makes the
 * cross-task hand-off thread-safe (Blynk itself is not re-entrant).
 *
 * Framework : Arduino-ESP32 (FreeRTOS is the underlying scheduler)
 * Board     : ESP32 Dev Module (38-pin NodeMCU)
 * Libraries : Blynk, DHT sensor library (+ Adafruit Unified Sensor)
 *
 * Fill in the credentials below before flashing. For a public repo prefer
 * moving them into a separate, git-ignored header.
 */

// ---- Blynk IoT identifiers (Blynk.Console > Device Info) ----
#define BLYNK_TEMPLATE_ID   "YourTemplateID"
#define BLYNK_TEMPLATE_NAME "RTOS Weather Logger"
#define BLYNK_AUTH_TOKEN    "YourAuthToken"

// Route Blynk's diagnostic prints to the serial monitor.
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

// ---- Wi-Fi credentials ----
char ssid[] = "YourWiFiSSID";
char pass[] = "YourWiFiPassword";

// ---- DHT sensor ----
#define DHT_PIN  4
#define DHT_TYPE DHT11
DHT dht(DHT_PIN, DHT_TYPE);

// ---- Blynk virtual pins (must match the widgets in the Blynk app) ----
#define VIRTUAL_PIN_TEMP V5
#define VIRTUAL_PIN_HUM  V6

// Sampling/upload period, shared by the sensor task and the upload timer.
static const uint32_t SAMPLE_PERIOD_MS = 2000;

// One reading shared between the sensor task (producer) and the Blynk
// upload timer (consumer). A length-1 "overwrite" queue always holds the
// freshest sample and is inherently thread-safe.
typedef struct {
  float temperature;  // Celsius
  float humidity;     // %RH
} Reading;

static QueueHandle_t readingQueue;
static BlynkTimer timer;

// FreeRTOS task: sample the DHT11 and publish the latest valid reading.
void sensorTask(void *pvParameters) {
  for (;;) {
    Reading r;
    r.temperature = dht.readTemperature();  // Celsius
    r.humidity    = dht.readHumidity();

    // The DHT library returns NaN on a failed/CRC-bad read — never forward
    // those to the dashboard, or the graphs fill with gaps/garbage.
    if (isnan(r.temperature) || isnan(r.humidity)) {
      Serial.println("DHT read failed; skipping sample");
    } else {
      Serial.printf("Temp: %.1f C, Hum: %.1f %%\n", r.temperature, r.humidity);
      xQueueOverwrite(readingQueue, &r);  // keep only the freshest sample
    }

    vTaskDelay(pdMS_TO_TICKS(SAMPLE_PERIOD_MS));
  }
}

// Runs on the main (Blynk-safe) task: push the most recent sample, if any.
void uploadReading() {
  Reading r;
  if (xQueuePeek(readingQueue, &r, 0) == pdTRUE) {
    Blynk.virtualWrite(VIRTUAL_PIN_TEMP, r.temperature);
    Blynk.virtualWrite(VIRTUAL_PIN_HUM, r.humidity);
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  readingQueue = xQueueCreate(1, sizeof(Reading));

  // Blynk.begin() brings up Wi-Fi and the Blynk cloud connection.
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Upload the latest reading from the main task on a fixed cadence.
  timer.setInterval(SAMPLE_PERIOD_MS, uploadReading);

  // Sensor sampling runs in its own FreeRTOS task.
  xTaskCreate(sensorTask, "sensorTask", 4096, NULL, 5, NULL);
}

void loop() {
  Blynk.run();
  timer.run();
}
