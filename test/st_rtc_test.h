#ifndef ST_RTC_TEST_H
#define ST_RTC_TEST_H

/// Чтение значения CURRENT после сброса
void systick_test_read_current();

/// Тест SysTick 1
void systick_test1();

/// Функция тестирования SysTick Timer
void systick_test();

/// Вывод счетных импульсов на RTC_OUT
void rtc_test_clock();

/// Тестирование режима ALARM с прерыванием
void rtc_test_alarm();

#endif // ST_RTC_TEST_H
