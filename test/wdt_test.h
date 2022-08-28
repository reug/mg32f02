#ifndef WDT_TEST_H
#define WDT_TEST_H


/// Тест сторожевого таймера IWDT
void iwdt_test();

/// Тест сторожевого таймера WWDT
void wwdt_test();

/// Вывод статуса RST в терминал
void debug_reset_status();


#endif // WDT_TEST_H
