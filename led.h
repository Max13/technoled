#ifndef   LED_H
# define  LED_H

# define  LED_MAX_BRIGHTNESS  255 * 0.7

# include <stdint.h>

// NEOPIXEL BEST PRACTICES for most reliable operation:
// - Add 1000 uF CAPACITOR between NeoPixel strip's + and - connections.
// - MINIMIZE WIRING LENGTH between microcontroller board and first pixel.
// - NeoPixel strip's DATA-IN should pass through a 300-500 OHM RESISTOR.
// - AVOID connecting NeoPixels on a LIVE CIRCUIT. If you must, ALWAYS
//   connect GROUND (-) first, then +, then data.
// - When using a 3.3V microcontroller with a 5V-powered NeoPixel strip,
//   a LOGIC-LEVEL CONVERTER on the data line is STRONGLY RECOMMENDED.
// (Skipping these may work OK on your workbench but can fail in the field)

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

void  led_blink(Adafruit_NeoPixel &strip, uint32_t color, uint8_t times, uint8_t delay_ms = 250);
void  led_ready(Adafruit_NeoPixel &strip);
void  led_turn_all(Adafruit_NeoPixel &strip, uint32_t color, int16_t brightness = -1);
void  led_turn_all(Adafruit_NeoPixel &strip, const char *color);
void  led_turn_rainbow(Adafruit_NeoPixel &strip);

#endif