#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "async_esp_alexa_manager.hh"
#include "async_esp_alexa_color_utils.hh"

const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";

// Create Alexa Manager
AsyncEspAlexaManager alexaManager;
AsyncWebServer server(80);

// Define device callback
void onColorChanged(bool on, uint8_t bri, uint16_t hue, uint8_t sat) {
  Serial.printf("[Device] ON: %d, Brightness: %u, Hue: %u, Sat: %u\n",
                on, bri, hue, sat);

  auto [r, g, b] = AsyncEspAlexaColorUtils::hsvToRgb(hue, sat, bri);
  Serial.printf("[RGB] R: %u, G: %u, B: %u\n", r, g, b);

  // Add your hardware control using RGB values here
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");

  // Create a color-capable device
  auto* colorDevice = new AsyncEspAlexaColorDevice("Living Room Light", true, 128, 30000, 200);
  colorDevice->setColorCallback(onColorChanged);

  // Add device to manager
  alexaManager.reserve(1);
  alexaManager.addDevice(colorDevice);
  alexaManager.begin();

  // Attach handler to web server
  server.addHandler(alexaManager.createAlexaAsyncWebHandler());
  server.begin();
}

void loop() {
  alexaManager.loop();
}
