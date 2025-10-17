#include "Print.h"
#include <stdlib.h>
#include <stdint.h>
#include "Adafruit_NeoPixel.h"
#include "led.h"

// uint32_t  led_get_color(uint8_t r, uint8_t g, uint8_t b, uint8_t brightness = 100)
// {
//   return led_get_color(Adafruit_NeoPixel::Color(r, g, b), brightness);
// }

// uint32_t  led_get_color(uint32_t color, uint8_t brightness = 100)
// {
//   //
// }

void      led_blink(Adafruit_NeoPixel &strip, uint32_t color, uint8_t times, uint8_t delay_ms) {
  for (uint8_t i=0; i<times; ++i) {
    strip.fill(color);
    strip.show();
    delay(delay_ms);
    strip.clear();
    strip.show();
    delay(delay_ms);
  }
}

void      led_ready(Adafruit_NeoPixel &strip) {
  strip.show();
  
  led_turn_all(strip, strip.Color(255, 0, 0));
  delay(500);
  
  led_turn_all(strip, strip.Color(0, 255, 0));
  delay(500);

  led_turn_all(strip, strip.Color(0, 0, 255));
  delay(500);

  led_turn_all(strip, strip.Color(255, 255, 255));
  delay(500);
  
  strip.clear();
  strip.show();
}

void      led_turn_all(Adafruit_NeoPixel &strip, uint32_t color, int16_t brightness) {
  strip.fill(color);

  if (brightness >= 0) {
    strip.setBrightness(brightness);
  }

  strip.show();
}

void      led_turn_all(Adafruit_NeoPixel &strip, const char *color) {
  if (strlen(color) != 6 && strlen(color) != 8) {
    return;
  }
  
  char    c[3];
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t brightness;

  strncpy(c, color, 2);
  r = strtoul(c, NULL, HEX);

  strncpy(c, color + 2, 2);
  g = strtoul(c, NULL, HEX);

  strncpy(c, color + 4, 2);
  b = strtoul(c, NULL, HEX);

  if (strlen(color) == 8) {
    strncpy(c, color + 6, 2);
    brightness = min(strtoul(c, NULL, HEX), (unsigned long)(LED_MAX_BRIGHTNESS));
  } else {
    brightness = LED_MAX_BRIGHTNESS;
  }

  strip.fill(Adafruit_NeoPixel::Color(r, g, b));
  strip.setBrightness(brightness);
  strip.show();
}

void      led_turn_rainbow(Adafruit_NeoPixel &strip) {
  strip.rainbow();
  strip.setBrightness(LED_MAX_BRIGHTNESS);
  strip.show();
}