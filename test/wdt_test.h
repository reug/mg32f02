#ifndef WDT_TEST_H
#define WDT_TEST_H

/// Вывод статуса RST в терминал
void debug_reset_status();

/// Тест сторожевого таймера IWDT
void iwdt_test();

/// Тест сторожевого таймера WWDT
void wwdt_test();

/// Выключение "горячего" сброса самого таймера WWDT
void rst_wwdt_wdis();


#endif // WDT_TEST_H
