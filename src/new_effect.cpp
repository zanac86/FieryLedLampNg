#include <Arduino.h>
#include <stdint.h>
#include "globals.h"

//---------------------------------------
// New Effects
//---------------------------------------

uint8_t custom_eff = 0;
uint16_t eff_timout;

uint32_t colorDimm(uint32_t colorValue, long lenght, long pixel)
{

    uint8_t red = (colorValue & 0x00FF0000) >> 16;
    uint8_t green = (colorValue & 0x0000FF00) >> 8;
    uint8_t blue = (colorValue & 0x000000FF);

    double prozent = 100 / lenght;

    red = red - red * ((prozent * pixel) / 100);
    green = green - green * ((prozent * pixel) / 100);
    blue = blue - blue * ((prozent * pixel) / 100);

    // colorValue = strip.Color(red,green,blue);
    colorValue = red;
    colorValue = (colorValue << 8) + green;
    colorValue = (colorValue << 8) + blue;
    return colorValue;
}

// =============== Wine ================
//    © SlingMaster | by Alex Dovby
//               EFF_WINE
//--------------------------------------

void colorsWine()
{
    uint8_t divider;
    if (loadingFlag)
    {
#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
        if (selectedSettings)
        {
            // scale | speed
            setModeSettings(20U + random8(200U), 200U);
        }
#endif
        loadingFlag = false;
        fillAll(CHSV(55U, 255U, 65U));
        deltaValue = 255U - modes[currentMode].Speed + 1U;
        // minspeed 230 maxspeed 250 ============
        // minscale  40 maxscale  75 ============
        // красное вино hue > 0 & <=10
        // розовое вино hue > 10 & <=20
        // белое вино   hue > 20U & <= 40
        // шампанское   hue > 40U & <= 60

        deltaHue2 = 0U;                         // count для замедления смены цвета
        step = deltaValue;                      // чтообы при старте эффекта сразу покрасить лампу
        deltaHue = 1U;                          // direction | 0 hue-- | 1 hue++ |
        hue = 55U;                              // Start Color
        hue2 = 65U;                             // Brightness
        pcnt = 0;
    }

    deltaHue2++;
    // маштаб задает скорость изменения цвета 5 уровней
    divider = 5 - floor((modes[currentMode].Scale - 1) / 20);

    // возвращаем яркость для перехода к белому
    if (hue >= 10 && hue2 < 100U)
    {
        hue2++;
    }
    // уменьшаем яркость для красного вина
    if (hue < 10 && hue2 > 40U)
    {
        hue2--;
    }

    // изменение цвета вина -----
    if (deltaHue == 1U)
    {
        if (deltaHue2 % divider == 0)
        {
            hue++;
        }
    }
    else
    {
        if (deltaHue2 % divider == 0)
        {
            hue--;
        }
    }
    // --------

    // LOG.printf_P(PSTR("Wine | hue = %03d | Dir = %d | Brightness %03d | deltaHue2 %03d | divider %d | %d\n"), hue, deltaHue, hue2, deltaHue2, divider, step);

    // сдвигаем всё вверх -----------
    for (uint8_t x = 0U; x < WIDTH; x++)
    {
        for (uint8_t y = HEIGHT; y > 0U; y--)
        {
            drawPixelXY(x, y, getPixColorXY(x, y - 1U));
        }
    }

    if (hue > 40U)
    {
        // добавляем перляж для шампанского
        pcnt = random(0, WIDTH);
    }
    else
    {
        pcnt = 0;
    }

    // заполняем нижнюю строку с учетом перляжа
    for (uint8_t x = 0U; x < WIDTH; x++)
    {
        if ((x == pcnt) && (pcnt > 0))
        {
            // с перляжем ------
            drawPixelXY(x, 0U, CHSV(hue, 150U, hue2 + 20U + random(0, 50U)));
        }
        else
        {
            drawPixelXY(x, 0U, CHSV(hue, 255U, hue2));
        }
    }

    // меняем направление изменения цвета вина от красного к шампанскому и обратно
    // в диапазоне шкалы HUE |0-60|
    if (hue == 0U)
    {
        deltaHue = 1U;
    }
    if (hue == 60U)
    {
        deltaHue = 0U;
    }
    step++;
}

// ============== Swirl ================
//    © SlingMaster | by Alex Dovby
//              EFF_SWIRL
//--------------------------------------
void Swirl()
{
    uint8_t divider;
    uint8_t lastHue;
    static const uint32_t colors[5][6] PROGMEM =
    {
        {CRGB::Blue, CRGB::DarkRed, CRGB::Aqua, CRGB::Magenta, CRGB::Gold, CRGB::Green },
        {CRGB::Yellow, CRGB::LemonChiffon, CRGB::LightYellow, CRGB::Gold, CRGB::Chocolate, CRGB::Goldenrod},
        {CRGB::Green, CRGB::DarkGreen, CRGB::LawnGreen, CRGB::SpringGreen, CRGB::Cyan, CRGB::Black },
        {CRGB::Blue, CRGB::DarkBlue, CRGB::MidnightBlue, CRGB::MediumSeaGreen, CRGB::MediumBlue, CRGB:: DeepSkyBlue },
        {CRGB::Magenta, CRGB::Red, CRGB::DarkMagenta, CRGB::IndianRed, CRGB::Gold, CRGB::MediumVioletRed }
    };
    uint32_t color;

    if (loadingFlag)
    {

#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
        if (selectedSettings)
        {
            // scale | speed
            setModeSettings(50U + random8(190U), 250U);
        }
#endif
        loadingFlag = false;
        FastLED.clear();
        deltaValue = 255U - modes[currentMode].Speed + 1U;
        step = deltaValue;                      // чтообы при старте эффекта сразу покрасить лампу
        deltaHue2 = 0U;                         // count для замедления смены цвета
        deltaHue = 0U;                          // direction | 0 hue-- | 1 hue++ |
        hue2 = 0U;                              // x
    }


    if (step >= deltaValue)
    {
        step = 0U;
    }
    divider = floor((modes[currentMode].Scale - 1) / 20); // маштаб задает смену палитры
    //  if (deltaValue > 50U && deltaHue2 == 0U) {
    //    hue = random8(6);                       // если низкая скорость меняем цвет после каждого витка
    //  }
    // задаем цвет и рисуем завиток --------
    color = colors[divider][hue];
    // drawPixelXY((hue2 + 1), (deltaHue2 - 1), 0x000000); // aded shadow
    drawPixelXY(hue2, deltaHue2, color);
    // LOG.printf_P(PSTR("Swirl | hue = %03d | x= %03d | y= %03d | divider %d | %d\n"), hue, hue2, deltaHue2, divider, step);
    // -------------------------------------

    hue2++;                     // x
    // два варианта custom_eff задается в сетапе лампы ----
    if (custom_eff == 1)
    {
        // blurScreen(beatsin8(5U, 20U, 5U));
        deltaHue2++;              // y
    }
    else
    {
        // blurScreen(10U);
        if (hue2 % 2 == 0)
        {
            deltaHue2++;            // y
        }
    }
    // -------------------------------------

    if (hue2 > WIDTH)
    {
        hue2 = 0U;
    }

    if (deltaHue2 >= HEIGHT)
    {
        deltaHue2 = 0U;
        // new swirl ------------
        hue2 = random8(WIDTH - 2);
        // hue2 = hue2 + 2;
        // select new color -----
        hue = random8(6);

        if (lastHue == hue)
        {
            hue = hue + 1;
            if (hue >= 6)
            {
                hue = 0;
            }
        }
        lastHue = hue;
    }
    // blurScreen(beatsin8(5U, 20U, 5U));
    blurScreen(4U + random8(8));
    step++;
}

// -------------------------------------------
// for effect Ukraine
// -------------------------------------------
void drawCrest()
{
    static const uint32_t data[9][5] PROGMEM =
    {
        {0x000000, 0x000000, 0xFFD700, 0x000000, 0x000000 },
        {0xFFD700, 0x000000, 0xFFD700, 0x000000, 0xFFD700 },
        {0xFFD700, 0x000000, 0xFFD700, 0x000000, 0xFFD700 },
        {0xFFD700, 0x000000, 0xFFD700, 0x000000, 0xFFD700 },
        {0xFFD700, 0x000000, 0xFFD700, 0x000000, 0xFFD700 },
        {0xFFD700, 0xFFD700, 0xFFD700, 0xFFD700, 0xFFD700 },
        {0xFFD700, 0x000000, 0xFFD700, 0x000000, 0xFFD700 },
        {0xFFD700, 0xFFD700, 0xFFD700, 0xFFD700, 0xFFD700 },
        {0x000000, 0x000000, 0xFFD700, 0x000000, 0x000000 }
    };

    uint8_t posX = floor(WIDTH * 0.5) - 3;
    uint8_t posY = 9;
    uint32_t color;
    if (HEIGHT > 16)
    {
        posY = floor(HEIGHT * 0.5) - 1;
    }
    FastLED.clear();
    for (uint8_t y = 0U; y < 9; y++)
    {
        for (uint8_t x = 0U; x < 5; x++)
        {
            color = data[y][x];
            drawPixelXY(posX + x, posY - y, color);
        }
    }
}

// ============== Ukraine ==============
//      © SlingMaster | by Alex Dovby
//              EFF_UKRAINE
//--------------------------------------
void Ukraine()
{
    uint8_t divider;
    uint32_t color;
    static const uint16_t MAX_TIME = 500;
    uint16_t tMAX = 100;
    static const uint8_t timeout = 100;
    static const uint32_t colors[2][5] =
    {
        {CRGB::Blue, CRGB::MediumBlue, 0x0F004F, 0x02002F, 0x1F2FFF },
        {CRGB::Yellow, CRGB::Gold, 0x4E4000, 0xFF6F00, 0xFFFF2F }
    };

    // Initialization =========================
    if (loadingFlag)
    {
#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
        if (selectedSettings)
        {
            //                     scale | speed
            setModeSettings(random8(250U), 200U + random8(50U));
        }
#endif
        loadingFlag = false;
        drawCrest();
        // minspeed 200 maxspeed 250 ============
        // minscale   0 maxscale 100 ============
        deltaValue = 255U - modes[currentMode].Speed + 1U;
        step = deltaValue;                        // чтообы при старте эффекта сразу покрасить лампу
        deltaHue2 = 0U;                           // count для замедления смены цвета
        deltaHue = 0U;                            // direction | 0 hue-- | 1 hue++ |
        hue2 = 0U;                                // Brightness
        ff_x = 1U;                                // counter
        tMAX = 100U;                              // timeout
    }
    divider = floor((modes[currentMode].Scale - 1) / 10); // маштаб задает режим рестарта
    tMAX = timeout + 100 * divider;

    if ((ff_x > timeout - 10) && (ff_x < timeout))   // таймаут блокировки отрисовки флага
    {
        if (ff_x < timeout - 5)                    // размытие тризуба
        {
            blurScreen(beatsin8(5U, 60U, 5U));
        }
        else
        {
            blurScreen(210U - ff_x);
        }
    }

    if (ff_x > tMAX)
    {
        if (divider == 0U)                         // отрисовка тризуба только раз
        {
            ff_x = 0U;
            tMAX += 20;
        }
        else
        {
            if (ff_x > tMAX + 100U * divider)        // рестар эффект
            {
                drawCrest();
                ff_x = 1U;
            }
        }
    }
    if ((ff_x != 0U) || (divider > 0))
    {
        ff_x++;
    }

    // Flag Draw =============================
    if ((ff_x > timeout) || (ff_x == 0U))        // отрисовка флага
    {
        if (step >= deltaValue)
        {
            step = 0U;
            hue2 = random8(WIDTH - 2);               // случайное смещение мазка по оси Y
            hue = random8(5);                        // flag color
            // blurScreen(dim8_raw(beatsin8(3, 64, 100)));
            // blurScreen(beatsin8(5U, 60U, 5U));
            // dimAll(200U);
        }
        if (step % 8 == 0 && modes[currentMode].Speed > 230)
        {
            blurScreen(beatsin8(5U, 5U, 72U));
        }
        hue2++;                                    // x
        deltaHue2++;                               // y

        if (hue2 >= WIDTH)
        {
            if (deltaHue2 > HEIGHT - 2)              // если матрица высокая дорисовываем остальные мазки
            {
                deltaHue2 = random8(5);                // изменяем положение по Y только отрисовав весь флаг
            }
            if (step % 2 == 0)
            {
                hue2 = 0U;
            }
            else
            {
                hue2 = random8(WIDTH);                 // смещение первого мазка по оси X
            }
        }

        if (deltaHue2 >= HEIGHT)
        {
            deltaHue2 = 0U;
            if (deltaValue > 200U)
            {
                hue = random8(5);                      // если низкая скорость меняем цвет после каждого витка
            }
        }

        if (deltaHue2 > floor(HEIGHT / 2) - 1)      // меняем цвет для разных частей флага
        {
            color = colors[0][hue];
        }
        else
        {
            color = colors[1][hue];
        }

        // LOG.printf_P(PSTR("color = %08d | hue2 = %d | speed = %03d | custom_eff = %d\n"), color, hue2, deltaValue, custom_eff);
        drawPixelXY(hue2, deltaHue2, color);
        // ----------------------------------
        step++;
    }
}

// ============ Oil Paints ==============
//      © SlingMaster | by Alex Dovby
//              EFF_PAINT
//           Масляные Краски
//---------------------------------------
void OilPaints()
{
    uint8_t wave;
    uint8_t divider;
    uint8_t entry_point;
    uint16_t value;
    uint16_t max_val;
    if (loadingFlag)
    {

#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
        if (selectedSettings)
        {
            //                          scale | speed 210
            setModeSettings(1U + random8(252U), 1 + random8(219U));
        }
#endif
        loadingFlag = false;
        FastLED.clear();
        // blurScreen(beatsin8(5U, 50U, 5U));
        deltaValue = 255U - modes[currentMode].Speed + 1U;
        step = deltaValue;                    // чтообы при старте эффекта сразу покрасить лампу
        hue = floor(21.25 * (random8(11) + 1)); // next color
        deltaHue = hue - 22;                  // last color
        deltaHue2 = 80;                       // min bright
        wave = floor(HEIGHT * 0.5);           // position swap color
        max_val = pow(2, WIDTH);
        //    for ( int i = WIDTH; i < (NUM_LEDS - WIDTH); i++) {
        //      leds[i] = CHSV(120U, 24U, 64U);
        //    }
    }

    // !!! ********
    // если собираетесь эксплуатировать custom_eff на постоянной основе а не динамически
    // то закомментируйте етоти блоки в коде, так как это уже не маслянные краски а акварель
    // или "побочный эффект" название забил на будущее
    //  if (custom_eff == 1) {
    //    if (step % wave - 1 == 0) {
    //      drawPixelXY(random8(WIDTH), 1U + random8(4), CHSV(hue + 180, 255U, 255U));
    //      drawPixelXY(random8(WIDTH), 1U + random8(4), CHSV(hue + 90, 255U, 255U));
    //    }
    //    blurScreen(32U);
    //    // blurScreen(beatsin8(2U, 7U, 2U));
    //  }
    // ************

    if (step >= deltaValue)
    {
        step = 0U;
        // LOG.printf_P(PSTR("%03d | log: %f | val: %03d\n\r"), modes[currentMode].Brightness, log(modes[currentMode].Brightness), deltaHue2);
    }

    // Create Oil Paints --------------
    // выбираем краски  ---------------
    if (step % wave == 0)
    {
        divider = floor((modes[currentMode].Scale - 1) / 10);             // маштаб задает диапазон изменения цвета
        deltaHue = hue;                                                   // set last color
        hue += 6 * divider;                                               // new color
        hue2 = 255;                                                       // restore brightness
        deltaHue2 = 80 - floor(log(modes[currentMode].Brightness) * 6);   // min bright
        entry_point = random8(WIDTH);                                     // start X position
        trackingObjectHue[entry_point] = hue;                             // set start position
        drawPixelXY(entry_point,  HEIGHT - 2, CHSV(hue, 255U, 255U));
        // !!! ********
        if (custom_eff == 1)
        {
            drawPixelXY(entry_point + 1,  HEIGHT - 3, CHSV(hue + 30, 255U, 255U));
        }
        // ************
        // LOG.printf_P(PSTR("BR %03d | SP %03d | SC %03d | hue %03d\n\r"), modes[currentMode].Brightness, modes[currentMode].Speed, modes[currentMode].Scale, hue);
    }

    // формируем форму краски, плавно расширяя струю ----
    if (random8(3) == 1)
    {
        // LOG.println("<--");
        for (uint8_t x = 1U; x < WIDTH; x++)
        {
            if (trackingObjectHue[x] == hue)
            {
                trackingObjectHue[x - 1] = hue;
                break;
            }
        }
    }
    else
    {
        // LOG.println("-->");
        for (uint8_t x = WIDTH - 1; x > 0U ; x--)
        {
            if (trackingObjectHue[x] == hue)
            {
                trackingObjectHue[x + 1] = hue;
                break;
            }
            // LOG.printf_P(PSTR("x = %02d | value = %03d | hue = %03d \n\r"), x, trackingObjectHue[x], hue);
        }
    }
    // LOG.println("------------------------------------");

    // выводим сформированную строку --------------------- максимально яркую в момент смены цвета
    for (uint8_t x = 0U; x < WIDTH; x++)
    {
        //                                                                                set color  next |    last  |
        drawPixelXY(x,  HEIGHT - 1, CHSV(trackingObjectHue[x], 255U, (trackingObjectHue[x] == hue) ? hue2 : deltaHue2));
    }
    //  LOG.println("");
    // уменьшаем яркость для следующих строк
    if (hue2 > (deltaHue2 + 16))
    {
        hue2 -= 16U;
    }
    // сдвигаем неравномерно поток вниз ---
    value = random16(max_val);
    //LOG.printf_P(PSTR("value = %06d | "), value);
    for (uint8_t x = 0U; x < WIDTH; x++)
    {
        if (bitRead(value, x) == 0)
        {
            //LOG.print (" X");
            for (uint8_t y = 0U; y < HEIGHT - 1; y++)
            {
                drawPixelXY(x, y, getPixColorXY(x, y + 1U));
            }
        }
    }
    // LOG.printf_P(PSTR("%02d | hue2 = %03d | min = %03d \n\r"), step, hue2, deltaHue2);
    // -------------------------------------

    step++;
}


// ========== Botswana Rivers ===========
//      © SlingMaster | by Alex Dovby
//              EFF_RIVERS
//            Реки Ботсваны

//---------------------------------------
void flora()
{
    uint32_t  FLORA_COLOR = 0x2F1F00;
    uint8_t posX =  floor(WIDTH * 0.5) - (WIDTH * 0.3);
    uint8_t h =  random8(HEIGHT - 6U) + 4U;
    DrawLine(posX + 1, 1U, posX + 1, h - 1, 0x000000);
    DrawLine(posX + 2, 1U, posX + 2, h, FLORA_COLOR);
    drawPixelXY(posX + 2, h - random8(floor(h * 0.5)), random8(2U) == 1 ? 0xFF00E0 :  random8(2U) == 1 ? 0xFFFF00 : 0x00FF00);
    drawPixelXY(posX + 1, h - random8(floor(h * 0.25)), random8(2U) == 1 ? 0xFF00E0 : 0xFFFF00);
    if (random8(2U) == 1)
    {
        drawPixelXY(posX + 1, floor(h * 0.5), random8(2U) == 1 ? 0xEF001F :  0x9FFF00);
    }
    h =  floor(h * 0.65);
    if (WIDTH > 8)
    {
        DrawLine(posX - 1, 1U, posX - 1, h - 1, 0x000000);
    }
    DrawLine(posX, 1U, posX, h, FLORA_COLOR);
    drawPixelXY(posX, h - random8(floor(h * 0.5)), random8(2U) == 1 ? 0xFF00E0 : 0xFFFF00);
}

//---------------------------------------
void animeBobbles()
{
    uint8_t center = floor(WIDTH * 0.5) + 1U;
    // сдвигаем всё вверх ----
    for (uint8_t x = center; x < WIDTH; x++)
    {
        for (uint8_t y = HEIGHT; y > 0U; y--)
        {
            if (getPixColorXY(x, y - 1) == 0xFFFFF7)
            {
                drawPixelXY(x, y, 0xFFFFF7);
                drawPixelXY(x, y - 1, getPixColorXY(0, y - 1));
            }
        }
    }
    // ----------------------

    if (step % 4 == 0)
    {
        drawPixelXY(center + random8(5), 0U, 0xFFFFF7);
        if (step % 12 == 0)
        {
            drawPixelXY(center + 2 + random8(3), 0U, 0xFFFFF7);
        }
    }
}

//---------------------------------------
void gradientDownTop(uint8_t bottom, CHSV bottom_color, uint8_t top, CHSV top_color)
{
    //  FORWARD_HUES: hue always goes clockwise
    //  BACKWARD_HUES: hue always goes counter-clockwise
    //  SHORTEST_HUES: hue goes whichever way is shortest
    //  LONGEST_HUES: hue goes whichever way is longest
    if (STRIP_DIRECTION < 2)
    {
        // STRIP_DIRECTION to UP ========
        fill_gradient(leds, top * WIDTH, top_color, bottom * WIDTH, bottom_color, SHORTEST_HUES);
    }
    else
    {
        // STRIP_DIRECTION to DOWN ======
        fill_gradient(leds, NUM_LEDS - bottom * WIDTH - 1, bottom_color, NUM_LEDS - top * WIDTH, top_color, SHORTEST_HUES);
    }
    //  FastLED.show();
}

//---------------------------------------
void createScene(uint8_t idx)
{
    switch (idx)
    {
        case 0:     // blue green ------
            gradientDownTop(floor((HEIGHT - 1) * 0.5), CHSV(96, 255, 100), HEIGHT, CHSV(160, 255, 255));
            gradientDownTop(0, CHSV(96, 255, 255), floor(HEIGHT * 0.5), CHSV(96, 255, 100));
            break;
        case 1:     // aquamarine green
            gradientDownTop(floor((HEIGHT - 1) * 0.3), CHSV(96, 255, 100), HEIGHT, CHSV(130, 255, 220));
            gradientDownTop(0, CHSV(96, 255, 255), floor(HEIGHT * 0.3), CHSV(96, 255, 100));
            break;
        case 2:     // blue aquamarine -
            gradientDownTop(floor((HEIGHT - 1) * 0.5), CHSV(170, 255, 100), HEIGHT, CHSV(160, 255, 200));
            gradientDownTop(0, CHSV(100, 255, 255), floor(HEIGHT * 0.5), CHSV(170, 255, 100));
            break;
        case 3:     // yellow green ----
            gradientDownTop(floor((HEIGHT - 1) * 0.5), CHSV(95, 255, 55), HEIGHT, CHSV(70, 255, 200));
            gradientDownTop(0, CHSV(95, 255, 255), floor(HEIGHT * 0.5), CHSV(100, 255, 55));
            break;
        case 4:     // sea green -------
            gradientDownTop(floor((HEIGHT - 1) * 0.3), CHSV(120, 255, 55), HEIGHT, CHSV(175, 255, 200));
            gradientDownTop(0, CHSV(120, 255, 255), floor(HEIGHT * 0.3), CHSV(120, 255, 55));
            break;
        default:
            gradientDownTop(floor((HEIGHT - 1) * 0.25), CHSV(180, 255, 85), HEIGHT, CHSV(160, 255, 200));
            gradientDownTop(0, CHSV(80, 255, 255), floor(HEIGHT * 0.25), CHSV(180, 255, 85));
            break;
    }
    flora();

    //  TEST --------------
    //  gradientDownTop(31, CHSV(0, 255, 255), 32, CHSV(0, 255, 50));
    //  gradientDownTop(14, CHSV(0, 255, 255), 15, CHSV(0, 255, 255));
    //  DrawLine(0, 12U, WIDTH, 12U, 0xff0000);
    //  gradientDownTop(0, CHSV(0, 255, 255), 32, CHSV(0, 255, 50));
}

//---------------------------------------
void BotswanaRivers()
{
    uint8_t divider;

    if (loadingFlag)
    {
#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
        if (selectedSettings)
        {
            //                          scale | speed 210
            setModeSettings(1U + random8(252U), 20 + random8(180U));
        }
#endif
        loadingFlag = false;
        deltaValue = 255U - modes[currentMode].Speed + 1U;
        step = deltaValue;                                          // чтообы при старте эффекта сразу покрасить лампу
        divider = floor((modes[currentMode].Scale - 1) / 20);       // маштаб задает смену палитры воды
        createScene(divider);
    }

    if (step >= deltaValue)
    {
        step = 0U;
    }

    // restore scene after power on ---------
    if (getPixColorXY(0U, HEIGHT - 2) == CRGB::Black)
    {
        createScene(divider);
    }

    // light at the bottom ------------------
    if (step % 2 == 0)
    {
        if (random8(6) == 1)
        {
            //fill_gradient(leds, NUM_LEDS - WIDTH, CHSV(96U, 255U, 200U), NUM_LEDS, CHSV(50U, 255U, 255U), SHORTEST_HUES);
            if (STRIP_DIRECTION < 2)
            {
                fill_gradient(leds, 0, CHSV(96U, 255U, 190U), random8(WIDTH + random8(6)), CHSV(90U, 200U, 255U), SHORTEST_HUES);
            }
            else
            {
                fill_gradient(leds, NUM_LEDS - random8(WIDTH + random8(6)), CHSV(96U, 255U, 190U), NUM_LEDS, CHSV(90U, 200U, 255U), SHORTEST_HUES);
            }
        }
        else
        {
            //fill_gradient(leds, NUM_LEDS - WIDTH, CHSV(50U, 128U, 255U), NUM_LEDS, CHSV(90U, 255U, 180U), SHORTEST_HUES);
            if (STRIP_DIRECTION < 2)
            {
                fill_gradient(leds, 0, CHSV(85U, 128U, 255U), random8(WIDTH), CHSV(90U, 255U, 180U), SHORTEST_HUES);
            }
            else
            {
                fill_gradient(leds, NUM_LEDS - random8(WIDTH), CHSV(85U, 128U, 255U), NUM_LEDS, CHSV(90U, 255U, 180U), SHORTEST_HUES);
            }
        }
        //    FastLED.show();
    }

    //  // LOG.printf_P(PSTR("%02d | hue2 = %03d | min = %03d \n\r"), step, hue2, deltaHue2);
    // -------------------------------------
    animeBobbles();
    if (custom_eff == 1)
    {
        blurRows(leds, WIDTH, 3U, 10U);
        // blurScreen(beatsin8(0U, 5U, 0U));
    }
    step++;
}


// ============ Watercolor ==============
//      © SlingMaster | by Alex Dovby
//            EFF_WATERCOLOR
//               Акварель
//---------------------------------------
void SmearPaint(uint8_t obj[trackingOBJECT_MAX_COUNT])
{
    uint8_t divider;
    int temp;
    static const uint32_t colors[6][8] PROGMEM =
    {
        {0x2F0000,  0xFF4040, 0x6F0000, 0xAF0000, 0xff5f00, CRGB::Red, 0x480000, 0xFF0030},
        {0x002F00, CRGB::LawnGreen, 0x006F00, 0x00AF00, CRGB::DarkMagenta, 0x00FF00, 0x004800, 0x00FF30},
        {0x002F1F, CRGB::DarkCyan, 0x00FF7F, 0x007FFF, 0x20FF5F, CRGB::Cyan, 0x004848, 0x7FCFCF },
        {0x00002F, 0x5030FF, 0x00006F, 0x0000AF, CRGB::DarkCyan, 0x0000FF, 0x000048, 0x5F5FFF},
        {0x2F002F, 0xFF4040, 0x6F004A, 0xFF0030, CRGB::DarkMagenta, CRGB::Magenta, 0x480048, 0x3F00FF},
        {CRGB::Blue, CRGB::Red, CRGB::Gold, CRGB::Green, CRGB::DarkCyan, CRGB::DarkMagenta, 0x000000, 0xFF7F00 }
    };
    if (trackingObjectHue[5] == 1)    // direction >>>
    {
        obj[1]++;
        if (obj[1] >= obj[2])
        {
            trackingObjectHue[5] = 0;     // swap direction
            obj[3]--;                     // new line
            if (step % 2 == 0)
            {
                obj[1]++;
            }
            else
            {
                obj[1]--;
            }

            obj[0]--;
        }
    }
    else                              // direction <<<
    {
        obj[1]--;
        if (obj[1] <= (obj[2] - obj[0]))
        {
            trackingObjectHue[5] = 1;     // swap direction
            obj[3]--;                     // new line
            if (obj[0] >= 1)
            {
                temp = obj[0] - 1;
                if (temp < 0)
                {
                    temp = 0;
                }
                obj[0] = temp;
                obj[1]++;
            }
        }
    }

    if (obj[3] == 255)
    {
        deltaHue = 255;
    }

    divider = floor((modes[currentMode].Scale - 1) / 16.7);
    if ((obj[1] >= WIDTH) || (obj[3] == obj[4]))
    {
        // deltaHue value == 255 activate -------
        // set new parameter for new smear ------
        deltaHue = 255;
    }
    drawPixelXY(obj[1], obj[3], colors[divider][hue]);

    // alternative variant without dimmer effect
    // uint8_t h = obj[3] - obj[4];
    // uint8_t br = 266 - 12 * h;
    // if (h > 0) {
    // drawPixelXY(obj[1], obj[3], makeDarker(colors[divider][hue], br));
    // } else {
    // drawPixelXY(obj[1], obj[3], makeDarker(colors[divider][hue], 240));
    // }
}



//---------------------------------------
void Watercolor()
{
    // #define DIMSPEED (254U - 500U / WIDTH / HEIGHT)
    uint8_t divider;
    if (loadingFlag)
    {

#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
        if (selectedSettings)
        {
            //                          scale | speed 250
            setModeSettings(1U + random8(252U), 1 + random8(250U));
        }
#endif
        loadingFlag = false;
        FastLED.clear();
        deltaValue = 255U - modes[currentMode].Speed + 1U;
        step = deltaValue;                    // чтообы при старте эффекта сразу покрасить лампу
        hue = 0;
        deltaHue = 255;                       // last color
        trackingObjectHue[1] = floor(WIDTH * 0.25);
        trackingObjectHue[3] = floor(HEIGHT * 0.25);
    }

    if (step >= deltaValue)
    {
        step = 0U;
        // LOG.printf_P(PSTR("%03d | log: %f | val: %03d | divider: %d \n\r"), modes[currentMode].Brightness, log(modes[currentMode].Brightness), deltaHue2, divider);
    }

    // ******************************
    // set random parameter for smear
    // ******************************
    if (deltaHue == 255)
    {

        trackingObjectHue[0] = 4 + random8(floor(WIDTH * 0.25));                // width
        trackingObjectHue[1] = random8(WIDTH - trackingObjectHue[0]);           // x
        int temp =  trackingObjectHue[1] + trackingObjectHue[0];
        if (temp >= (WIDTH - 1))
        {
            temp = WIDTH - 1;
            if (trackingObjectHue[1] > 1)
            {
                trackingObjectHue[1]--;
            }
            else
            {
                trackingObjectHue[1]++;
            }
        }
        trackingObjectHue[2] = temp;                                            // x end
        trackingObjectHue[3] = 3 + random8(HEIGHT - 4);                         // y
        temp = trackingObjectHue[3] - random8(3) - 3;
        if (temp <= 0)
        {
            temp = 0;
        }
        trackingObjectHue[4] = temp;                                            // y end
        trackingObjectHue[5] = 1;
        divider = floor((modes[currentMode].Scale - 1) / 16.7);                 // маштаб задает смену палитры
        hue = random8(8);
        //    if (step % 127 == 0) {
        //      LOG.printf_P(PSTR("BR %03d | SP %03d | SC %03d | divider %d | [ %d ]\n\r"), modes[currentMode].Brightness, modes[currentMode].Speed, modes[currentMode].Scale, divider, hue);
        //    }
        hue2 = 255;
        deltaHue = 0;
    }
    // ******************************
    SmearPaint(trackingObjectHue);

    // LOG.printf_P(PSTR("%02d | hue2 = %03d | min = %03d \n\r"), step, hue2, deltaHue2);
    // -------------------------------------
    //  if (custom_eff == 1) {
    // dimAll(DIMSPEED);
    if (step % 2 == 0)
    {
        blurScreen(beatsin8(1U, 1U, 6U));
        // blurRows(leds, WIDTH, 3U, 10U);
    }
    //  }
    step++;
}

// =========== FeatherCandle ============
//         адаптация © SottNick
//    github.com/mnemocron/FeatherCandle
//      modify & design © SlingMaster
//           EFF_FEATHER_CANDLE
//                Свеча
//---------------------------------------
#include "data7x15flip.h"                       // FeatherCandle animation data
const uint8_t  level = 160;
const uint8_t  low_level = 110;
const uint8_t* ptr  = anim;                     // Current pointer into animation data
const uint8_t  w    = 7;                        // image width
const uint8_t  h    = 15;                       // image height
uint8_t        img[w * h];                      // Buffer for rendering image
uint8_t        deltaX = floor(WIDTH * 0.5) - 4; // position img
uint8_t last_brightness;
void FeatherCandleRoutine()
{
#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
    if (selectedSettings)
    {
        // brightness | scale | speed
        // { 21, 220,  40}
        setModeSettings(1U + random8(100U), 190U + random8(65U));
    }
#endif
    if (loadingFlag)
    {
        FastLED.clear();
        hue = 0;
        gradientDownTop(0, CHSV(255U, 0U, 128U), 2, CHSV(50U, 255, 128U));
        trackingObjectState[0] = low_level;
        trackingObjectState[1] = low_level;
        trackingObjectState[2] = low_level;
        trackingObjectState[4] = floor(WIDTH * 0.5);
        loadingFlag = false;
    }

    uint8_t a = pgm_read_byte(ptr++);     // New frame X1/Y1
    if (a >= 0x90)                        // EOD marker? (valid X1 never exceeds 8)
    {
        ptr = anim;                         // Reset animation data pointer to start
        a   = pgm_read_byte(ptr++);         // and take first value
    }
    uint8_t x1 = a >> 4;                  // X1 = high 4 bits
    uint8_t y1 = a & 0x0F;                // Y1 = low 4 bits
    a  = pgm_read_byte(ptr++);            // New frame X2/Y2
    uint8_t x2 = a >> 4;                  // X2 = high 4 bits
    uint8_t y2 = a & 0x0F;                // Y2 = low 4 bits

    // Read rectangle of data from anim[] into portion of img[] buffer
    for (uint8_t y = y1; y <= y2; y++)
        for (uint8_t x = x1; x <= x2; x++)
        {
            img[y * w + x] = pgm_read_byte(ptr++);
        }
    int i = 0;
    uint8_t color = (modes[currentMode].Scale - 1U) * 2.57;



    // draw flame -------------------
    for (uint8_t y = 1; y < h; y++)
    {
        if (HEIGHT < 15)
        {
            // for small matrix -----
            if (y % 2 == 0)
            {
                leds[XY(floor(WIDTH * 0.5) - 2, 7)] = CHSV(color, 255U, 55 + random8(200));
                leds[XY(floor(WIDTH * 0.5) - 1, 5)] = CHSV(color, 255U, 205 + random8(50));
                leds[XY(floor(WIDTH * 0.5) - 1, 6)] = CHSV(color, 255U, 155 + random8(100));
                leds[XY(floor(WIDTH * 0.5) - 2, 4)] = CHSV(color - 10U, 255U, 120 + random8(130));
                leds[XY(floor(WIDTH * 0.5), 4)] = CHSV(color - 10U, 255U, 100 + random8(120));
            }
        }
        else
        {
            for (uint8_t x = 0; x < w; x++)
            {
                uint8_t brightness = img[i];
                leds[XY(deltaX + x, y)] = CHSV(brightness > 240 ? color : color - 10U, 255U, brightness);
                i++;
            }
        }
        // draw body FeatherCandle ------
        if (y <= 4)
        {
            if (y % 2 == 0)
            {
                gradientDownTop(0, CHSV(50U, 16U, 32U), 3U, CHSV(color, 96U, 128U));
            }
        }

        // drops of wax move -------------
        switch (hue)
        {
            case 0:
                if (trackingObjectState[0] < level)
                {
                    trackingObjectState[0]++;
                }
                break;
            case 1:
                if (trackingObjectState[0] > low_level)
                {
                    trackingObjectState[0] --;
                }
                if (trackingObjectState[1] < level)
                {
                    trackingObjectState[1] ++;
                }
                break;
            case 2:
                if (trackingObjectState[1] > low_level)
                {
                    trackingObjectState[1] --;
                }
                if (trackingObjectState[2] < level)
                {
                    trackingObjectState[2] ++;
                }
                break;
            case 3:
                if (trackingObjectState[2] > low_level)
                {
                    trackingObjectState[2] --;
                }
                else
                {
                    hue++;
                    // set random position drop of wax
                    trackingObjectState[4] = floor(WIDTH * 0.5) - 3 + random8(6);
                }
                break;
        }

        if (hue > 3)
        {
            hue++;
        }
        else
        {
            // LOG.printf_P(PSTR("[0] = %03d | [1] = %03d | [2] = %03d \n\r"), trackingObjectState[0], trackingObjectState[1], trackingObjectState[2]);
            if (hue < 2)
            {
                leds[XY(trackingObjectState[4], 2)] = CHSV(50U, 20U, trackingObjectState[0]);
            }
            if ((hue == 1) || (hue == 2))
            {
                leds[XY(trackingObjectState[4], 1)] = CHSV(50U, 15U, trackingObjectState[1]); // - 10;
            }
            if (hue > 1)
            {
                leds[XY(trackingObjectState[4], 0)] = CHSV(50U, 5U, trackingObjectState[2]); // - 20;
            }
        }
    }

    // next -----------------
    if ((trackingObjectState[0] == level) || (trackingObjectState[1] == level) || (trackingObjectState[2] == level))
    {
        hue++;
    }

}
