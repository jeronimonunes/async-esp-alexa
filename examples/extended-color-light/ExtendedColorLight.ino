#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "async_esp_alexa_manager.hh"
#include "async_esp_alexa_color_utils.hh"

const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";

AsyncEspAlexaManager alexaManager;
AsyncWebServer server(80);

void onCtChanged(bool on, uint8_t bri, uint16_t ct) {
  Serial.printf("[CT] ON:%d Bri:%u CT:%u\n", on, bri, ct);
}

void onColorChanged(bool on, uint8_t bri, uint16_t hue, uint8_t sat) {
  Serial.printf("[HSV] ON:%d Bri:%u Hue:%u Sat:%u\n", on, bri, hue, sat);
  auto rgb = AsyncEspAlexaColorUtils::hsvToRgb(hue, sat, bri);
  Serial.printf("[RGB] R:%u G:%u B:%u\n", rgb[0], rgb[1], rgb[2]);
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");

  using Device = AsyncEspAlexaExtendedColorDevice;
  auto* light = new Device(
    "Extended Light", true, 128, 30000, 200, 370, Device::ColorMode::ct);
  light->setColorCallback(onColorChanged);
  light->setColorTemperatureCallback(onCtChanged);

  alexaManager.reserve(1);
  alexaManager.addDevice(light);
  alexaManager.begin();

  server.addHandler(alexaManager.createAlexaAsyncWebHandler());
  server.begin();
}

void loop() {
  alexaManager.loop();
}
