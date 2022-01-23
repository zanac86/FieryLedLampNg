#include "globals.h"
#include "constants.h"

#include "favoritesmanager.h"
#include "timermanager.h"

#include <ArduinoJson.h>        //Установить из менеджера библиотек версию 5.13.5 !!!. https://arduinojson.org/
#include <ESP8266WiFi.h>
#include <LittleFS.h>
#define SPIFFS LittleFS


// --- ИНИЦИАЛИЗАЦИЯ ОБЪЕКТОВ ----------
CRGB leds[NUM_LEDS];

uint8_t selectedSettings = 0U;


#ifdef ESP_USE_BUTTON
#if (BUTTON_IS_SENSORY == 1)
GButton touch(BTN_PIN, LOW_PULL, NORM_OPEN);  // для сенсорной кнопки LOW_PULL
#else
GButton touch(BTN_PIN, HIGH_PULL, NORM_OPEN); // для физической (не сенсорной) кнопки HIGH_PULL. ну и кнопку нужно ставить без резистора в разрыв между пинами D2 и GND
#endif
#endif


ESP8266HTTPUpdateServer httpUpdater;  // Объект для обнавления с web страницы
ESP8266WebServer HTTP(ESP_HTTP_PORT); //ESP8266WebServer HTTP;  // Web интерфейс для устройства


// --- ИНИЦИАЛИЗАЦИЯ ПЕРЕМЕННЫХ -------

// а также если у вас выбрано меньше десятка эффектов. кстати, если выбрано менее 2 эффектов, то демонстрироваться будут все эффекты по порядку без перемешивания
uint8_t DONT_TURN_ON_AFTER_SHUTDOWN;                        // Не удаляйте и не комментируйте эту строку
uint32_t AUTOMATIC_OFF_TIME = (5 * 60UL * 60UL * 1000UL);   // Не удаляйте и не комментируйте эту строку


const uint8_t maxDim = max(WIDTH, HEIGHT);
uint32_t thisTime;
bool manualOff = false;
uint8_t currentMode = 3;
bool loadingFlag = true;
bool ONflag = false;
uint32_t eepromTimeout;
bool settChanged = false;
bool buttonEnabled = true; // это важное первоначальное значение. нельзя делать false по умолчанию
unsigned char matrixValue[8][16]; //это массив для эффекта Огонь
bool TimerManager::TimerRunning = false;
bool TimerManager::TimerHasFired = false;
uint8_t TimerManager::TimerOption = 1U;
uint64_t TimerManager::TimeToFire = 0ULL;

uint8_t FavoritesManager::FavoritesRunning = 0;
uint16_t FavoritesManager::Interval = DEFAULT_FAVORITES_INTERVAL;
uint16_t FavoritesManager::Dispersion = DEFAULT_FAVORITES_DISPERSION;
uint8_t FavoritesManager::UseSavedFavoritesRunning = 0;
uint8_t FavoritesManager::FavoriteModes[MODE_AMOUNT] = {0};
uint32_t FavoritesManager::nextModeAt = 0UL;
bool FavoritesManager::rndCycle = false;

char TextTicker [80];
int Painting = 0;
CRGB DriwingColor = CRGB(255, 255, 255);

uint8_t espMode;
uint8_t random_on;
uint8_t Favorit_only;
uint32_t my_timer;
uint8_t time_always;
bool connect = false;
uint8_t ESP_CONN_TIMEOUT;

ModeType modes[MODE_AMOUNT];

String configSetup = "{}";

// --- AP (WiFi точка доступа) ---
String AP_NAME = "";                                        // Переменная для имени точки доступа. Задаётся на web странице
String AP_PASS = "";                                        // Переменная для пароля точки доступа. Задаётся на web странице
String LAMP_NAME = "";                                      // Переменная для имени Лампы. Задаётся на web странице


String jsonRead(String& json, String name)
{
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(json);
    return root[name].as<String>();
}

// ------------- Чтение значения json int
int jsonReadtoInt(String& json, String name)
{
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(json);
    return root[name];
}

// ------------- Запись значения json String
void jsonWrite(String& json, String name, String value)
{
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(json);
    root[name] = value;
    json = "";
    root.printTo(json);
}

// ------------- Запись значения json int
void jsonWrite(String& json, String name, int value)
{
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(json);
    root[name] = value;
    json = "";
    root.printTo(json);
}

void jsonWrite(String& json, String name, uint8_t value)
{
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(json);
    root[name] = (int)value;
    json = "";
    root.printTo(json);
}

// ------------- Запись строки в файл
void writeFile(String fileName, String strings)
{
    File configFile = SPIFFS.open("/" + fileName, "w");
    if (!configFile)
    {
        return;
    }
    configFile.print(strings);
    //strings.printTo(configFile);
    configFile.close();
}

void saveConfig()
{
    writeFile("config.json", configSetup);
}

// ------------- Чтение файла в строку
String readFile(String fileName, size_t len)
{
    File configFile = SPIFFS.open("/" + fileName, "r");
    if (!configFile)
    {
        return "Failed";
    }
    size_t size = configFile.size();
    if (size > len)
    {
        configFile.close();
        return "Large";
    }
    String temp = configFile.readString();
    configFile.close();
    return temp;
}
