# TechnoLED

Firmware for ESP32-based LED controller with WiFi, MQTT, and OTA update support. Designed for remote lighting control via MQTT messages, handling both static and dynamic LED patterns at ITIC Paris school.


## Overview

**TechnoLED**  is an embedded system project built for the ESP32 platform. It provides the following capabilities:
- WiFi connectivity and automatic reconnection handling
- MQTT communication for LED state updates and remote control
- OTA (Over-The-Air) firmware updates via HTTP
- NeoPixel LED strip management (e.g., WS2812-based LEDs)
- Basic diagnostics over serial output


## Features

- **WiFi**
  - Connects using provided SSID and key
  - Displays IP address and connection status on serial monitor

- **MQTT**
  - Publishes device state to  `/devices`
  - Subscribes to a LED control topic (e.g.,  `/room/11/leds`)
  - Handles control payloads for LED behavior

- **OTA Update Server**
  - Accessible via: `http://<device_ip>/update`
  - Allows firmware upload through browser or  `curl -F 'image=@firmware.bin' http://<device_ip>/update`

- **LED Control**
  - `#RRGGBB`  → Set all LEDs to specified color
  - `*`  → Activate rainbow pattern
  - `I`  → Turn all LEDs on (white)
  - `O`  → Turn all LEDs off


## Configuration

Modify the following constants before compiling:

```c
#define LED_PIN   IO4 // <- TO FILL
#define LED_COUNT 300 // <- TO FILL
```

```c
const char *hostname = "esp32_room_XX";
const char *wifiSsid = "<your_wifi_ssid>";
const char *wifiKey  = "<your_wifi_password>";
const char *mqttHost = "<mqtt_broker_ip>";
const char *mqttUser = "<mqtt_username>";
const char *mqttPass = "<mqtt_password>";
```

Topics can be customized:

```c
const char *ledsTopic = "/room/XX/leds";
const char *stateTopic = "/devices";
```


## Hardware

- **Board**: ESP32-ETH01
- **LEDs**: WS2812B pixel strip
- **Power adapter**: AC/DC 230v to 5v, 80W (for 22m strip)
- **Pinout**: Only 1 pin is necessary for the data pin
- **Wires**, **breadboards**, …


## Build and Flash

1. Open project in Arduino IDE
2. Download and Select the ESP32 board type (WT32-ETH01)
3. Adjust configuration values
4. Compile and upload firmware via USB
5. Once connected, note IP address from serial output
6. Optionally, use the OTA endpoint for future updates


## Dependencies

Install these Arduino libraries:
- `WiFi.h`
- `WebServer.h`
- `HTTPUpdateServer.h`
- `MQTT.h`
- `Adafruit_NeoPixel.h`