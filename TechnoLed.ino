#include <HTTPUpdateServer.h>
#include <MQTT.h>
#include <WebServer.h>
#include <WiFi.h>

#include "config.h"
#include "led.h"

// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
HTTPUpdateServer  httpUpdater;
MQTTClient        mqttClient;
WiFiClient        wifiClient;
WebServer         httpServer(80);

bool wifiConnect() {
  Serial.print("[WiFi] Connecting to ");
  Serial.println(wifiSsid);
  WiFi.setHostname(hostname);
  WiFi.begin(wifiSsid, wifiKey);

  // Will try for about 10 seconds (20x 500ms)
  int tryDelay = 500;
  int numberOfTries = 20;

  // Wait for the WiFi event
  while (true) {
    switch (WiFi.status()) {
      case WL_NO_SSID_AVAIL: Serial.println("[WiFi] SSID not found"); break;
      case WL_CONNECT_FAILED:
        Serial.print("[WiFi] Failed - WiFi not connected! Reason: ");
        return false;
        break;
      case WL_CONNECTION_LOST: Serial.println("[WiFi] Connection was lost"); break;
      case WL_SCAN_COMPLETED:  Serial.println("[WiFi] Scan is completed"); break;
      case WL_DISCONNECTED:    Serial.println("[WiFi] WiFi is disconnected"); break;
      case WL_CONNECTED:
        Serial.println("[WiFi] WiFi is connected!");
        Serial.print("[WiFi] IP address: ");
        Serial.println(WiFi.localIP());
        return true;
        break;
      default:
        Serial.print("[WiFi] Status: ");
        Serial.println(WiFi.status());
        break;
    }

    delay(tryDelay);

    if (numberOfTries <= 0) {
      Serial.print("[WiFi] Failed to connect to WiFi!");
      // Use disconnect function to force stop trying to connect
      WiFi.disconnect();

      return false;
    } else {
      --numberOfTries;
    }
  }

  return false;
}

void messageReceived(const String &topic, const String &payload) {
  Serial.println("Incoming from " + topic + " : " + payload);
  
  const char ctrl = payload.charAt(0);
  
  switch (ctrl) {
    case '#':
      led_turn_all(strip, payload.substring(1).c_str());
      break;
    case '*':
      led_turn_rainbow(strip);
      break;
    case '1':
      led_turn_all(strip, 0xFFFFFF);
      break;
    case '0':
      led_turn_all(strip, (uint32_t) 0);
      break;
    default:
      return;
  }

  // Note: Do not use the client in the callback to publish, subscribe or
  // unsubscribe as it may cause deadlocks when other things arrive while
  // sending and receiving acknowledgments. Instead, change a global variable,
  // or push to a queue and handle it in the loop after calling `mqttClient.loop()`.
}
  
bool mqttConnect() {
  int tryDelay = 1000;
  int numberOfTries = 60;

  mqttClient.begin(mqttHost, 1883, wifiClient);
  mqttClient.onMessage(messageReceived);

  Serial.print("[MQTT] Connecting to ");
  Serial.print(mqttHost);
  while (numberOfTries > 0) {
    if (mqttClient.connect(hostname, mqttUser, mqttPass)) {
      Serial.println(" Connected");
      break;
    }

    Serial.print(".");
    --numberOfTries;
    delay(tryDelay);
  }

  Serial.print("\n");

  if (mqttClient.connected()) {
    String mqttWill("> ");
    mqttWill.concat(hostname);
    mqttWill.concat(" disconnected");
    mqttClient.setWill(mqttWill.c_str());

    Serial.print("[MQTT] Sending state to ");
    Serial.print(stateTopic);
    Serial.print(": ");
    if (mqttClient.publish(stateTopic, String("> ") + hostname + " connected")) {
      Serial.println("OK");
    } else {
      Serial.println("KO");
      return false;
    }

    Serial.print("[MQTT] Subscribing to ");
    Serial.print(ledsTopic);
    Serial.print(": ");
    if (mqttClient.subscribe(ledsTopic, 1)) {
      Serial.println("OK");
    } else {
      Serial.println("KO");
      return false;
    }

    return true;
  }

  Serial.println("[MQTT] Couldn't connect");
  return false;
}

void setup() {
    // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  Serial.begin(115200);
  delay(10);

  Serial.println("Serial ready\n");

  // LEDs connection
  strip.begin();                  // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.setBrightness(255 * 0.2); // 70% as hardware value for brightness because of consumption
  led_ready(strip);

  // WiFi Connection
  if (!wifiConnect()) {
    Serial.println("No connection, hanging there");
    while(true);
  }

  Serial.print("\n");

  // MQTT Connection
  if (!mqttConnect()) {
    Serial.println("No connection, hanging there");
    while(true);
  }

  Serial.print("\n");

  // OTA Updater
  httpUpdater.setup(&httpServer);
  httpServer.begin();
  Serial.printf("OTA Update ready on http://%s/update or curl -F 'image=@firmware.bin' http://%s/update\n", WiFi.localIP(), WiFi.localIP());

  Serial.print("\n");

  // // LEDs connection
  // FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  // for (unsigned int i=0; i<NUM_LEDS; ++i) {
  //   leds[i] = CRGB(50, 50, 50);
  // }
}

void loop() {
  httpServer.handleClient();
  mqttClient.loop();

  // if (!mqttClient.connected()) {
  //   wifiConnect();
  // }

  // publish a message roughly every second.
  // if (millis() - lastMillis > 1000) {
  //   lastMillis = millis();
  //   mqttClient.publish(ledsTopic, "Hello");
  //   mqttClistateTublish(ledsTopic, devices);
  // }
}
