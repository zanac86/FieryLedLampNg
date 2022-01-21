//
//  https://community.alexgyver.ru/goto/post?id=73929
// ==================================================================

// Ссылка для менеджера плат:
// https://arduino.esp8266.com/stable/package_esp8266com_index.json
// При установке выбираем версию 2.7.4

/*
  Скетч к проекту "Многофункциональный RGB светильник"
  Страница проекта (схемы, описания): https://alexgyver.ru/GyverLamp/
  Исходники авторской (старой) прошивки на GitHub: https://github.com/AlexGyver/GyverLamp/
  ==========================================================================================
  Исходники новой версии: https://community.alexgyver.ru/goto/post?id=73929
  ==========================================================================================
  Автор идеи и первой реализации: AlexGyver, AlexGyver Technologies, 2019
  https://AlexGyver.ru/
*/

#include "pgmspace.h"
#include "globals.h"
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include "fonts.h"
#include <TimeLib.h>
#include "TimerManager.h"
#include "eeprommanager.h"
#include "favoritesmanager.h"
#include "http_init.h"
#include "fs.h"


void setup()  //==================================================================  void setup()  =========================================================================
{

    Serial.begin(115200);
    Serial.println();
    ESP.wdtEnable(WDTO_8S);

    //HTTP
    User_setings();
    LOG.print(F("\nСтарт файловой системы\n"));
    FS_init();  //Запускаем файловую систему
    LOG.print(F("Чтение файла конфигурации\n"));
    configSetup = readFile("config.json", 768);
    LOG.println(configSetup);
    //SSDP_init();
    //Настраиваем и запускаем HTTP интерфейс
    LOG.print(F("Старт WebServer\n"));
    HTTP_init();


    //-----------Инициализируем переменные, хранящиеся в файле config.json--------------
    LAMP_NAME = jsonRead(configSetup, "SSDP");
    AP_NAME = jsonRead(configSetup, "ssidAP");
    AP_PASS = jsonRead(configSetup, "passwordAP");
    Favorit_only = jsonReadtoInt(configSetup, "favorit");
    random_on = jsonReadtoInt(configSetup, "random_on");
    espMode = jsonReadtoInt(configSetup, "ESP_mode");
    buttonEnabled = jsonReadtoInt(configSetup, "button_on");
    ESP_CONN_TIMEOUT = jsonReadtoInt(configSetup, "TimeOut");
    time_always = jsonReadtoInt(configSetup, "time_always");
    (jsonRead(configSetup, "run_text")).toCharArray(TextTicker, (jsonRead(configSetup, "run_text")).length() + 1);
    DONT_TURN_ON_AFTER_SHUTDOWN = jsonReadtoInt(configSetup, "effect_always");
    FavoritesManager::rndCycle = jsonReadtoInt(configSetup, "rnd_cycle");  // Перемешать Цикл
    AUTOMATIC_OFF_TIME = (5 * 60UL * 60UL * 1000UL) * (uint32_t)(jsonReadtoInt(configSetup, "timer5h"));

    // КНОПКА
#if defined(ESP_USE_BUTTON)
    touch.setStepTimeout(BUTTON_STEP_TIMEOUT);
    touch.setClickTimeout(BUTTON_CLICK_TIMEOUT);
    touch.setDebounce(BUTTON_SET_DEBOUNCE);
#if (BUTTON_IS_SENSORY == 1)
#if ESP_RESET_ON_START
    delay(500);                                            // ожидание инициализации модуля кнопки ttp223 (по спецификации 250мс)
    if (digitalRead(BTN_PIN))
    {
        // wifiManager.resetSettings();
        LOG.println(F("Настройки WiFiManager сброшены"));
        //buttonEnabled = true;                                   // при сбросе параметров WiFi сразу после старта с зажатой кнопкой, также разблокируется кнопка, если была заблокирована раньше
        jsonWrite(configSetup, "ssid", "");                          // сброс сохранённых SSID и пароля при старте с зажатой кнопкой, если разрешено
        jsonWrite(configSetup, "password", "");
        saveConfig();                                       // Функция сохранения данных во Flash
    }
    ESP.wdtFeed();
#elif defined(BUTTON_LOCK_ON_START)
    delay(500);                                            // ожидание инициализации модуля кнопки ttp223 (по спецификации 250мс)
    if (digitalRead(BTN_PIN))
    {
        buttonEnabled = false;
    }
    ESP.wdtFeed();
#endif
#endif
#if (BUTTON_IS_SENSORY == 0)
#if ESP_RESET_ON_START
    delay(500);                                            // ожидание инициализации модуля кнопки ttp223 (по спецификации 250мс)
    if (!(digitalRead(BTN_PIN)))
    {
        // wifiManager.resetSettings();
        LOG.println(F("Настройки WiFiManager сброшены"));
        //buttonEnabled = true;                                   // при сбросе параметров WiFi сразу после старта с зажатой кнопкой, также разблокируется кнопка, если была заблокирована раньше
        jsonWrite(configSetup, "ssid", "");                          // сброс сохранённых SSID и пароля при старте с зажатой кнопкой, если разрешено
        jsonWrite(configSetup, "password", "");
        saveConfig();                                       // Функция сохранения данных во Flash
    }
    ESP.wdtFeed();
#elif defined(BUTTON_LOCK_ON_START)
    delay(500);                                            // ожидание инициализации модуля кнопки ttp223 (по спецификации 250мс)
    if (!(digitalRead(BTN_PIN)))
    {
        buttonEnabled = false;
    }
    ESP.wdtFeed();
#endif
#endif
#endif


    // ЛЕНТА/МАТРИЦА
    FastLED.addLeds<WS2812B, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS)/*.setCorrection(TypicalLEDStrip)*/;
    //FastLED.addLeds<WS2812B, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(0xB0FFE0); // Калибровка баланса белого цвета. Последовательность байт RGB (B0-R FF-G E0-B)
    FastLED.setBrightness(BRIGHTNESS);
    if (CURRENT_LIMIT > 0)
    {
        FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
    }
    FastLED.clear();
    FastLED.show();

    CRGB cs[] = {CRGB::Red, CRGB::Green, CRGB::Blue, CRGB::Yellow, CRGB::Cyan};
    for (int i = 0; i < 5; i++)
    {
        fill_solid(leds, 1, cs[i]);
        FastLED.show();
        delay(500);
    }

    FastLED.clear();
    FastLED.show();


#ifdef USE_SHUFFLE_FAVORITES // первоначальная очередь избранного до перемешивания
    for (uint8_t i = 0; i < MODE_AMOUNT; i++)
    {
        shuffleFavoriteModes[i] = i;
    }
#endif

    // EEPROM
    EepromManager::InitEepromSettings(                        // инициализация EEPROM; запись начального состояния настроек, если их там ещё нет; инициализация настроек лампы значениями из EEPROM
        modes, &ONflag, &currentMode,
        &(FavoritesManager::ReadFavoritesFromEeprom),
        &(FavoritesManager::SaveFavoritesToEeprom),
        &(restoreSettings)); // не придумал ничего лучше, чем делать восстановление настроек по умолчанию в обработчике инициализации EepromManager

    jsonWrite(configSetup, "Power", ONflag);  // Чтение состояния лампы вкл/выкл,текущий эффект,яркость,скорость,масштаб
    jsonWrite(configSetup, "eff_sel", currentMode);
    jsonWrite(configSetup, "br", modes[currentMode].Brightness);
    jsonWrite(configSetup, "sp", modes[currentMode].Speed);
    jsonWrite(configSetup, "sc", modes[currentMode].Scale);
    jsonWrite(configSetup, "cycle_on", FavoritesManager::FavoritesRunning);  // чтение состояния настроек режима Цикл
    jsonWrite(configSetup, "time_eff", FavoritesManager::Interval);          // вкл/выкл,время переключения,дисперсия,вкл цикла после перезагрузки
    jsonWrite(configSetup, "disp", FavoritesManager::Dispersion);
    jsonWrite(configSetup, "cycle_allwase", FavoritesManager::UseSavedFavoritesRunning);
    jsonWrite(configSetup, "tmr", 0);
    jsonWrite(configSetup, "button_on", buttonEnabled);
    cycle_get();  // чтение выбранных эффектов

    // WI-FI

    LOG.printf_P(PSTR("Рабочий режим лампы: ESP_MODE = %d\n"), espMode);
    //Запускаем WIFI
    LOG.println(F("Старуем WIFI"));

    WiFi.persistent(false);   // Побережём EEPROM

    if (espMode == 0U)                                        // режим WiFi точки доступа
    {
        // Отключаем WIFI
        WiFi.disconnect();
        // Меняем режим на режим точки доступа
        WiFi.mode(WIFI_AP);
        // Задаем настройки сети
        if (sizeof(AP_STATIC_IP))
        {
            WiFi.softAPConfig(
                IPAddress(AP_STATIC_IP[0], AP_STATIC_IP[1], AP_STATIC_IP[2], AP_STATIC_IP[3]),      // IP адрес WiFi точки доступа
                IPAddress(AP_STATIC_IP[0], AP_STATIC_IP[1], AP_STATIC_IP[2], 1),                    // первый доступный IP адрес сети
                IPAddress(255, 255, 255, 0));                                                       // маска подсети
        }
        // Включаем WIFI в режиме точки доступа с именем и паролем
        // хронящихся в переменных _ssidAP _passwordAP в фвйле config.json
        WiFi.softAP(AP_NAME, AP_PASS);
        LOG.print(F("Старт WiFi в режиме точки доступа\n"));
        LOG.print(F("IP адрес: "));
        LOG.println(WiFi.softAPIP());
        connect = true;
        delay(100);
    }
    else                                                      // режим WiFi клиента. Подключаемся к роутеру
    {
        LOG.println(F("Старт WiFi в режиме клиента (подключение к роутеру)"));
        //  WIFI_start_station_mode ();


        WiFi.persistent(false);

        // Попытка подключения к Роутеру
        WiFi.mode(WIFI_STA);
        String _ssid = jsonRead(configSetup, "ssid");
        String _password = jsonRead(configSetup, "password");
        if (_ssid == "" && _password == "")
        {
            espMode = 0;
            jsonWrite(configSetup, "ESP_mode", (int)espMode);
            saveConfig();
            ESP.restart();
        }
        else
        {
            WiFi.begin(_ssid.c_str(), _password.c_str());
        }

        delay(100);
    }     //if (espMode == 0U) {...} else {...

    ESP.wdtFeed();

    // LOG.printf_P(PSTR("Порт UDP сервера: %u\n"), localPort);
    // Udp.begin(localPort);

    // ОСТАЛЬНОЕ
    memset(matrixValue, 0, sizeof(matrixValue)); //это массив для эффекта Огонь. странно, что его нужно залить нулями
    randomSeed(micros());
    changePower();
    loadingFlag = true;

    //TextTicker = RUNNING_TEXT_DEFAULT;
    delay(100);

    my_timer = millis();
}


void loop()  //====================================================================  void loop()  ===========================================================================
{

    if (espMode)
    {
        if (WiFi.status() != WL_CONNECTED)
        {
            if ((millis() - my_timer) >= 1000UL)
            {
                my_timer = millis();
                if (ESP_CONN_TIMEOUT--)
                {
                    LOG.print(".");
                    ESP.wdtFeed();
                }
                else
                {
                    // Если не удалось подключиться запускаем в режиме AP
                    espMode = 0;
                    jsonWrite(configSetup, "ESP_mode", (int)espMode);
                    saveConfig();
                    ESP.restart();
                    //StartAPMode();
                }
            }
        }
        else
        {
            // Иначе удалось подключиться отправляем сообщение
            // о подключении и выводим адрес IP
            LOG.print(F("\nПодключение к роутеру установлено\n"));
            LOG.print(F("IP адрес: "));
            LOG.println(WiFi.localIP());
            long rssi = WiFi.RSSI();
            LOG.print(F("Уровень сигнала сети RSSI = "));
            LOG.print(rssi);
            LOG.println(F(" dbm"));
            connect = true;
            //ESP_CONN_TIMEOUT = 0;
#ifdef DISPLAY_IP_AT_START
            loadingFlag = true;
            while (!fillString(WiFi.localIP().toString().c_str(), CRGB::White, false))
            {
                delay(1);
                ESP.wdtFeed();
            }
            loadingFlag = true;
#endif  // DISPLAY_IP_AT_START
            delay(100);
        }
    }

    if (connect || !espMode)
    {
        my_timer = millis();
    }

    do      //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++========= Главный цикл ==========+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    {
        // Если не устойчивое подключение к WiFi, или не создаётся точка доступа, или лампа не хочет подключаться к вашей сети или вы не можете подключиться к точке доступа, то может быть у вас не качественная плата.
        delay(20);    //Для некоторых плат ( особенно без металлического экрана над ESP и Flash памятью ) эта задержка должна быть увеличена. Подбирается индивидуально в пределах 1-12 мс до устойчивой работы WiFi. Чем меньше, тем лучше. Качественные платы работают с задержкой 0.
        yield();

        if ((connect || !espMode) && ((millis() - my_timer) >= 10UL))
        {
            HTTP.handleClient(); // Обработка запросов web страницы.
            my_timer = millis();
        }

        //HTTP.handleClient(); // Обработка запросов web страницы.
        //    parseUDP();
        yield();
        if (Painting == 0)
        {

            effectsTick();

            EepromManager::HandleEepromTick(&settChanged, &eepromTimeout, &ONflag,
                                            &currentMode, modes, &(FavoritesManager::SaveFavoritesToEeprom));
            yield();

#ifdef ESP_USE_BUTTON
            //if (buttonEnabled) в процедуре ведь есть эта проверка
            buttonTick();
#endif

            TimerManager::HandleTimer(&ONflag, &settChanged,          // обработка событий таймера отключения лампы
                                      &eepromTimeout, &changePower);

            if (FavoritesManager::HandleFavorites(                    // обработка режима избранных эффектов
                        &ONflag,
                        &currentMode,
                        &loadingFlag
                        , &random_on
                        , &selectedSettings
                    ))
            {
                FastLED.setBrightness(modes[currentMode].Brightness);
            }

        }//if (Painting == 0)
        yield();
        ESP.wdtFeed();
        //delay (7);
    }
    while (connect);

}
