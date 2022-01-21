#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <stdint.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPUpdateServer.h>    // Обновление с web страницы
#ifdef ESP_USE_BUTTON
#include <GyverButton.h>
extern GButton touch;  // для сенсорной кнопки LOW_PULL
#endif
#include "constants.h"

extern uint8_t DONT_TURN_ON_AFTER_SHUTDOWN;
extern uint32_t AUTOMATIC_OFF_TIME;

// extern static const uint8_t defaultSettings[][3] PROGMEM;

extern String configSetup;

extern ESP8266HTTPUpdateServer httpUpdater;
extern ESP8266WebServer HTTP;



#define FASTLED_USE_PROGMEM 1 // просим библиотеку FASTLED экономить память контроллера на свои палитры
#include <FastLED.h>
extern CRGB leds[NUM_LEDS];

extern uint8_t selectedSettings;

extern ModeType modes[MODE_AMOUNT];

#endif
