# AsyncEspAlexa - Documentation

## Overview

`AsyncEspAlexa` is a C++ library designed to enable ESP32-based devices to interact with Amazon Alexa using the Hue-compatible API. It leverages asynchronous networking (via `ESPAsyncWebServer`) for performance and modular design.

This library is based on the [EspAlexa](https://github.com/Aircoookie/Espalexa) project, but has been
refactored to only support `ESP32` and to use the `ESPAsyncWebServer` library for asynchronous HTTP handling.
---

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

## Configuration

### `library.json`

Metadata and dependencies for PlatformIO:

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

---

## License

MIT License
