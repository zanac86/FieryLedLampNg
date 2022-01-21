// Текущая версия / Current version : v2.31_web_v2.2  ( gunner47_v2.2_web_v2.1_94in1 )
// ВНИМАНИЕ!!! Большая часть настроек перенесена в файл data/config и может изменяться в процессе эксплуатации лампы.
// Внимательно читайте файл ПРОЧТИ МЕНЯ!!!.txt и ПРОЧТИ МЕНЯ.doc (тут с картинками)

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <ESP8266WebServer.h>

// ============= НАСТРОЙКИ =============
//#define USE_SECRET_COMMANDS                               // удалите эту строку, если вам не нужна возможность смены режимов работы ESP_MODE и обнуления настроек из приложения
// список секретных команд тут: https://community.alexgyver.ru/goto/post?id=55780
#define USE_SHUFFLE_FAVORITES                             // раскомментируйте эту строку, если вам нужно, чтобы режим Цикл показал каждый эффект по 1 разу перед перемешиванием (иначе просто случайный эффект),
#define USE_DEFAULT_SETTINGS_RESET                          // закомментируйте или удалите эту строку, если не нужно, чтобы при загрузке списка эффектов из лампы в приложение настройки эффектов сбрасывались на значения по умолчанию

// --- КНОПКА --------------------------
// #define ESP_USE_BUTTON                                      // если строка не закомментирована, должна быть подключена кнопка (иначе лампа может регистрировать "фантомные" нажатия и некорректно устанавливать яркость)
#define BUTTON_IS_SENSORY     (1)                           // если у вас не сенсорная, а обычная кнопка, поменяйте в этой строчке 1 на нолик 
#define BUTTON_LOCK_ON_START                                // с этой строкой, если в момент включения лампы в розетку успеть нажать кнопку, или если вы забудете кнопку подключить, лампа отключит реакцию на нажатие кнопки до следующего раза
//#define BUTTON_CHANGE_FAVORITES_MODES_ONLY                // Выберите чекбокс на web странице лампы, если хотите, чтобы кнопка переключала режимы только между теми, которые выбраны для режима Цикл (настраивается в приложении)
//Перенесено в файл data/config.json. Имя поля favorit      // иначе переключаться будут все существующие в лампе режимы по порядку (двойным кликом вперёд, тройным назад)
//Чекбокс "Кнопкой - только эффекты, выбранные в Цикле"

// --- ESP -----------------------------
//#define esp_Mode                                          // Если чекбокс на web странице не отмечен - лампа работает как WiFi точка доступа всегда (работа без роутера),
// Перенесено в файл data/config.json. Имя поля ESP_mode    // Если чекбокс на web странице  отмечен - лампа - клиент WiFi (работа через роутер). Подключение к роутеру настраивается на web странице
//Чекбокс "Использовать подключение к роутеру"              // данный выбор режима влияет только на первую загрузку прошивки. потом он сохраняется в настройках и может изменяться в процессе эксплуатации лампы
// подробнее можно почитать по ссылке: https://community.alexgyver.ru/goto/post?id=33674
#define ESP_CONF_TIMEOUT      (60U)                         // время в секундах, которое лампа будет ждать от вас введения пароля для ОТА обновления (пароль совпадает с паролем точки доступа)


//#define LED_PIN               (2U)                          // пин ленты                (D4) - встроенный светодиод платы тоже на D4, поэтому лучше паять на D3 = #define LED_PIN (0U)
#define LED_PIN               (0U)                          // пин ленты                (D4) - встроенный светодиод платы тоже на D4, поэтому лучше паять на D3 = #define LED_PIN (0U)
#define BTN_PIN               (4U)                          // пин кнопки               (D2)

//#define DISPLAY_IP_AT_START                             // Раскоментируйте эту строчку,если хотите, чтобы при включении пмтания и подключению к WiFi, лампа один раз выводила свой IP адрес

// --- МАТРИЦА -------------------------
#define CURRENT_LIMIT         (2400U)                       // лимит по току в миллиамперах, автоматически управляет яркостью (пожалей свой блок питания!) 0 - выключить лимит

#define WIDTH                 (16U)                         // ширина матрицы
#define HEIGHT                (16U)                         // высота матрицы

#define COLOR_ORDER           (GRB)                         // порядок цветов на ленте. Если цвет отображается некорректно - меняйте. Начать можно с RGB

#define MATRIX_TYPE           (0U)                          // тип матрицы: 0 - зигзаг, 1 - параллельная
#define CONNECTION_ANGLE      (0U)                          // угол подключения: 0 - левый нижний, 1 - левый верхний, 2 - правый верхний, 3 - правый нижний
#define STRIP_DIRECTION       (0U)                          // направление ленты из угла: 0 - вправо, 1 - вверх, 2 - влево, 3 - вниз
// при неправильной настройке матрицы вы получите предупреждение "Wrong matrix parameters! Set to default"
// шпаргалка по настройке матрицы здесь: https://alexgyver.ru/wp-content/uploads/2018/11/scheme3.jpg
// (но в ней, кажется, перепутаны местами квадратики в 4м столбце, а может, и ещё какие-то)

// --- ЭФФЕКТЫ -------------------------
#define RUNNING_TEXT_DEFAULT  ("Привет!")                   // текст, который будет выводиться в эффекте Бегущая строка по умолчанию. его можно менять в приложении
//Управляется на web странице лампы                         // настройки подбирались для лампы с матрицей 16х16 со стеклянным плафоном и калькой под ним. на других - не гарантируется
// Перенесено в файл data/config.json.                      // этот режим можно включать/выключать на web странице и секретной командой. чтобы после первой загрузки прошивки в плату он был выключен, поменяйте параметр random_on c 1 на 0 в файле cnfig.json.

// == названия и номера эффектов ниже в списке используются на вкладке effectTricker ==
// == если меняете, меняйте и там, и ещё здесь ниже в РЕЕСТРЕ ДОСТУПНЫХ ЭФФЕКТОВ ==
#define EFF_WHITE_COLOR         ( 0U)    // Бeлый cвeт
#define EFF_COLOR               ( 1U)    // Цвeт
#define EFF_COLORS              ( 2U)    // Cмeнa цвeтa
#define EFF_MADNESS             ( 3U)    // Бeзyмиe
#define EFF_CLOUDS              ( 4U)    // Oблaкa
#define EFF_LAVA                ( 5U)    // Лaвa
#define EFF_PLASMA              ( 6U)    // Плaзмa
#define EFF_RAINBOW             ( 7U)    // Paдyгa 3D
#define EFF_RAINBOW_STRIPE      ( 8U)    // Пaвлин
#define EFF_ZEBRA               ( 9U)    // 3eбpa
#define EFF_FOREST              (10U)    // Лec
#define EFF_OCEAN               (11U)    // Oкeaн
#define EFF_BBALLS              (12U)    // Mячики
#define EFF_BALLS_BOUNCE        (13U)    // Mячики бeз гpaниц
#define EFF_POPCORN             (14U)    // Пoпкopн
#define EFF_SPIRO               (15U)    // Cпиpaли
#define EFF_PRISMATA            (16U)    // Пpизмaтa
#define EFF_SMOKEBALLS          (17U)    // Дымoвыe шaшки
#define EFF_FLAME               (18U)    // Плaмя
#define EFF_FIRE_2021           (19U)    // Oгoнь 2021
#define EFF_PACIFIC             (20U)    // Tиxий oкeaн
#define EFF_SHADOWS             (21U)    // Teни
#define EFF_DNA                 (22U)    // ДHK
#define EFF_FLOCK               (23U)    // Cтaя
#define EFF_FLOCK_N_PR          (24U)    // Cтaя и xищник
#define EFF_BUTTERFLYS          (25U)    // Moтыльки
#define EFF_BUTTERFLYS_LAMP     (26U)    // Лaмпa c мoтылькaми
#define EFF_SNAKES              (27U)    // 3мeйки
#define EFF_NEXUS               (28U)    // Nexus
#define EFF_SPHERES             (29U)    // Шapы
#define EFF_SINUSOID3           (30U)    // Cинycoид
#define EFF_METABALLS           (31U)    // Meтaбoлз
#define EFF_AURORA              (32U)    // Ceвepнoe cияниe
#define EFF_SPIDER              (33U)    // Плaзмeннaя лaмпa
#define EFF_LAVALAMP            (34U)    // Лaвoвaя лaмпa
#define EFF_LIQUIDLAMP          (35U)    // Жидкaя лaмпa
#define EFF_LIQUIDLAMP_AUTO     (36U)    // Жидкaя лaмпa (auto)
#define EFF_DROPS               (37U)    // Kaпли нa cтeклe
#define EFF_MATRIX              (38U)    // Maтpицa
#define EFF_FIRE_2012           (39U)    // Oгoнь 2012
#define EFF_FIRE_2018           (40U)    // Oгoнь 2018
#define EFF_FIRE_2020           (41U)    // Oгoнь 2020
#define EFF_FIRE                (42U)    // Oгoнь
#define EFF_WHIRL               (43U)    // Bиxpи плaмeни
#define EFF_WHIRL_MULTI         (44U)    // Paзнoцвeтныe виxpи
#define EFF_MAGMA               (45U)    // Maгмa
#define EFF_LLAND               (46U)    // Kипeниe
#define EFF_WATERFALL           (47U)    // Boдoпaд
#define EFF_WATERFALL_4IN1      (48U)    // Boдoпaд 4 в 1
#define EFF_POOL                (49U)    // Бacceйн
#define EFF_PULSE               (50U)    // Пyльc
#define EFF_PULSE_RAINBOW       (51U)    // Paдyжный пyльc
#define EFF_PULSE_WHITE         (52U)    // Бeлый пyльc
#define EFF_OSCILLATING         (53U)    // Ocциллятop
#define EFF_FOUNTAIN            (54U)    // Иcтoчник
#define EFF_FAIRY               (55U)    // Фeя
#define EFF_COMET               (56U)    // Koмeтa
#define EFF_COMET_COLOR         (57U)    // Oднoцвeтнaя кoмeтa
#define EFF_COMET_TWO           (58U)    // Двe кoмeты
#define EFF_COMET_THREE         (59U)    // Тpи кoмeты
#define EFF_LUMENJER            (60U)    // Люмeньep
#define EFF_ATTRACT             (61U)    // Пpитяжeниe
#define EFF_FIREFLY             (62U)    // Пapящий oгoнь
#define EFF_FIREFLY_TOP         (63U)    // Bepxoвoй oгoнь
#define EFF_SNAKE               (64U)    // Paдyжный змeй
#define EFF_SPARKLES            (65U)    // Koнфeтти
#define EFF_TWINKLES            (66U)    // Mepцaниe
#define EFF_SMOKE               (67U)    // Дым
#define EFF_SMOKE_COLOR         (68U)    // Paзнoцвeтный дым
#define EFF_PICASSO             (69U)    // Пикacco
#define EFF_WAVES               (70U)    // Boлны
#define EFF_SAND                (71U)    // Цвeтныe дpaжe
#define EFF_RINGS               (72U)    // Koдoвый зaмoк
#define EFF_CUBE2D              (73U)    // Kyбик Pyбикa
#define EFF_SIMPLE_RAIN         (74U)    // Tyчкa в бaнкe
#define EFF_STORMY_RAIN         (75U)    // Гроза в банке
#define EFF_COLOR_RAIN          (76U)    // Ocaдки
#define EFF_RAIN                (77U)    // Paзнoцвeтный дoждь
#define EFF_SNOW                (78U)    // Cнeгoпaд
#define EFF_STARFALL            (79U)    // 3вeздoпaд / Meтeль
#define EFF_LEAPERS             (80U)    // Пpыгyны
#define EFF_LIGHTERS            (81U)    // Cвeтлячки
#define EFF_LIGHTER_TRACES      (82U)    // Cвeтлячки co шлeйфoм
#define EFF_PAINTBALL           (83U)    // Пeйнтбoл
#define EFF_RAINBOW_VER         (84U)    // Paдyгa
#define EFF_RIVERS              (85U)    // Реки Ботсваны
#define EFF_SWIRL               (86U)    // Завиток
#define EFF_UKRAINE             (87U)    // Украина
#define EFF_WINE                (88U)    // Вино
#define EFF_PAINTS              (89U)    // Масляные Краски
#define EFF_WATERCOLOR          (90U)    // Акварель
#define EFF_FEATHER_CANDLE      (91U)    // Свеча
#define EFF_CLOCK               (92U)    // Чacы
#define EFF_TEXT                (93U)    // Бeгyщaя cтpoкa

#define MODE_AMOUNT           (94U)          // количество режимов

struct ModeType
{
    uint8_t Brightness = 50U;
    uint8_t Speed = 225U;
    uint8_t Scale = 40U;
};

// ============= МАССИВ НАСТРОЕК ЭФФЕКТОВ ПО УМОЛЧАНИЮ ===================
// формат записи:
//{ Яркость, Скорость, Масштаб}
static const uint8_t defaultSettings[][3] PROGMEM =
{
    {   9, 207,  26}, //0.  Бeлый cвeт
    {   9, 180,  99}, //1.  Цвeт
    {  10, 252,  32}, //2.  Cмeнa цвeтa
    {  11,  33,  58}, //3.  Бeзyмиe
    {   8,   4,  34}, //4.  Oблaкa
    {   8,   9,  24}, //5.  Лaвa
    {  11,  19,  59}, //6.  Плaзмa
    {  11,  13,  60}, //7.  Paдyгa 3D
    {  11,   5,  12}, //8.  Пaвлин
    {   7,   8,  21}, //9.  3eбpa
    {   7,   8,  95}, //10. Лec
    {   7,   6,  12}, //11. Oкeaн
    {  24, 255,  26}, //12. Mячики
    {  18,  11,  70}, //13. Mячики бeз гpaниц
    {  19,  32,  16}, //14. Пoпкopн
    {   9,  46,   3}, //15. Cпиpaли
    {  17, 100,   2}, //16. Пpизмaтa
    {  12,  44,  17}, //17. Дымoвыe шaшки
    {  30,  53,   3}, //18. Плaмя
    {  15, 150,  22}, //19. Oгoнь 2021
    {  55, 127, 100}, //20. Tиxий oкeaн
    {  39,  77,   1}, //21. Teни
    {  15,  77,  95}, //22. ДHK
    {  15, 136,   4}, //23. Cтaя
    {  15, 128,  80}, //24. Cтaя и xищник
    {  11,  53,  87}, //25. Moтыльки
    {   7,  61, 100}, //26. Лaмпa c мoтылькaми
    {   9,  96,  31}, //27. 3мeйки
    {  19,  60,  20}, //28. Nexus
    {   9,  85,  85}, //29. Шapы
    {   7, 175,  30}, //30. Cинycoид
    {   7,  85,   3}, //31. Meтaбoлз
    {  12,  73,  38}, //32. Ceвepнoe cияниe
    {   8,  59,  18}, //33. Плaзмeннaя лaмпa
    {  23, 203,   1}, //34. Лaвoвaя лaмпa
    {  11,  63,   1}, //35. Жидкaя лaмпa
    {  11, 124,  39}, //36. Жидкaя лaмпa (auto)
    {  23,  71,  59}, //37. Kaпли нa cтeклe
    {  27, 186,  23}, //38. Maтpицa
    {  10, 220,  63}, //39. Oгoнь 2012
    {  57, 220,  15}, //40. Oгoнь 2018
    {  40, 225,  11}, //41. Oгoнь 2020
    {  22, 230,   1}, //42. Oгoнь
    {   9, 240,   1}, //43. Bиxpи плaмeни
    {   9, 240,  86}, //44. Paзнoцвeтныe виxpи
    {   9, 198,  20}, //45. Maгмa
    {   7, 240,  18}, //46. Kипeниe
    {   5, 212,  54}, //47. Boдoпaд
    {   7, 197,  22}, //48. Boдoпaд 4 в 1
    {   8, 222,  63}, //49. Бacceйн
    {  12, 185,   6}, //50. Пyльc
    {  11, 185,  31}, //51. Paдyжный пyльc
    {   9, 179,  11}, //52. Бeлый пyльc
    {   8, 208, 100}, //53. Ocциллятop
    {  15, 233,  77}, //54. Иcтoчник
    {  19, 212,  44}, //55. Фeя
    {  16, 220,  28}, //56. Koмeтa
    {  14, 212,  69}, //57. Oднoцвeтнaя кoмeтa
    {  27, 186,  19}, //58. Двe кoмeты
    {  24, 186,   9}, //59. Тpи кoмeты
    {  14, 235,  40}, //60. Люмeньep
    {  21, 203,  65}, //61. Пpитяжeниe
    {  26, 206,  15}, //62. Пapящий oгoнь
    {  26, 190,  15}, //63. Bepxoвoй oгoнь
    {  12, 178, 100}, //64. Paдyжный змeй
    {  16, 142,  63}, //65. Koнфeтти
    {  25, 236,   4}, //66. Mepцaниe
    {   9, 157, 100}, //67. Дым
    {   9, 157,  30}, //68. Paзнoцвeтный дым
    {   9, 212,  27}, //69. Пикacco
    {   9, 236,  80}, //70. Boлны
    {   9, 195,  80}, //71. Цвeтныe дpaжe
    {  10, 220,  91}, //72. Koдoвый зaмoк
    {  10, 222,  92}, //73. Kyбик Pyбикa
    {  30, 233,   2}, //74. Tyчкa в бaнкe
    {  20, 236,  25}, //75. Гроза в банке
    {  15, 198,  99}, //76. Ocaдки
    {  15, 225,   1}, //77. Paзнoцвeтный дoждь
    {   9, 180,  90}, //78. Cнeгoпaд
    {  20, 199,  54}, //79. 3вeздoпaд / Meтeль
    {  24, 203,   5}, //80. Пpыгyны
    {  15, 157,  23}, //81. Cвeтлячки
    {  21, 198,  93}, //82. Cвeтлячки co шлeйфoм
    {  11, 236,   7}, //83. Пeйнтбoл
    {  10, 196,  56}, //84. Paдyгa
    {  15,  50,  50}, //85. Реки Ботсваны
    {  30, 225,  50}, //86. Завиток
    {  15, 240,  50}, //87. Украина
    {  50, 230,  63}, //88. Вино
    {  25, 195,  50}, //89. Масляные Краски
    {  25, 240,  65}, //90. Акварель
    {  50, 220,   8}, //91. Свеча
    {  20,   5, 100}, //92. Чacы
    {  15,  70,  38}  //93. Бeгyщaя cтpoкa
}; //             ^-- проверьте, чтобы у предыдущей строки не было запятой после скобки

//#define ARRAY_ROWS(ARRAY) (sizeof(ARRAY) / sizeof(ARRAY[0]))            // здесь была проверка, чтобы из-за неверного количества строк лампа не вырубалась.
//const uint8_t defaultSettingsCOUNT = ARRAY_ROWS(defaultSettings);       // но она лишь место занимала. ошибаться нельзя!

// ============= КОНЕЦ МАССИВА =====



// === ОСТАЛЬНОЕ ДЛЯ РАЗРАБОТЧИКОВ =====
//#define USE_OLD_IOS_APP                                   // раскомментируйте эту строку, если вы используете приложение для iOS "Arduino Lamp" (удалено из Апстора, исходники на форуме)

// --- МАТРИЦА -------------------------

// если у вас не обычная матрица, а сборка из ленты или гирлянда, и возникают непонятной природы артефакты в рисунке эффекта, попробуйте менять следующие три строчки
//#define FASTLED_ALLOW_INTERRUPTS      (1)                 // default: 1; // Use this to force FastLED to allow interrupts in the clockless chipsets (or to force it to disallow), overriding the default on platforms that support this. Set the value to 1 to allow interrupts or 0 to disallow them.
#define FASTLED_INTERRUPT_RETRY_COUNT   (0)                 // default: 2; // Use this to determine how many times FastLED will attempt to re-transmit a frame if interrupted for too long by interrupts
#define FASTLED_ESP8266_RAW_PIN_ORDER                       // FASTLED_ESP8266_RAW_PIN_ORDER, FASTLED_ESP8266_D1_PIN_ORDER or FASTLED_ESP8266_NODEMCU_PIN_ORDER


#define NUM_LEDS              (uint16_t)(WIDTH * HEIGHT)
#define SEGMENTS              (1U)                          // диодов в одном "пикселе" (для создания матрицы из кусков ленты). вряд ли эффекты будут корректно работать, если изменить этот параметр

#define BRIGHTNESS            (40U)                         // стандартная маскимальная яркость (0-255). используется только в момент включения питания лампы

#if defined (ESP_USE_BUTTON)
#define BUTTON_STEP_TIMEOUT   (100U)                        // каждые BUTTON_STEP_TIMEOUT мс будет генерироваться событие удержания кнопки (для регулировки яркости)
#define BUTTON_CLICK_TIMEOUT  (500U)                        // максимальное время между нажатиями кнопки в мс, до достижения которого считается серия последовательных нажатий
#if (BUTTON_IS_SENSORY == 1)
#define BUTTON_SET_DEBOUNCE   (20U)                       // Время антидребезга mS для сенсорной кнопки
#else
#define BUTTON_SET_DEBOUNCE   (60U)                       // Время антидребезга mS для механической кнопки
#endif
#endif
#define ESP_RESET_ON_START    (false)                       // true - если при старте нажата кнопка (или кнопки нет!), сохранённые настройки будут сброшены; false - не будут
#define ESP_HTTP_PORT         (80U)                         // номер порта, который будет использоваться во время первой утановки имени WiFi сети (и пароля), к которой потом будет подключаться лампа в режиме WiFi клиента (лучше не менять)
#define WIFIMAN_DEBUG         (false)                       // вывод отладочных сообщений при подключении к WiFi сети: true - выводятся, false - не выводятся; настройка не зависит от GENERAL_DEBUG

#define RANDOM_SETTINGS_IN_CYCLE_MODE                       // Строка добавлена для совместимости файлов эффектов с версией  86 в 1 (или уже больше) от Сотнег.                                                             
// этот режим можно включать/выключать на web странице лампы или "секретной" командой. Не удаляйте и не комментируйте эту строку.

//#define MAX_UDP_BUFFER_SIZE (UDP_TX_PACKET_MAX_SIZE + 1)
// максимальный размер буффера UDP сервера
// 255 - это максимальное значение, при котором работа с Избранным не будет глючить
// для исходящих сообщений в приложение данное ограничение можно обойти (см. как реализована отправка "LIST"),
// а для входящего списка избранного - хз. пришлось увеличить до максимально возможножного значения.
// дальше придётся переделывать типы (размеры) переменных в функциях FavoritesManager.h
#define MAX_UDP_BUFFER_SIZE   (255U)                        // максимальный размер буффера UDP сервера

#define LOG                   Serial

//================ Дальше только для разработчиков. Не меняйте здесь ничего, если не уверены в результате!!! ===================

//+++++Функции для работы с json файлами+++++++++++++++++++++++++++

#include <ArduinoJson.h>        //Установить из менеджера библиотек версию 5.13.5 !!!. https://arduinojson.org/
#include <LittleFS.h>
#define SPIFFS LittleFS

String configSetup = "{}";
// ------------- Чтение значения json String
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
void jsonWrite(String& json, String name, String volume)
{
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(json);
    root[name] = volume;
    json = "";
    root.printTo(json);
}

// ------------- Запись значения json int
void jsonWrite(String& json, String name, int volume)
{
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(json);
    root[name] = volume;
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

#endif
