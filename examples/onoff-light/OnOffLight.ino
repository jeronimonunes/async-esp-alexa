#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "async_esp_alexa_manager.hh"

const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";

AsyncEspAlexaManager alexaManager;
AsyncWebServer server(80);

constexpr uint8_t LIGHT_PIN = 2; // Built-in LED on many boards

void onPowerChanged(bool on) {
  Serial.printf("[Device] ON: %d\n", on);
  digitalWrite(LIGHT_PIN, on ? HIGH : LOW);
}

void setup() {
  Serial.begin(115200);
  pinMode(LIGHT_PIN, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");

  auto* light = new AsyncEspAlexaOnOffDevice("Simple Light", false);
  light->setOnOffCallback(onPowerChanged);

  alexaManager.reserve(1);
  alexaManager.addDevice(light);
  alexaManager.begin();

  server.addHandler(alexaManager.createAlexaAsyncWebHandler());
  server.begin();
}

void loop() {
  alexaManager.loop();
}
