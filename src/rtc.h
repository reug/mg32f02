#ifndef RTC_H
#define RTC_H

#include <stdint.h>

/// Инициализация RTC
void rtc_init();

/// Включение прерывания INT_SYS по флагам, указанным в flags согласно формату RTC_INT
void rtc_set_int(uint8_t flags);

/// Разблокировка записи
void rtc_write_unlock();

/// Блокировка записи
void rtc_write_lock();

/// Подключение выхода. Сигнал определяется out_sel (0-3) согласно формату RTC_OUT_SEL.
/// Разблокировка не требуется.
void rtc_out(uint8_t out_sel);

#endif // RTC_H
