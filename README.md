# AsyncEspAlexa

## Overview

Control ESP32 lights with Amazon Alexa using asynchronous HTTP. This header-only library implements the Hue-style API and includes utilities for color conversion.


This library is based on the [EspAlexa](https://github.com/Aircoookie/Espalexa) project, but has been
refactored to only support `ESP32` and to use the `ESPAsyncWebServer` library for asynchronous HTTP handling.


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
  jeronimonunes/AsyncEspAlexa
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

## Modules

### 1. `AsyncEspAlexaColorUtils`

Utility class for converting between color models:

* Supported conversions:

  * HSV ↔ RGB
  * RGB ↔ XY (CIE 1931)
  * RGB ↔ CT (Color Temperature in Mireds)
* Includes Alexa-specific ranges:

  * Brightness: 1–254
  * Saturation: 0–254
  * Hue: 0–65535
* Function Highlights:

  * `hsvToRgb`, `rgbToHsv`, `xyToHsv`
  * `ctToRgb`, `kelvinToRgb`
  * `isCtLikeColor(r, g, b)`: checks if a color is near grayscale
  * `rgbToCtBrightness(r, g, b)`: approximates CT and brightness from RGB

### 2. `AsyncEspAlexaDevice`

Base and derived classes modeling smart lighting devices:

* Core class: `AsyncEspAlexaDevice`

  * Properties: `id`, `name`, `on`
  * Methods: `getUniqueId()`, `encodeLightKey()`, `decodeLightKey()`
  * Callbacks: `beforeStateUpdate`, `afterStateUpdate`

* Derived device types:

  * `AsyncEspAlexaOnOffDevice`
  * `AsyncEspAlexaDimmableDevice`
  * `AsyncEspAlexaWhiteSpectrumDevice`
  * `AsyncEspAlexaColorDevice`
  * `AsyncEspAlexaExtendedColorDevice` (supports both CT and HSV)

Each device class implements:

* `getType()`, `getModelId()`, `getProductName()`
* `handleStateUpdate(JsonObject)` to parse Alexa requests
* `toJson(JsonObject)` to describe current state

### 3. `AsyncEspAlexaManager`

Main controller responsible for:

* Storing and managing device list
* Handling SSDP (Alexa discovery via multicast UDP)
* Providing a factory method for the web handler:

  ```cpp
  AsyncWebHandler* createAlexaAsyncWebHandler();
  ```
* Methods:

  * `begin()`, `loop()`
  * `addDevice(device)`
  * `reserve(n)` // reserves space for n devices, to avoid reallocations
  * `setDiscoverable(bool)`

### 4. `AsyncEspAlexaWebHandler`

Handles Alexa's HTTP requests:

* Routes supported:

  * `GET /description.xml`: returns bridge metadata
  * `GET /api/lights`: lists all devices
  * `GET /api/lights/{id}`: details of a device
  * `PUT /api/lights/{id}/state`: updates state
* Automatically converts request body to JSON and invokes appropriate callbacks
* Handles Hue-style onboarding with `devicetype`

---

## License

MIT License
