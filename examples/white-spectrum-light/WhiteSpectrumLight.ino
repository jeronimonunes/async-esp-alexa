#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "async_esp_alexa_manager.hh"
#include "async_esp_alexa_color_utils.hh"

const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";

AsyncEspAlexaManager alexaManager;
AsyncWebServer server(80);

void onWhiteChanged(bool on, uint8_t bri, uint16_t ct) {
  Serial.printf("[Device] ON: %d, Brightness: %u, CT: %u\n", on, bri, ct);
  auto rgb = AsyncEspAlexaColorUtils::ctToRgb(bri, ct);
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

  auto* light = new AsyncEspAlexaWhiteSpectrumDevice("White Light", true, 128, 300);
  light->setCallback(onWhiteChanged);

  alexaManager.reserve(1);
  alexaManager.addDevice(light);
  alexaManager.begin();

  server.addHandler(alexaManager.createAlexaAsyncWebHandler());
  server.begin();
}

void loop() {
  alexaManager.loop();
}
