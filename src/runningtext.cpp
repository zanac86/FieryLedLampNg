#include <Arduino.h>
#include <stdint.h>
#include "globals.h"
#include "utility.h"
#include "fonts.h"

// работа с бегущим текстом


// --- ДЛЯ РАЗРАБОТЧИКОВ ---------------

int16_t offset = WIDTH;
uint32_t scrollTimer = 0LL;


void drawLetter(uint8_t letter, int8_t offset, CRGB letterColor)
{

    uint8_t start_pos = 0, finish_pos = LET_WIDTH;

    if (offset < (int8_t) - LET_WIDTH || offset > (int8_t)WIDTH)
    {
        return;
    }
    if (offset < 0)
    {
        start_pos = (uint8_t) - offset;
    }
    if (offset > (int8_t)(WIDTH - LET_WIDTH))
    {
        finish_pos = (uint8_t)(WIDTH - offset);
    }
    for (uint8_t i = start_pos; i < finish_pos; i++)
    {
        uint8_t thisByte;
        if (MIRR_V)
        {
            thisByte = getFont(letter, (uint8_t)(LET_WIDTH - 1 - i));
        }
        else
        {
            thisByte = getFont(letter, i);
        }

        for (uint8_t j = 0; j < LET_HEIGHT; j++)
        {
            bool thisBit = MIRR_H
                           ? thisByte & (1 << j)
                           : thisByte & (1 << (LET_HEIGHT - 1 - j));

            // рисуем столбец (i - горизонтальная позиция, j - вертикальная)
            if (TEXT_DIRECTION)
            {
                if (thisBit)
                {
                    leds[XY(offset + i, TEXT_HEIGHT + j)] = letterColor;
                }
                else
                {
                    drawPixelXY(offset + i, TEXT_HEIGHT + j, 0x000000);
                }
            }
            else
            {
                if (thisBit)
                {
                    leds[XY(i, offset + TEXT_HEIGHT + j)] = letterColor;
                }
                else
                {
                    drawPixelXY(i, offset + TEXT_HEIGHT + j, 0x000000);
                }
            }
        }
    }
}


// --- СЛУЖЕБНЫЕ ФУНКЦИИ ---------------
uint8_t getFont(uint8_t asciiCode, uint8_t row)             // интерпретатор кода символа в массиве fontHEX (для Arduino IDE 1.8.* и выше)
{
    asciiCode = asciiCode - '0' + 16;                         // перевод код символа из таблицы ASCII в номер согласно нумерации массива

    if (asciiCode <= 90)                                      // печатаемые символы и английские буквы
    {
        return pgm_read_byte(&fontHEX[asciiCode][row]);
    }
    else
        if (asciiCode >= 112 && asciiCode <= 159)
        {
            return pgm_read_byte(&fontHEX[asciiCode - 17][row]);
        }
        else
            if (asciiCode >= 96 && asciiCode <= 111)
            {
                return pgm_read_byte(&fontHEX[asciiCode + 47][row]);
            }

    return 0;
}
