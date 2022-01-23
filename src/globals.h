#ifndef GLOBALS_H_
#define GLOBALS_H_

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

extern uint32_t eepromTimeout;
extern uint8_t currentMode;
extern bool loadingFlag;
extern bool ONflag;

extern uint8_t random_on;
extern uint32_t my_timer;
extern uint8_t espMode;

extern uint8_t Favorit_only;
extern uint8_t ESP_CONN_TIMEOUT;
extern uint8_t time_always;

extern bool settChanged;
extern bool buttonEnabled; // это важное первоначальное значение. нельзя делать false по умолчанию
extern unsigned char matrixValue[8][16]; //это массив для эффекта Огонь

extern const uint8_t maxDim;

extern char TextTicker [80];

extern String AP_NAME;
extern String AP_PASS;
extern String LAMP_NAME;

extern bool connect;
extern int Painting;

String jsonRead(String& json, String name);
int jsonReadtoInt(String& json, String name);
void jsonWrite(String& json, String name, String value);
void jsonWrite(String& json, String name, int value);
void jsonWrite(String& json, String name, uint8_t value);
void writeFile(String fileName, String strings);
void saveConfig();
String readFile(String fileName, size_t len);


#endif
