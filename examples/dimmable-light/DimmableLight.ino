#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "async_esp_alexa_manager.hh"

const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";

AsyncEspAlexaManager alexaManager;
AsyncWebServer server(80);

void onBrightnessChanged(bool on, uint8_t bri) {
  Serial.printf("[Device] ON: %d, Brightness: %u\n", on, bri);
  // analogWrite() or other PWM control can be used here
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");

  auto* light = new AsyncEspAlexaDimmableDevice("Dimmable Light", true, 128);
  light->setBrightnessCallback(onBrightnessChanged);

  alexaManager.reserve(1);
  alexaManager.addDevice(light);
  alexaManager.begin();

  server.addHandler(alexaManager.createAlexaAsyncWebHandler());
  server.begin();
}

void loop() {
  alexaManager.loop();
}
