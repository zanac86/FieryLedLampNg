#ifndef TIMER_MANAGER_H
#define TIMER_MANAGER_H

#include <Arduino.h>
#include "stdint.h"
#include "globals.h"

class TimerManager
{
    public:
        static bool TimerRunning;   // флаг "таймер взведён"
        static bool TimerHasFired;  // флаг "таймер отработал"
        static uint8_t TimerOption; // индекс элемента в списке List Picker'а
        static uint64_t TimeToFire; // время, в которое должен сработать таймер (millis)

        static void HandleTimer( // функция, обрабатывающая срабатывание таймера, гасит матрицу
            bool* ONflag,
            bool* settChanged,
            uint32_t* eepromTimeout,
            void (*changePower)())
        {
            if (!TimerManager::TimerHasFired &&
                    TimerManager::TimerRunning &&
                    millis() >= TimerManager::TimeToFire)
            {
                TimerManager::TimerRunning = false;
                TimerManager::TimerHasFired = true;
                FastLED.clear();
                delay(2);
                FastLED.show();
                *ONflag = !(*ONflag);
                changePower();
                jsonWrite(configSetup, "Power", (uint8_t)*ONflag);
                *settChanged = true;
                *eepromTimeout = millis();
            }
        }
};

#endif