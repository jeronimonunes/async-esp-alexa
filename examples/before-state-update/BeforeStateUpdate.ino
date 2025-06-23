#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "async_esp_alexa_manager.hh"

const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";

AsyncEspAlexaManager alexaManager;
AsyncWebServer server(80);

constexpr uint8_t SWITCH_PIN = 2; // Example output pin

void syncState(AsyncEspAlexaDevice* device) {
  auto* light = static_cast<AsyncEspAlexaOnOffDevice*>(device);
  bool state = digitalRead(SWITCH_PIN);
  light->setOn(state);
}

void onPowerChanged(bool on) {
  digitalWrite(SWITCH_PIN, on ? HIGH : LOW);
  Serial.printf("[Device] ON: %d\n", on);
}

void setup() {
  Serial.begin(115200);
  pinMode(SWITCH_PIN, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");

  auto* light = new AsyncEspAlexaOnOffDevice("Synced Light", false);
  light->setOnOffCallback(onPowerChanged);
  light->setBeforeStateUpdateCallback(syncState);

  alexaManager.reserve(1);
  alexaManager.addDevice(light);
  alexaManager.begin();

  server.addHandler(alexaManager.createAlexaAsyncWebHandler());
  server.begin();
}

void loop() {
  alexaManager.loop();
}
