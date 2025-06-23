# AsyncEspAlexa

Control ESP32 lights with Amazon Alexa using asynchronous HTTP. This header-only library implements the Hue-style API and includes utilities for color conversion.

## Features

- Hue-compatible API (on/off, brightness, color temperature and HSV color)
- Asynchronous networking using ESPAsyncWebServer
- Built-in color conversion helpers
- Simple device classes for common light types
- SSDP discovery for Alexa

## Requirements

- ESP32 board with WiFi
- Arduino framework (PlatformIO or Arduino IDE)
- Dependencies:
  - ESPAsyncWebServer
  - AsyncTCP
  - ArduinoJson

## Installation

Use **PlatformIO**:

```ini
lib_deps =
  https://github.com/jeronimonunes/async-esp-alexa.git
```

Or clone this repository into your Arduino `libraries` folder.

## Quick Start

The `examples/rgb-light` sketch demonstrates how to expose a color light:

```cpp
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "async_esp_alexa_manager.hh"
#include "async_esp_alexa_color_utils.hh"

AsyncEspAlexaManager alexa;
AsyncWebServer server(80);

void onColorChanged(bool on, uint8_t bri, uint16_t hue, uint8_t sat) {
  auto [r,g,b] = AsyncEspAlexaColorUtils::hsvToRgb(hue, sat, bri);
  // Add hardware control using r,g,b
}

void setup() {
  WiFi.begin("ssid", "password");
  while (WiFi.status() != WL_CONNECTED) delay(500);

  auto *lamp = new AsyncEspAlexaColorDevice("Lamp", true, 128, 30000, 200);
  lamp->setColorCallback(onColorChanged);

  alexa.reserve(1);
  alexa.addDevice(lamp);
  alexa.begin();

  server.addHandler(alexa.createAlexaAsyncWebHandler());
  server.begin();
}

void loop() {
  alexa.loop();
}
```

Upload the sketch and ask Alexa to discover devices.

## Library Components

### Color Utils
- HSV ↔ RGB, XY and color temperature conversions
- Alexa ranges: brightness 1–254, hue 0–65535

### Device Classes
- On/off, dimmable, white spectrum, color, and extended color lights
- JSON parsing and serialization helpers

### Manager & Web Handler
- Handles SSDP discovery
- Provides the HTTP endpoints required by Alexa

## Configuration

The `library.json` file lists dependencies and metadata:

```json
{
  "name": "AsyncEspAlexa",
  "version": "1.0.0",
  "frameworks": "Arduino",
  "platforms": "espressif32",
  "dependencies": {
    "bblanchon/ArduinoJson": "^7.4.2",
    "ESP32Async/AsyncTCP": "^3.4.4",
    "ESP32Async/ESPAsyncWebServer": "^3.7.8"
  }
}
```

## License

MIT License
