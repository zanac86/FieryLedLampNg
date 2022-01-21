void updateSets()
{
    loadingFlag = true;
    settChanged = true;
    eepromTimeout = millis();
}

void User_setings()
{

    HTTP.on("/favorit", handle_favorit);    // включить \ выключить переход кнопкой только по эффектам из выбранных в режиме Цикл и
    HTTP.on("/random_on", handle_random);  // случайных настроек эффектов в режиме цикл без сохранения в EEPROM
    HTTP.on("/button_on", handle_button_on);  // Вкл\Выкл кнопки лампы (дублирует в приложении, но на виду)
    //HTTP.on("/User_set", handle_User_set); // Сохранение random_on, time_always, button_on и favorit в EEPROM (Файл)
    HTTP.on("/ESP_mode", handle_ESP_mode); // Установка ESP Mode
    HTTP.on("/eff_reset", handle_eff_reset);  //сброс настроек эффектов по умолчанию
    HTTP.on("/run_text", handle_run_text);  // Текст для бегущей строки
    HTTP.on("/effect_always", handle_effect_always);  // Не возобновлять работу эффектов
    HTTP.on("/timer5h", handle_timer5h);  // Автовыключение через 5 часов
    HTTP.on("/eff_sel", handle_eff_sel);  // Выбор эффекта из списка
    HTTP.on("/eff", handle_eff);  // Следующий / Предыдущий
    HTTP.on("/br", handle_br);  // Яркость
    HTTP.on("/sp", handle_sp);  // Скорость
    HTTP.on("/sc", handle_sc);  // Масштаб / Цвет
    HTTP.on("/brm", handle_brm);  // Пошаговая яркость  минус
    HTTP.on("/brp", handle_brp);  // Пошаговая яркость плюс
    HTTP.on("/spm", handle_spm);  // Пошаговая  скорость минус
    HTTP.on("/spp", handle_spp);  // Пошаговая скорость  плюс
    HTTP.on("/scm", handle_scm);  // Пошаговый масштаб  минус
    HTTP.on("/scp", handle_scp);  // Пошаговый мвсштаб  плюс
    HTTP.on("/PassOn", handle_PassOn); // Использовать (1) или нет (0) пароль для доступа к странице Начальных настроек
    HTTP.on("/Power", handle_Power);          // устройство вкл/выкл
    HTTP.on("/time_always", handle_time_always);     // Выводить или нет время бегущей строкой(если задано) на не активной лампе
    HTTP.on("/cycle_on", handle_cycle_on);   // Вкл/выкл режима Цикл
    HTTP.on("/time_eff", handle_time_eff);   // Время переключения цикла + Dispersion добавочное случайное время от 0 до disp
    HTTP.on("/rnd_cycle", handle_rnd_cycle);   // Перемешать выбранные или по порядку
    HTTP.on("/cycle_allwase", handle_cycle_allwase);   // Запускать режим цыкл после выкл/вкл лампы или нет
    HTTP.on("/cycle_set", handle_cycle_set);   // Выбор эффектов
    HTTP.on("/eff_all", handle_eff_all);   // Выбрать все
    HTTP.on("/eff_clr", handle_eff_clr);   // сбросить Выбор
    HTTP.on("/timer", handle_timer);   // Запуск таймера выключения
    HTTP.on("/def", handle_def);   //  Установка настроек эффекта по умолчанию
    HTTP.on("/rnd", handle_rnd);   // Установка случайных настроек эффектов
    HTTP.on("/all_br", handle_all_br);  // Общая яркость
    HTTP.on("/eff_save", handle_eff_save);  // Сохранить настройки эффектов в файл
    HTTP.on("/eff_read", handle_eff_read);  // Загрузить настройки эффектов из файла
    HTTP.on("/alt", handle_alt_panel);   // Альтернативная главная web страница управления эффектами

    // --------------------Получаем SSID со страницы
    HTTP.on("/ssid", HTTP_GET, []()
    {
        jsonWrite(configSetup, "ssid", HTTP.arg("ssid"));
        jsonWrite(configSetup, "password", HTTP.arg("password"));
        jsonWrite(configSetup, "TimeOut", HTTP.arg("TimeOut").toInt());
        ESP_CONN_TIMEOUT = jsonReadtoInt(configSetup, "TimeOut");
        saveConfig();                 // Функция сохранения данных во Flash
        HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
    });
    // --------------------Получаем SSID AP со страницы
    HTTP.on("/ssidap", HTTP_GET, []()
    {
        jsonWrite(configSetup, "ssidAP", HTTP.arg("ssidAP"));
        jsonWrite(configSetup, "passwordAP", HTTP.arg("passwordAP"));
        saveConfig();                 // Функция сохранения данных во Flash
        HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
    });

}

void handle_favorit()
{
    jsonWrite(configSetup, "favorit", HTTP.arg("favorit").toInt());
    saveConfig();
    Favorit_only = jsonReadtoInt(configSetup, "favorit");
    HTTP.send(200, "text/plain", "OK");
}

void handle_random()
{
    jsonWrite(configSetup, "random_on", HTTP.arg("random_on").toInt());
    saveConfig();
    random_on = jsonReadtoInt(configSetup, "random_on");
    HTTP.send(200, "text/plain", "OK");
}

void handle_button_on()
{
    jsonWrite(configSetup, "button_on", HTTP.arg("button_on").toInt());
    saveConfig();
    buttonEnabled = jsonReadtoInt(configSetup, "button_on");
    HTTP.send(200, "text/plain", "OK");
}

/*
  void handle_User_set () {
    handle_random();
    handle_favorit();
    handle_time_always();
    handle_button_on();
    saveConfig();
    HTTP.send(200, "text/plain", "OK");
  }
*/

void handle_ESP_mode()
{
    jsonWrite(configSetup, "ESP_mode", HTTP.arg("ESP_mode").toInt());
    saveConfig();
    espMode = jsonReadtoInt(configSetup, "ESP_mode");
    HTTP.send(200, "text/plain", "OK");
}

void handle_eff_reset()
{
    restoreSettings();
    updateSets();
    //loadingFlag = true;  // Перезапуск Эффекта
    jsonWrite(configSetup, "br", modes[currentMode].Brightness);
    jsonWrite(configSetup, "sp", modes[currentMode].Speed);
    jsonWrite(configSetup, "sc", modes[currentMode].Scale);
    HTTP.send(200, "text/plain", "OK");
}

void handle_run_text()
{
    jsonWrite(configSetup, "run_text", HTTP.arg("run_text"));
    saveConfig();                 // Функция сохранения данных во Flash
    (jsonRead(configSetup, "run_text")).toCharArray(TextTicker, (jsonRead(configSetup, "run_text")).length() + 1);
    HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
}

void handle_effect_always()
{
    jsonWrite(configSetup, "effect_always", HTTP.arg("effect_always").toInt());
    saveConfig();
    DONT_TURN_ON_AFTER_SHUTDOWN = jsonReadtoInt(configSetup, "effect_always");
    HTTP.send(200, "text/plain", "OK");
}

void handle_timer5h()
{
    jsonWrite(configSetup, "timer5h", HTTP.arg("timer5h").toInt());
    saveConfig();
    AUTOMATIC_OFF_TIME = (5 * 60UL * 60UL * 1000UL) * (uint32_t)(jsonReadtoInt(configSetup, "timer5h"));
    HTTP.send(200, "text/plain", "OK");
}

void handle_eff_sel()
{
    jsonWrite(configSetup, "eff_sel", HTTP.arg("eff_sel").toInt());
    currentMode = jsonReadtoInt(configSetup, "eff_sel");
    jsonWrite(configSetup, "br", modes[currentMode].Brightness);
    jsonWrite(configSetup, "sp", modes[currentMode].Speed);
    jsonWrite(configSetup, "sc", modes[currentMode].Scale);
    FastLED.setBrightness(modes[currentMode].Brightness);
    loadingFlag = true;
    settChanged = true;
    eepromTimeout = millis();
    if (random_on && FavoritesManager::FavoritesRunning)
    {
        selectedSettings = 1U;
    }
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_eff()
{
    jsonWrite(configSetup, "eff", HTTP.arg("eff").toInt());
    if (HTTP.arg("eff").toInt())
    {
        if (++currentMode >= MODE_AMOUNT)
        {
            currentMode = 0;
        }
        jsonWrite(configSetup, "eff_sel", currentMode);
        jsonWrite(configSetup, "br", modes[currentMode].Brightness);
        jsonWrite(configSetup, "sp", modes[currentMode].Speed);
        jsonWrite(configSetup, "sc", modes[currentMode].Scale);
        FastLED.setBrightness(modes[currentMode].Brightness);
        loadingFlag = true;
        settChanged = true;
        eepromTimeout = millis();
        if (random_on && FavoritesManager::FavoritesRunning)
        {
            selectedSettings = 1U;
        }
    }
    else
    {
        if (--currentMode >= MODE_AMOUNT)
        {
            currentMode = MODE_AMOUNT - 1;
        }

        jsonWrite(configSetup, "eff_sel", currentMode);
        jsonWrite(configSetup, "br", modes[currentMode].Brightness);
        jsonWrite(configSetup, "sp", modes[currentMode].Speed);
        jsonWrite(configSetup, "sc", modes[currentMode].Scale);
        FastLED.setBrightness(modes[currentMode].Brightness);
        loadingFlag = true;
        settChanged = true;
        eepromTimeout = millis();
        if (random_on && FavoritesManager::FavoritesRunning)
        {
            selectedSettings = 1U;
        }
    }
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_br()
{
    jsonWrite(configSetup, "br", HTTP.arg("br").toInt());
    modes[currentMode].Brightness = jsonReadtoInt(configSetup, "br");
    FastLED.setBrightness(modes[currentMode].Brightness);
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_sp()
{
    jsonWrite(configSetup, "sp", HTTP.arg("sp").toInt());
    modes[currentMode].Speed = jsonReadtoInt(configSetup, "sp");
    loadingFlag = true;  // Перезапуск Эффекта
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_sc()
{
    jsonWrite(configSetup, "sc", HTTP.arg("sc").toInt());
    modes[currentMode].Scale = jsonReadtoInt(configSetup, "sc");
    loadingFlag = true;  // Перезапуск Эффекта
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_brm()
{
    modes[currentMode].Brightness = constrain(modes[currentMode].Brightness - 1, 1, 255);
    jsonWrite(configSetup, "br", modes[currentMode].Brightness);
    FastLED.setBrightness(modes[currentMode].Brightness);
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_brp()
{
    modes[currentMode].Brightness = constrain(modes[currentMode].Brightness + 1, 1, 255);
    jsonWrite(configSetup, "br", modes[currentMode].Brightness);
    FastLED.setBrightness(modes[currentMode].Brightness);
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_spm()
{
    modes[currentMode].Speed = constrain(modes[currentMode].Speed - 1, 1, 255);
    jsonWrite(configSetup, "sp", modes[currentMode].Speed);
    loadingFlag = true;  // Перезапуск Эффекта
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_spp()
{
    modes[currentMode].Speed = constrain(modes[currentMode].Speed + 1, 1, 255);
    jsonWrite(configSetup, "sp", modes[currentMode].Speed);
    loadingFlag = true;  // Перезапуск Эффекта
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_scm()
{
    modes[currentMode].Scale = constrain(modes[currentMode].Scale - 1, 1, 100);
    jsonWrite(configSetup, "sc", modes[currentMode].Scale);
    loadingFlag = true;  // Перезапуск Эффекта
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_scp()
{
    modes[currentMode].Scale = constrain(modes[currentMode].Scale + 1, 1, 100);
    jsonWrite(configSetup, "sc", modes[currentMode].Scale);
    loadingFlag = true;  // Перезапуск Эффекта
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_PassOn()
{
    bool flg = false;
    jsonWrite(configSetup, "PassOn", HTTP.arg("PassOn").toInt());
    if (jsonReadtoInt(configSetup, "PassOn"))
    {
        flg = FileCopy("/stp/stp_l.json.gz", "/setup.json.gz");
    }
    else
    {
        flg = FileCopy("/stp/stp_nl.json.gz", "/setup.json.gz");
    }
    if (flg)
    {
        HTTP.send(200, "text/plain", "OK");
        saveConfig();
    }
    else
    {
        HTTP.send(404, "text/plain", "File not found");
    }
}

void handle_Power()
{
    uint8_t tmp;
    tmp = HTTP.arg("Power").toInt();
    if (tmp == 2)
    {
        jsonReadtoInt(configSetup, "Power") == 0 ? tmp = 1 : tmp = 0;
    }
    jsonWrite(configSetup, "Power", tmp);
    ONflag = tmp;
    changePower();
    if (ONflag)
    {
        eepromTimeout = millis();
    }
    else
    {
        eepromTimeout = millis() + EEPROM_WRITE_DELAY;
    }
    settChanged = true;
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_time_always()
{
    jsonWrite(configSetup, "time_always", HTTP.arg("time_always").toInt());
    saveConfig();
    time_always = jsonReadtoInt(configSetup, "time_always");
    HTTP.send(200, "text/plain", "OK");
}

void handle_cycle_on()     // Вкл/выкл режима Цикл
{
    uint8_t tmp;
    tmp = HTTP.arg("cycle_on").toInt();
    jsonReadtoInt(configSetup, "cycle_on") == 0 ? tmp = 1 : tmp = 0;
    jsonWrite(configSetup, "cycle_on", tmp);
    if (ONflag)
    {
        FavoritesManager::FavoritesRunning = tmp;
    }
    else
    {
        FavoritesManager::FavoritesRunning = 0;
        jsonWrite(configSetup, "cycle_on", 0);
    }

    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");  //HTTP.send(200, "text/plain", "OK");
}

void handle_time_eff()      // Время переключения цикла + Dispersion добавочное случайное время от 0 до disp
{
    jsonWrite(configSetup, "time_eff", HTTP.arg("time_eff").toInt());
    FavoritesManager::Interval = jsonReadtoInt(configSetup, "time_eff");
    jsonWrite(configSetup, "disp", HTTP.arg("disp").toInt());
    FavoritesManager::Dispersion = jsonReadtoInt(configSetup, "disp");
    HTTP.send(200, "text/plain", "OK");
}

void handle_rnd_cycle()      // Перемешать выбранные или по порядку
{
    jsonWrite(configSetup, "rnd_cycle", HTTP.arg("rnd_cycle").toInt());
    FavoritesManager::rndCycle = jsonReadtoInt(configSetup, "rnd_cycle");
    saveConfig();
    HTTP.send(200, "text/plain", "OK");
}

void handle_cycle_allwase()      // Запускать режим цыкл после выкл/вкл лампы или нет
{
    jsonWrite(configSetup, "cycle_allwase", HTTP.arg("cycle_allwase").toInt());
    FavoritesManager::UseSavedFavoritesRunning = jsonReadtoInt(configSetup, "cycle_allwase");
    if (!ONflag && !FavoritesManager::UseSavedFavoritesRunning)
    {
        FavoritesManager::FavoritesRunning = 0;
        jsonWrite(configSetup, "cycle_on", 0);
    }
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");  //HTTP.send(200, "text/plain", "OK");
}

void handle_eff_all()       //Выбрать все эффекты
{
    char i[3];
    String configCycle = readFile("cycle_config.json", 1024);
    // подготовка  строк с именами полей json
    for (uint8_t k = 0; k < MODE_AMOUNT; k++)
    {
        itoa((k), i, 10);
        String e = "e" + String(i) ;
        //сохранение параметров в строку
        jsonWrite(configCycle, e, 1U);
    }
    writeFile("cycle_config.json", configCycle);
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_eff_clr()       //очостить все эффекты
{
    char i[3];
    String configCycle = readFile("cycle_config.json", 1024);
    // подготовка  строк с именами полей json
    for (uint8_t k = 0; k < MODE_AMOUNT; k++)
    {
        itoa((k), i, 10);
        String e = "e" + String(i) ;
        //сохранение параметров в строку
        jsonWrite(configCycle, e, 0U);
    }
    writeFile("cycle_config.json", configCycle);
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_cycle_set()      // Выбор эффектов для Цикла
{
    char i[3];
    String configCycle = readFile("cycle_config.json", 1024);
    // подготовка  строк с именами полей json file
    for (uint8_t k = 0; k < MODE_AMOUNT; k++)
    {
        itoa((k), i, 10);
        String e = "e" + String(i) ;
        //сохранение параметров в строку
        jsonWrite(configCycle, e, HTTP.arg(e).toInt());
        //сохранение выбранных эффектов для Цикла
        FavoritesManager::FavoriteModes[k] = jsonReadtoInt(configCycle, e);
    }
    FavoritesManager::SaveFavoritesToEeprom();
    writeFile("cycle_config.json", configCycle);
    //settChanged = true;
    //eepromTimeout = millis();

    HTTP.send(200, "text/plain", "OK");
}

void cycle_get()     // Начальная инициализация выбранных эффектов из файла при включении питания
{
    char i[3];
    bool cycle_change = false;
    String configCycle = readFile("cycle_config.json", 1024);
    // подготовка  строк с именами полей json file
    for (uint8_t k = 0; k < MODE_AMOUNT; k++)
    {
        itoa((k), i, 10);
        String e = "e" + String(i) ;
        //передача параметров из массива в строку json если значение в памяти не равно значению в файле
        if (FavoritesManager::FavoriteModes[k] != jsonReadtoInt(configCycle, e))
        {
            jsonWrite(configCycle, e, FavoritesManager::FavoriteModes[k]);
            cycle_change = true;
        }

    }
    if (cycle_change)
    {
        writeFile("cycle_config.json", configCycle);
    }
}

void handle_timer()       // Установка таймера выключения
{
    uint8_t tmp;
    tmp = HTTP.arg("timer").toInt();
    if (tmp != jsonReadtoInt(configSetup, "timer"))
    {
        jsonWrite(configSetup, "timer", tmp);
        saveConfig();
    }
    jsonWrite(configSetup, "tmr", 1);
    TimerManager::TimeToFire = millis() + tmp * 60UL * 1000UL;
    TimerManager::TimerRunning = true;
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}"); //HTTP.send(200, "application/json", "{\"title\":\"Запущен\",\"class\":\"btn btn-block btn-warning\"}");
}

void handle_def()      // Сброс настроек текущего эффекта по умолчанию
{
    setModeSettings();
    updateSets();
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_rnd()      // Установка случайных настроек текущему эффекту
{
    selectedSettings = 1U;
    updateSets();
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_all_br()       //Общая яркость
{
    jsonWrite(configSetup, "all_br", HTTP.arg("all_br").toInt());
    uint8_t ALLbri = jsonReadtoInt(configSetup, "all_br");
    for (uint8_t i = 0; i < MODE_AMOUNT; i++)
    {
        modes[i].Brightness = ALLbri;
    }
    jsonWrite(configSetup, "br", ALLbri);
    FastLED.setBrightness(ALLbri);
    loadingFlag = true;
    //LOG.println (ALLbri);
    HTTP.send(200, "application/json", "{\"should_refresh\": \"true\"}");
}

void handle_eff_save()
{
    SPIFFS.begin();
    File file = SPIFFS.open("/effect.ini", "w");
    if (file)
    {
        for (uint8_t i = 0; i < MODE_AMOUNT; i++)
        {
            file.write(modes[i].Brightness);
            file.write(modes[i].Speed);
            file.write(modes[i].Scale);
        }
    }
    else
    {
    }
    file.close();
    HTTP.send(200, "text/plain", "OK");
}

void handle_eff_read()
{
    SPIFFS.begin();
    File file = SPIFFS.open("/effect.ini", "r");
    if (file)
    {
        for (uint8_t i = 0; i < MODE_AMOUNT; i++)
        {
            modes[i].Brightness = file.read();
            modes[i].Speed = file.read();
            modes[i].Scale = file.read();
        }
        loadingFlag = true;  // Перезапуск Эффекта
        jsonWrite(configSetup, "br", modes[currentMode].Brightness);
        jsonWrite(configSetup, "sp", modes[currentMode].Speed);
        jsonWrite(configSetup, "sc", modes[currentMode].Scale);
    }
    else
    {
    }
    file.close();
    HTTP.send(200, "text/plain", "OK");
}

bool FileCopy(String SourceFile, String TargetFile)
{
    File S_File = SPIFFS.open(SourceFile, "r");
    File T_File = SPIFFS.open(TargetFile, "w");
    if (!S_File || !T_File)
    {
        return false;
    }
    size_t size = S_File.size();
    for (unsigned int i = 0; i < size; i++)
    {
        T_File.write(S_File.read());
    }
    S_File.close();
    T_File.close();
    return true;
}
